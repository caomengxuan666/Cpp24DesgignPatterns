// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Factory Method Pattern                                        ┃
// ┃  PURPOSE   ::  Defer object creation to subclasses                          ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    factory_method.cpp
 * @brief   现代 C++20 实现工厂方法模式
 * @details 
 *          工厂方法模式（Factory Method Pattern）是一种创建型设计模式，它定义了一个创建对象的接口，
 *          但由子类决定实例化哪一个类。工厂方法让类的实例化延迟到子类。
 * 
 * @par 核心思想
 *      - 抽象工厂（AbstractFactory）声明创建产品的接口
 *      - 具体工厂（ConcreteFactory）实现创建具体产品的逻辑
 *      - 客户端只依赖抽象工厂和抽象产品，不关心具体实现
 * 
 * @note    本实现使用 std::unique_ptr 避免内存泄漏
 * @warning 不要直接 new 工厂或产品，应通过智能指针管理
 * @todo    支持工厂注册机制，实现插件式扩展
 * @date    2025-08-19
 * @version 1.0
 */

#include <format>
#include <memory>
#include <print>


using std::println;

// ─────────────────────────────────────────── 抽象产品 ───────────────────────────────────────────

/**
 * @brief 抽象产品类：所有交通工具的基类
 * @details 定义了所有具体产品必须实现的接口
 */
class AbstractProduct {
public:
    virtual ~AbstractProduct() = default;

    /**
     * @brief 制造产品（纯虚函数）
     */
    virtual void makeProduct() const = 0;
};

// ─────────────────────────────────────────── 具体产品 ───────────────────────────────────────────

/**
 * @brief 飞机产品
 */
class PlaneProduct : public AbstractProduct {
public:
    void makeProduct() const override {
        println("✈️  Here the plane comes");
    }
};

/**
 * @brief 火箭产品
 */
class RocketProduct : public AbstractProduct {
public:
    void makeProduct() const override {
        println("🚀  Here the rocket comes");
    }
};

// ─────────────────────────────────────────── 抽象工厂 ───────────────────────────────────────────

/**
 * @brief 抽象工厂类：声明创建产品的接口
 * @details 所有具体工厂必须实现 createProduct 方法
 */
class AbstractFactory {
public:
    virtual ~AbstractFactory() = default;

    /**
     * @brief 创建产品对象
     * @return std::unique_ptr<AbstractProduct> 智能指针管理的产品
     */
    virtual std::unique_ptr<AbstractProduct> createProduct() const = 0;
};

// ─────────────────────────────────────────── 具体工厂 ───────────────────────────────────────────

/**
 * @brief 飞机工厂：创建 PlaneProduct
 */
class PlaneFactory : public AbstractFactory {
public:
    std::unique_ptr<AbstractProduct> createProduct() const override {
        return std::make_unique<PlaneProduct>();
    }
};

/**
 * @brief 火箭工厂：创建 RocketProduct
 */
class RocketFactory : public AbstractFactory {
public:
    std::unique_ptr<AbstractProduct> createProduct() const override {
        return std::make_unique<RocketProduct>();
    }
};

// ─────────────────────────────────────────── 模板工厂（现代 C++ 增强） ───────────────────────────────────────────

/**
 * @brief 模板工厂：使用 CRTP 避免重复实现,也就是说我们可以不用写具体工厂了，只需要用到模板工厂接口，然后创建具体工厂即可
 * @tparam ProductType 要创建的具体产品类型
 * @note 一行代码即可创建新工厂，符合 DRY 原则
 */
template<typename ProductType>
class SimpleFactory : public AbstractFactory {
public:
    std::unique_ptr<AbstractProduct> createProduct() const override {
        return std::make_unique<ProductType>();
    }
};

// 类型别名简化使用
using ModernPlaneFactory = SimpleFactory<PlaneProduct>;
using ModernRocketFactory = SimpleFactory<RocketProduct>;

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试传统工厂方法
 */
void testTraditional() {
    println("\n🧪 测试 1: 传统工厂方法");

    // 飞机
    std::unique_ptr<AbstractFactory> factory = std::make_unique<PlaneFactory>();
    std::unique_ptr<AbstractProduct> product = factory->createProduct();
    product->makeProduct();

    // 火箭
    factory = std::make_unique<RocketFactory>();
    product = factory->createProduct();
    product->makeProduct();
}

/**
 * @brief 测试模板工厂（现代 C++ 风格）
 */
void testModern() {
    println("\n🧪 测试 2: 模板工厂（CRTP 风格）");

    ModernPlaneFactory planeFactory;
    auto plane = planeFactory.createProduct();
    plane->makeProduct();

    ModernRocketFactory rocketFactory;
    auto rocket = rocketFactory.createProduct();
    rocket->makeProduct();
}

/**
 * @brief 测试扩展性：新增一个“汽车”产品
 */
void testExtensibility() {
    println("\n🧪 测试 3: 扩展性测试（新增汽车产品）");

    // 新增产品（无需修改任何已有工厂接口）
    class CarProduct : public AbstractProduct {
    public:
        void makeProduct() const override {
            println("🚗  Here the car comes");
        }
    };

    // 新增工厂（只需一行）
    using CarFactory = SimpleFactory<CarProduct>;

    CarFactory carFactory;
    auto car = carFactory.createProduct();
    car->makeProduct();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 工厂方法模式");

    testTraditional();
    testModern();
    testExtensibility();

    println("\n✅ 所有测试完成！");
    return 0;
}