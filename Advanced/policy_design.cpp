// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Policy-Based Design                                           ┃
// ┃  PURPOSE   ::  Compose classes from reusable policy components              ┃
// ┃  CATEGORY  ::  Modern C++ Idiom                                             ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    policy_design.cpp
 * @brief   策略式设计：通过模板组合行为
 * @details 
 *          策略式设计是一种基于模板的设计模式，将不同的功能分解为独立的“策略类”，
 *          然后通过模板继承将它们组合成一个功能完整的类。它支持编译时多态和零成本抽象。
 * 
 * @par 核心思想
 *      - 每个策略封装一种独立行为
 *      - 主类模板继承多个策略
 *      - 行为在编译时确定，无运行时开销
 * 
 * @note    本实现展示创建、验证、日志、线程安全策略
 * @warning 策略间不应有强依赖
 * @todo    支持策略依赖注入
 * @date    2025-08-19
 * @version 1.0
 */


#include <string>
#include <mutex>
#include <print>

using std::println;

// ─────────────────────────────────────────── 1️⃣ 创建策略（Creation Policy） ───────────────────────────────────────────

struct OpNewCreator {
    static void* create() {
        println("🔧 使用 new 创建对象");
        return new int(42);
    }
    static void destroy(void* p) {
        println("🧹 使用 delete 销毁对象");
        delete static_cast<int*>(p);
    }
};

struct MallocCreator {
    static void* create() {
        println("🔧 使用 malloc 创建对象");
        return std::malloc(sizeof(int));
    }
    static void destroy(void* p) {
        println("🧹 使用 free 销毁对象");
        std::free(p);
    }
};

// ─────────────────────────────────────────── 2️⃣ 验证策略（Validation Policy） ───────────────────────────────────────────

struct NoValidation {
    static void validate(int value) {
        // 无验证
    }
};

struct RangeValidation {
    static void validate(int value) {
        if (value < 0 || value > 100) {
            throw std::invalid_argument(std::format("值 {} 超出范围 [0,100]", value));
        }
        println("✅ 值 {} 通过范围验证", value);
    }
};

// ─────────────────────────────────────────── 3️⃣ 日志策略（Logging Policy） ───────────────────────────────────────────

struct NoLogging {
    static void log(const std::string& msg) {
        // 无日志
    }
};

struct ConsoleLogging {
    static void log(const std::string& msg) {
        println("📝 [LOG] {}", msg);
    }
};

// ─────────────────────────────────────────── 4️⃣ 线程安全策略（Threading Policy） ───────────────────────────────────────────

struct SingleThreaded {
    using MutexType = std::false_type;
    static void lock() { /* 无锁 */ }
    static void unlock() { /* 无锁 */ }
};

struct MultiThreaded {
    using MutexType = std::true_type;
    static std::mutex mtx;
    static void lock() { mtx.lock(); }
    static void unlock() { mtx.unlock(); }
};

std::mutex MultiThreaded::mtx;

// ─────────────────────────────────────────── 主类模板：Widget ───────────────────────────────────────────

template<
    typename CreationPolicy = OpNewCreator,
    typename ValidationPolicy = NoValidation,
    typename LoggingPolicy = NoLogging,
    typename ThreadingPolicy = SingleThreaded
>
class Widget : public CreationPolicy, public ValidationPolicy, public LoggingPolicy, public ThreadingPolicy {
    int* data;

public:
    Widget() : data(nullptr) {
        LoggingPolicy::log("构造 Widget");
        ThreadingPolicy::lock();
        try {
            data = static_cast<int*>(CreationPolicy::create());
            if (data) *data = 0;
        } catch (...) {
            ThreadingPolicy::unlock();
            throw;
        }
        ThreadingPolicy::unlock();
    }

    void setValue(int value) {
        ThreadingPolicy::lock();
        LoggingPolicy::log(std::format("设置值: {}", value));
        ValidationPolicy::validate(value);
        if (data) *data = value;
        ThreadingPolicy::unlock();
    }

    int getValue() const {
        ThreadingPolicy::lock();
        int val = data ? *data : -1;
        ThreadingPolicy::unlock();
        return val;
    }

    ~Widget() {
        LoggingPolicy::log("析构 Widget");
        ThreadingPolicy::lock();
        if (data) {
            CreationPolicy::destroy(data);
            data = nullptr;
        }
        ThreadingPolicy::unlock();
    }
};

// ─────────────────────────────────────────── 类型别名：组合策略 ───────────────────────────────────────────

using BasicWidget = Widget<OpNewCreator, NoValidation, NoLogging, SingleThreaded>;

using SafeWidget = Widget<
    OpNewCreator,
    RangeValidation,
    ConsoleLogging,
    SingleThreaded
>;

using ThreadSafeWidget = Widget<
    OpNewCreator,
    RangeValidation,
    ConsoleLogging,
    MultiThreaded
>;

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testBasicWidget() {
    println("\n🧪 1. 基础 Widget（无策略）");
    BasicWidget w;
    w.setValue(10);
    println("值: {}", w.getValue());
}

void testSafeWidget() {
    println("\n🧪 2. 安全 Widget（验证 + 日志）");
    SafeWidget w;
    w.setValue(50);
    try {
        w.setValue(150);  // 超出范围
    } catch (const std::exception& e) {
        println("❌ 异常: {}", e.what());
    }
}

void testThreadSafeWidget() {
    println("\n🧪 3. 线程安全 Widget");
    ThreadSafeWidget w;
    w.setValue(75);
    println("值: {}", w.getValue());
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

int main() {
    println("🎯 开始测试策略式设计");

    testBasicWidget();
    testSafeWidget();
    testThreadSafeWidget();

    println("\n✅ 所有测试完成！");
    return 0;
}