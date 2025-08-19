// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Prototype Pattern                                             ┃
// ┃  PURPOSE   ::  Create new objects by copying existing instances             ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    prototype.cpp
 * @brief   现代 C++20 实现原型模式
 * @details 
 *          原型模式（Prototype Pattern）是一种创建型设计模式，通过克隆现有对象来创建新对象，
 *          而无需知道其创建细节。适用于对象创建成本高、结构复杂或运行时动态变化的场景。
 * 
 * @par 核心思想
 *      - 所有可克隆对象实现 `clone()` 方法
 *      - 客户端通过 `clone()` 获得副本，无需调用构造函数
 *      - 支持深拷贝，避免共享状态
 * 
 * @note    本实现使用 std::unique_ptr 避免内存泄漏
 * @warning 如果对象包含指针成员，必须实现深拷贝
 * @todo    支持对象池（Object Pool）优化频繁克隆场景
 * @date    2025-08-19
 * @version 1.0
 */

#include <format>
#include <memory>
#include <print>
#include <sstream>
#include <string>
#include <vector>


using std::println;


static std::string join(const std::vector<std::string> &vec, const std::string &delimiter) {
    if (vec.empty()) return "";

    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) oss << delimiter;
        oss << vec[i];
    }
    return oss.str();
}

// ─────────────────────────────────────────── 抽象原型基类 ───────────────────────────────────────────

/**
 * @brief 抽象原型类：所有可克隆对象的基类
 */
class Prototype {
public:
    virtual ~Prototype() = default;

    /**
     * @brief 克隆接口：返回对象的深拷贝
     * @return std::unique_ptr<Prototype> 新对象副本
     */
    virtual std::unique_ptr<Prototype> clone() const = 0;

    /**
     * @brief 显示对象信息（用于测试）
     */
    virtual void display() const = 0;
};

// ─────────────────────────────────────────── 具体原型：猴子（WuKong） ───────────────────────────────────────────

/**
 * @brief 孙悟空类：可克隆的具体原型
 * @details 支持深拷贝，包含字符串和技能列表
 */
class WuKong : public Prototype {
private:
    std::string name;               ///< 名字
    std::vector<std::string> skills;///< 技能列表（模拟复杂状态）

public:
    /**
     * @brief 构造函数
     * @param n 名字
     */
    explicit WuKong(std::string n) : name(std::move(n)) {
        skills = {"72变", "筋斗云", "金箍棒"};
    }

    /**
     * @brief 拷贝构造函数（深拷贝）
     * @param other 源对象
     */
    WuKong(const WuKong &other)
        : name(other.name), skills(other.skills)// std::vector 自动深拷贝
    {
        println("🧩 WuKong 深拷贝: {}", name);
    }

    /**
     * @brief 赋值运算符
     */
    WuKong &operator=(const WuKong &other) = default;

    /**
     * @brief 克隆实现
     * @return 新的 WuKong 副本
     */
    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<WuKong>(*this);
    }

    /**
     * @brief 显示信息
     */
    void display() const override {
        println("🐒 {} 正在玩原神，技能: {}", name, join(skills, ", "));
    }

    /**
     * @brief 修改技能（用于测试副本独立性）
     */
    void addSkill(const std::string &skill) {
        skills.push_back(skill);
    }
};

// ─────────────────────────────────────────── 带指针成员的深拷贝示例 ───────────────────────────────────────────

/**
 * @brief 演示深拷贝：包含原始指针的类
 * @warning 必须手动实现深拷贝
 */
class MonkeyWithPointer : public Prototype {
private:
    std::string *pName;// 指针成员（仅用于演示）

public:
    explicit MonkeyWithPointer(std::string name) {
        pName = new std::string(std::move(name));
    }

    // 拷贝构造函数（深拷贝）
    MonkeyWithPointer(const MonkeyWithPointer &other) {
        pName = new std::string(*other.pName);// 深拷贝
        println("🧩 MonkeyWithPointer 深拷贝: {}", *pName);
    }

    // 析构函数
    ~MonkeyWithPointer() override {
        delete pName;
    }

    // 赋值运算符（可选）
    MonkeyWithPointer &operator=(const MonkeyWithPointer &other) {
        if (this != &other) {
            *pName = *other.pName;
        }
        return *this;
    }

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<MonkeyWithPointer>(*this);
    }

    void display() const override {
        println("🐵 指针版: {}", *pName);
    }
};

// ─────────────────────────────────────────── CRTP 通用克隆基类（高级技巧） ───────────────────────────────────────────

/**
 * @brief CRTP 基类：自动实现 clone()
 * @tparam Derived 派生类类型
 * @note 使用方式: class Derived : public Clonable<Derived>
 */
template<typename Derived>
class Clonable : public Prototype {
public:
    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived &>(*this));
    }
};

// 使用 CRTP 的简化版本
class SimpleMonkey : public Clonable<SimpleMonkey> {
    std::string name;

public:
    explicit SimpleMonkey(std::string n) : name(std::move(n)) {}

    void display() const override {
        println("🐵 Simple: {}", name);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本克隆功能
 */
void testBasicClone() {
    println("\n🧪 测试 1: 基本克隆功能");

    auto wukong = std::make_unique<WuKong>("黑神话悟空");
    wukong->display();

    auto clone = wukong->clone();
    clone->display();

    // 修改副本，验证独立性
    auto kong2 = dynamic_cast<WuKong *>(clone.get());
    if (kong2) {
        kong2->addSkill("新技能");
        println("🧩 修改副本后:");
        kong2->display();
        println("🧩 原对象不变:");
        wukong->display();
    }
}

/**
 * @brief 测试指针成员深拷贝
 */
void testDeepCopy() {
    println("\n🧪 测试 2: 指针成员深拷贝");

    auto monkey = std::make_unique<MonkeyWithPointer>("指针悟空");
    monkey->display();

    auto clone = monkey->clone();
    clone->display();
}

/**
 * @brief 测试 CRTP 简化克隆
 */
void testCRTP() {
    println("\n🧪 测试 3: CRTP 简化克隆");

    SimpleMonkey original("CRTP悟空");
    original.display();

    auto clone = original.clone();
    clone->display();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 原型模式");

    testBasicClone();
    testDeepCopy();
    testCRTP();

    println("\n✅ 所有测试完成！");
    return 0;
}