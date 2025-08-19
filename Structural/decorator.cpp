// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Decorator Pattern                                             ┃
// ┃  PURPOSE   ::  Dynamically add responsibilities to an object               ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    decorator.cpp
 * @brief   现代 C++20 实现装饰者模式
 * @details 
 *          装饰者模式（Decorator Pattern）是一种结构型设计模式，允许动态地为对象添加功能，
 *          而无需修改其代码。它通过组合而非继承实现功能扩展，避免了类爆炸问题。
 * 
 * @par 核心思想
 *      - 抽象组件（Component）定义接口
 *      - 具体组件（ConcreteComponent）提供基础行为
 *      - 装饰器（Decorator）持有组件指针，增强其功能
 *      - 客户端可以按需组合装饰器
 * 
 * @note    本实现使用 std::unique_ptr 自动管理装饰链
 * @warning 不要循环装饰（会导致无限递归）
 * @todo    支持运行时移除装饰器
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>

using std::println;

// ─────────────────────────────────────────── 组件接口：Component ───────────────────────────────────────────

/**
 * @brief 抽象组件：所有对象的公共接口
 */
class Component {
public:
    virtual ~Component() = default;

    /**
     * @brief 显示对象状态（如穿着）
     */
    virtual void display() const = 0;
};

// ─────────────────────────────────────────── 具体组件：Girl ───────────────────────────────────────────

/**
 * @brief 裸体女孩（基础组件）
 */
class Girl : public Component {
public:
    void display() const override {
        println("👧 naked girl");
    }
};

// ─────────────────────────────────────────── 抽象装饰器 ───────────────────────────────────────────

/**
 * @brief 抽象装饰器：持有组件指针
 */
class Decorator : public Component {
protected:
    std::unique_ptr<Component> component;

public:
    explicit Decorator(std::unique_ptr<Component> comp)
        : component(std::move(comp)) {}
};

// ─────────────────────────────────────────── 具体装饰器：裙子、丝袜等 ───────────────────────────────────────────

/**
 * @brief 迷你裙装饰器
 */
class Miniskirt : public Decorator {
public:
    using Decorator::Decorator;

    void display() const override {
        component->display();// 先显示原有状态
        println("🎀 wear Miniskirt");
    }
};

/**
 * @brief 黑丝装饰器
 */
class BlackSilk : public Decorator {
public:
    using Decorator::Decorator;

    void display() const override {
        component->display();
        println("🧦 wear BlackSilk");
    }
};

/**
 * @brief 高跟鞋装饰器
 */
class HighHeels : public Decorator {
public:
    using Decorator::Decorator;

    void display() const override {
        component->display();
        println("👠 wear HighHeels");
    }
};

// ─────────────────────────────────────────── 流式装饰器构建（现代 C++ 增强） ───────────────────────────────────────────

/**
 * @brief 通用装饰函数
 * @tparam DecoratorType 装饰器类型
 * @param comp 原始组件
 * @return 装饰后的组件
 */
template<typename DecoratorType>
std::unique_ptr<DecoratorType> decorate(std::unique_ptr<Component> comp) {
    return std::make_unique<DecoratorType>(std::move(comp));
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本装饰功能
 */
void testBasicDecorator() {
    println("\n🧪 测试 1: 基本装饰模式");

    auto girl = std::make_unique<Girl>();

    auto withSkirt = std::make_unique<Miniskirt>(std::move(girl));
    withSkirt->display();

    auto withSilk = std::make_unique<BlackSilk>(std::move(withSkirt));
    withSilk->display();
}

/**
 * @brief 测试流式装饰（函数模板）
 */
void testFluentDecorator() {
    println("\n🧪 测试 2: 流式装饰（模板函数）");

    auto look = decorate<HighHeels>(
            decorate<BlackSilk>(
                    decorate<Miniskirt>(
                            std::make_unique<Girl>())));

    println("👠 + 🧦 + 🎀 组合：");
    look->display();
}

/**
 * @brief 测试不同装饰顺序
 */
void testOrderMatters() {
    println("\n🧪 测试 3: 装饰顺序影响");

    // 先丝袜后裙子
    auto look1 = decorate<Miniskirt>(decorate<BlackSilk>(std::make_unique<Girl>()));
    println("先丝袜后裙子：");
    look1->display();

    // 先裙子后丝袜
    auto look2 = decorate<BlackSilk>(decorate<Miniskirt>(std::make_unique<Girl>()));
    println("先裙子后丝袜：");
    look2->display();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 装饰者模式");

    testBasicDecorator();
    testFluentDecorator();
    testOrderMatters();

    println("\n✅ 所有测试完成！");
    return 0;
}