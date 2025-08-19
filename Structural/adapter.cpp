// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Adapter Pattern                                               ┃
// ┃  PURPOSE   ::  Convert interface of a class to another interface clients expect ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    adapter.cpp
 * @brief   现代 C++20 实现适配器模式
 * @details 
 *          适配器模式（Adapter Pattern）是一种结构型设计模式，将一个类的接口转换成客户端期望的另一个接口。
 *          它使得原本由于接口不兼容而不能一起工作的类可以协同工作，就像电源插头转换器一样。
 * 
 * @par 核心思想
 *      - **对象适配器**：通过组合（has-a）包装旧接口
 *      - **类适配器**：通过多重继承（is-a）实现（C++ 中不推荐）
 *      - 客户端只依赖目标接口，不关心适配细节
 * 
 * @note    本实现使用对象适配器 + 智能指针，避免内存泄漏
 * @warning 避免使用类适配器（多继承易出问题）
 * @todo    支持运行时插件式适配器注册
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>

using std::println;

// ─────────────────────────────────────────── 目标接口：两孔插座 ───────────────────────────────────────────

/**
 * @brief 目标接口：客户端期望的接口（两孔插座）
 */
class TwoPinOutlet {
public:
    virtual ~TwoPinOutlet() = default;

    /**
     * @brief 开始充电（两孔）
     */
    virtual void chargeWithTwoPins() const = 0;
};

// ─────────────────────────────────────────── 被适配者：三孔插座 ───────────────────────────────────────────

/**
 * @brief 被适配的类：三孔插座（已有系统）
 */
class ThreePinOutlet {
public:
    void chargeWithThreePins() const {
        println("🔌 开始三孔插座充电（220V）");
    }
};

// ─────────────────────────────────────────── 对象适配器（推荐） ───────────────────────────────────────────

/**
 * @brief 两转三插头适配器（对象适配器）
 * @note 我们这里通过继承目标适配器类，包含被适配的类，并实现适配器类的功能
 * @details 使用组合（has-a）包装 ThreePinOutlet
 */
class TwoToThreePinAdapter : public TwoPinOutlet {
private:
    std::unique_ptr<ThreePinOutlet> outlet;

public:
    /**
     * @brief 构造函数
     * @param out 三孔插座对象（转移所有权）
     */
    explicit TwoToThreePinAdapter(std::unique_ptr<ThreePinOutlet> out)
        : outlet(std::move(out)) {}

    void chargeWithTwoPins() const override {
        println("🔄 适配器：将两孔信号转换为三孔");
        outlet->chargeWithThreePins();
    }
};

// ─────────────────────────────────────────── 类适配器（不推荐） ───────────────────────────────────────────

/**
 * @brief 类适配器：多重继承（C++ 中不推荐）
 * @warning 多重继承可能导致二义性、菱形继承等问题
 */
class ClassAdapter : public TwoPinOutlet, private ThreePinOutlet {
public:
    void chargeWithTwoPins() const override {
        println("🔄 类适配器：将两孔转换为三孔");
        ThreePinOutlet::chargeWithThreePins();
    }
};

// ─────────────────────────────────────────── 更复杂的适配：国标转换 ───────────────────────────────────────────

/**
 * @brief 美标插座（110V）
 */
class USOutlet {
public:
    void charge110V() const {
        println("⚡ 美标插座充电（110V）");
    }
};

/**
 * @brief 国际旅行适配器：将国标转为美标
 * @note 这里还是一样，我们传入一个美标插座，然后通过适配器将国标插座转为美标插座
 */
class TravelAdapter : public TwoPinOutlet {
private:
    std::unique_ptr<USOutlet> usOutlet;

public:
    explicit TravelAdapter(std::unique_ptr<USOutlet> outlet)
        : usOutlet(std::move(outlet)) {}

    void chargeWithTwoPins() const override {
        println("🌍 旅行适配器：中国220V → 美国110V 转换");
        usOutlet->charge110V();
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 使用两孔接口充电
 * @param outlet 适配器或具体实现
 */
void charge(TwoPinOutlet &outlet) {
    outlet.chargeWithTwoPins();
}

/**
 * @brief 测试对象适配器
 */
void testObjectAdapter() {
    println("\n🧪 测试 1: 对象适配器（推荐）");

    auto threePin = std::make_unique<ThreePinOutlet>();
    auto adapter = std::make_unique<TwoToThreePinAdapter>(std::move(threePin));

    charge(*adapter);
}

/**
 * @brief 测试类适配器（不推荐）
 */
void testClassAdapter() {
    println("\n🧪 测试 2: 类适配器（不推荐，慎用）");

    ClassAdapter adapter;
    charge(adapter);
}

/**
 * @brief 测试旅行适配器（真实场景）
 */
void testTravelAdapter() {
    println("\n🧪 测试 3: 国际旅行适配器");

    auto usOutlet = std::make_unique<USOutlet>();
    auto adapter = std::make_unique<TravelAdapter>(std::move(usOutlet));

    charge(*adapter);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 适配器模式");

    testObjectAdapter();
    testClassAdapter();
    testTravelAdapter();

    println("\n✅ 所有测试完成！");
    return 0;
}