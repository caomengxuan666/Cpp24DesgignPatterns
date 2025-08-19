// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Abstract Factory Pattern                                      ┃
// ┃  PURPOSE   ::  Create families of related objects without specifying class  ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    abstract_factory.cpp
 * @brief   现代 C++20 实现抽象工厂模式
 * @details 
 *          抽象工厂模式（Abstract Factory Pattern）是一种创建型设计模式，用于创建一组相关或依赖对象的家族，
 *          而无需指定具体类。它常用于 UI 框架、数据库驱动、主题系统等需要“产品族”一致性的场景。
 * 
 * @par 核心思想
 *      - 抽象工厂（AbstractFactory）声明创建多个产品的接口
 *      - 具体工厂（ConcreteFactory）实现创建某一族产品的逻辑
 *      - 客户端只依赖抽象工厂和抽象产品，不关心具体实现
 * 
 * @note    本实现使用 std::unique_ptr 避免内存泄漏
 * @warning 不要直接 new 工厂或产品，应通过智能指针管理
 * @todo    支持工厂注册机制，实现插件式扩展
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>


using std::println;
// ─────────────────────────────────────────── 抽象产品 ───────────────────────────────────────────

/**
 * @brief 抽象产品基类：所有设备的共同接口
 */
class Product {
public:
    virtual ~Product() = default;
    virtual void show() const = 0;
};

/**
 * @brief 键盘抽象产品
 */
class Keyboard : public Product {
public:
    void show() const override {
        println("🔧  Keyboard: {}", getBrand());
    }
    virtual std::string getBrand() const = 0;
};

/**
 * @brief 鼠标抽象产品
 */
class Mouse : public Product {
public:
    void show() const override {
        println("🖱️  Mouse: {}", getBrand());
    }
    virtual std::string getBrand() const = 0;
};

// ─────────────────────────────────────────── 具体产品族：罗技（Logitech） ───────────────────────────────────────────

class LogiKeyboard : public Keyboard {
public:
    std::string getBrand() const override {
        return "Logitech Keyboard";
    }
};

class LogiMouse : public Mouse {
public:
    std::string getBrand() const override {
        return "Logitech Mouse";
    }
};

// ─────────────────────────────────────────── 具体产品族：雷蛇（Razer） ───────────────────────────────────────────

class RazerKeyboard : public Keyboard {
public:
    std::string getBrand() const override {
        return "Razer Keyboard";
    }
};

class RazerMouse : public Mouse {
public:
    std::string getBrand() const override {
        return "Razer Mouse";
    }
};

// ─────────────────────────────────────────── 抽象工厂 ───────────────────────────────────────────

/**
 * @brief 抽象工厂：声明创建产品族的接口
 */
class DeviceFactory {
public:
    virtual ~DeviceFactory() = default;

    virtual std::unique_ptr<Keyboard> createKeyboard() const = 0;
    virtual std::unique_ptr<Mouse> createMouse() const = 0;
};

// ─────────────────────────────────────────── 具体工厂 ───────────────────────────────────────────

/**
 * @brief 罗技工厂：创建罗技产品族
 */
class LogiFactory : public DeviceFactory {
public:
    std::unique_ptr<Keyboard> createKeyboard() const override {
        return std::make_unique<LogiKeyboard>();
    }

    std::unique_ptr<Mouse> createMouse() const override {
        return std::make_unique<LogiMouse>();
    }
};

/**
 * @brief 雷蛇工厂：创建雷蛇产品族
 */
class RazerFactory : public DeviceFactory {
public:
    std::unique_ptr<Keyboard> createKeyboard() const override {
        return std::make_unique<RazerKeyboard>();
    }

    std::unique_ptr<Mouse> createMouse() const override {
        return std::make_unique<RazerMouse>();
    }
};

// ─────────────────────────────────────────── 模板工厂（现代 C++ 增强） ───────────────────────────────────────────

/**
 * @brief 模板工厂：使用模板避免重复代码
 * @tparam KeyboardType 键盘具体类型
 * @tparam MouseType    鼠标具体类型
 */
template<typename KeyboardType, typename MouseType>
class SimpleDeviceFactory : public DeviceFactory {
public:
    std::unique_ptr<Keyboard> createKeyboard() const override {
        return std::make_unique<KeyboardType>();
    }

    std::unique_ptr<Mouse> createMouse() const override {
        return std::make_unique<MouseType>();
    }
};

// 类型别名简化使用
using ModernLogiFactory = SimpleDeviceFactory<LogiKeyboard, LogiMouse>;
using ModernRazerFactory = SimpleDeviceFactory<RazerKeyboard, RazerMouse>;

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 使用工厂创建并展示设备
 * @param factory 工厂实例
 */
void useFactory(const DeviceFactory &factory) {
    auto keyboard = factory.createKeyboard();
    auto mouse = factory.createMouse();
    keyboard->show();
    mouse->show();
}

/**
 * @brief 测试传统抽象工厂
 */
void testTraditional() {
    println("\n🧪 测试 1: 传统抽象工厂");

    LogiFactory logiFactory;
    RazerFactory razerFactory;

    println("📦 罗技产品族：");
    useFactory(logiFactory);

    println("📦 雷蛇产品族：");
    useFactory(razerFactory);
}

/**
 * @brief 测试模板工厂（现代 C++ 风格）
 */
void testModern() {
    println("\n🧪 测试 2: 模板工厂（CRTP 风格）");

    ModernLogiFactory logiFactory;
    ModernRazerFactory razerFactory;

    println("📦 罗技产品族（模板）：");
    useFactory(logiFactory);

    println("📦 雷蛇产品族（模板）：");
    useFactory(razerFactory);
}

/**
 * @brief 测试扩展性：新增“海盗船”产品族
 */
void testExtensibility() {
    println("\n🧪 测试 3: 扩展性测试（新增海盗船产品族）");

    // 新增产品
    class CorsairKeyboard : public Keyboard {
    public:
        std::string getBrand() const override {
            return "Corsair Keyboard";
        }
    };

    class CorsairMouse : public Mouse {
    public:
        std::string getBrand() const override {
            return "Corsair Mouse";
        }
    };

    // 新增工厂（只需一行）
    using CorsairFactory = SimpleDeviceFactory<CorsairKeyboard, CorsairMouse>;
    CorsairFactory corsairFactory;

    println("📦 海盗船产品族：");
    useFactory(corsairFactory);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 抽象工厂模式");

    testTraditional();
    testModern();
    testExtensibility();

    println("\n✅ 所有测试完成！");
    return 0;
}