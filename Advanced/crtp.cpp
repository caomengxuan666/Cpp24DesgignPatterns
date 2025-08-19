// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  CRTP (5 Implementations)                                      ┃
// ┃  PURPOSE   ::  Static polymorphism, mixins, operator generation              ┃
// ┃  CATEGORY  ::  C++ Idiom                                                   ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    crtp.cpp
 * @brief   CRTP 惯用法的 5 种现代实现
 * @details 
 *          CRTP（奇异递归模板模式）是一种 C++ 惯用法，通过模板将派生类作为参数传给基类，
 *          实现静态多态、混入、操作符生成等高级功能，零运行时开销。
 * 
 * @par 5 种实现方式
 *      1. 静态多态：替代虚函数
 *      2. 奇异递归基类：基类调用派生类
 *      3. 混入组合：多个行为组合
 *      4. 操作符生成器：自动生成比较操作
 *      5. 单例基类：线程安全单例
 * 
 * @note    所有实现均为编译时多态，无虚表开销
 * @warning 派生类必须继承自 CRTP<T>
 * @todo    结合 Concepts 约束模板参数
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <mutex>
#include <print>
#include <string>
#include <format>

using std::println;

// ─────────────────────────────────────────── 1️⃣ 静态多态（Static Polymorphism） ───────────────────────────────────────────

/**
 * @brief 静态多态：替代虚函数，零成本抽象
 */
template<typename T>
class Shape {
public:
    void draw() const {
        static_cast<const T *>(this)->drawImpl();
    }

    double area() const {
        return static_cast<const T *>(this)->areaImpl();
    }
};

class Circle : public Shape<Circle> {
    double radius;

public:
    explicit Circle(double r) : radius(r) {}

    void drawImpl() const {
        println("⭕ Circle: radius={:.2f}", radius);
    }

    double areaImpl() const {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape<Rectangle> {
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void drawImpl() const {
        println("▭ Rectangle: {} x {}", width, height);
    }

    double areaImpl() const {
        return width * height;
    }
};

// ─────────────────────────────────────────── 2️⃣ 奇异递归基类（EnableSharedFromThis 模拟） ───────────────────────────────────────────

/**
 * @brief 基类调用派生类方法（类似 enable_shared_from_this）
 */
template<typename T>
class EnableCRTPShared : public std::enable_shared_from_this<T>{
public:
    std::shared_ptr<T> shared_from_crtp() {
        return std::static_pointer_cast<T>(shared_from_this());
    }

    std::shared_ptr<const T> shared_from_crtp() const {
        return std::static_pointer_cast<const T>(shared_from_this());
    }

private:
    // 派生类必须实现 shared_from_this()
    std::shared_ptr<void> shared_from_this();
    std::shared_ptr<const void> shared_from_this() const;
};

// 用户类
class Task : public EnableCRTPShared<Task> {
public:
    void process() {
        auto self = shared_from_crtp();
        println("🔧 Task 正在处理...");
    }
};

// 实现 shared_from_this 代理
template<typename T>
std::shared_ptr<void> EnableCRTPShared<T>::shared_from_this() {
    return std::enable_shared_from_this<T>::shared_from_this();
}

template<typename T>
std::shared_ptr<const void> EnableCRTPShared<T>::shared_from_this() const {
    return std::enable_shared_from_this<T>::shared_from_this();
}

// ─────────────────────────────────────────── 3️⃣ 混入组合（Mixin Composition） ───────────────────────────────────────────

/**
 * @brief 混入：组合多个行为
 */
template<typename T>
class Loggable {
public:
    void log(const std::string &msg) const {
        println("📝 [LOG] {}", msg);
    }
};

template<typename T>
class Serializable {
public:
    std::string serialize() const {
        return static_cast<const T *>(this)->toJson();
    }
};

// 组合：可日志 + 可序列化
class User : public Loggable<User>, public Serializable<User> {
    std::string name;
    int age;

public:
    User(std::string n, int a) : name(std::move(n)), age(a) {}

    std::string toJson() const {
        return std::format(R"({{"name":"{}","age":{}}})", name, age);
    }

    void greet() const {
        log(std::format("Hello, I'm {}", name));
    }
};

// ─────────────────────────────────────────── 4️⃣ 操作符生成器（Operator Generator） ───────────────────────────────────────────

/**
 * @brief 自动生成比较操作符
 */
template<typename T>
class Orderable {
public:
    bool operator<(const T &other) const {
        return static_cast<const T*>(this)->value() < static_cast<const T&>(other).value();
    }

    bool operator>(const T &other) const {
        return other < *static_cast<const T *>(this);
    }

    bool operator<=(const T &other) const {
        return !(other < *static_cast<const T *>(this));
    }

    bool operator>=(const T &other) const {
        return !(*static_cast<const T *>(this) < other);
    }

    bool operator==(const T &other) const {
        return !(*static_cast<const T *>(this) < other) && !(other < *static_cast<const T *>(this));
    }

    bool operator!=(const T &other) const {
        return !(*static_cast<const T *>(this) == other);
    }
};

class Version : public Orderable<Version> {
    int major, minor;

public:
    Version(int m, int n) : major(m), minor(n) {}

    int value() const {
        return major * 1000 + minor;
    }

    void print() const {
        println("📦 Version {}.{}", major, minor);
    }
};

// ─────────────────────────────────────────── 5️⃣ 单例基类（CRTP Singleton） ───────────────────────────────────────────

/**
 * @brief 线程安全单例基类
 */
template<typename T>
class Singleton {
    static std::unique_ptr<T> instance;
    static std::once_flag flag;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    static T &getInstance() {
        std::call_once(flag, [] {
            instance = std::unique_ptr<T>(new T);
        });
        return *instance;
    }
};

template<typename T>
std::unique_ptr<T> Singleton<T>::instance;
template<typename T>
std::once_flag Singleton<T>::flag;

class Logger : public Singleton<Logger> {
    friend class Singleton<Logger>;

    Logger() {
        println("✅ Logger 初始化");
    }

public:
    void log(const std::string &msg) {
        println("🕮 {}", msg);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testStaticPolymorphism() {
    println("\n🧪 1. 静态多态");
    Circle c(3.0);
    Rectangle r(4, 5);

    c.draw();
    println("面积: {:.2f}", c.area());

    r.draw();
    println("面积: {:.2f}", r.area());
}

void testCRTPShared() {
    println("\n🧪 2. 奇异递归基类");
    auto task = std::make_shared<Task>();
    task->process();
}

void testMixin() {
    println("\n🧪 3. 混入组合");
    User user("Alice", 30);
    user.greet();
    println("序列化: {}", user.serialize());
}

void testOperatorGenerator() {
    println("\n🧪 4. 操作符生成器");

    Version v1(1, 2), v2(1, 5), v3(2, 0);

    v1.print();
    v2.print();
    v3.print();

    println("v1 < v2: {}", v1 < v2);
    println("v2 == v3: {}", v2 == v3);
    println("v3 >= v2: {}", v3 >= v2);
}

void testSingleton() {
    println("\n🧪 5. CRTP 单例");
    Logger::getInstance().log("第一次日志");
    Logger::getInstance().log("第二次日志");
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试 CRTP 的 5 种实现");

    testStaticPolymorphism();
    testCRTPShared();
    testMixin();
    testOperatorGenerator();
    testSingleton();

    println("\n✅ 所有测试完成！");
    return 0;
}