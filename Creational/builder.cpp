// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Builder Pattern                                               ┃
// ┃  PURPOSE   ::  Separate construction of complex objects from representation ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    builder.cpp
 * @brief   现代 C++20 实现建造者模式
 * @details 
 *          建造者模式（Builder Pattern）是一种创建型设计模式，将复杂对象的构建过程与其表示分离，
 *          使得同样的构建过程可以创建不同的表示。适用于配置复杂、步骤固定但内容可变的对象创建。
 * 
 * @par 核心思想
 *      - 将对象的构造过程分解为多个步骤
 *      - 客户端通过 Director 或流式接口控制构建过程
 *      - 最终获得完整对象
 * 
 * @note    本实现提供两种风格：
 *          1. 传统 GoF 风格（Builder + Director）
 *          2. 现代流式风格（Fluent Builder）其实也就是链式
 * @warning 建造者不应暴露中间状态
 * @todo    支持构建验证（如必须设置主机）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>
#include <vector>

using std::println;

// ─────────────────────────────────────────── 组件类型定义 ───────────────────────────────────────────

enum class DisplayBrand { Samsung,
                          Dell,
                          LG };
enum class MouseBrand { Logitech,
                        Razer,
                        Microsoft };
enum class KeyboardBrand { Astro,
                           Keychron,
                           Filco };
enum class HostBrand { Legion,
                       MacPro,
                       DellXPS };

std::string toString(DisplayBrand b) {
    switch (b) {
        case DisplayBrand::Samsung:
            return "三星显示器";
        case DisplayBrand::Dell:
            return "戴尔显示器";
        case DisplayBrand::LG:
            return "LG显示器";
    }
    return "未知显示器";
}

std::string toString(MouseBrand b) {
    switch (b) {
        case MouseBrand::Logitech:
            return "罗技鼠标";
        case MouseBrand::Razer:
            return "雷蛇鼠标";
        case MouseBrand::Microsoft:
            return "微软鼠标";
    }
    return "未知鼠标";
}

std::string toString(KeyboardBrand b) {
    switch (b) {
        case KeyboardBrand::Astro:
            return "艾石头键盘";
        case KeyboardBrand::Keychron:
            return "京造键盘";
        case KeyboardBrand::Filco:
            return "斐尔可键盘";
    }
    return "未知键盘";
}

std::string toString(HostBrand b) {
    switch (b) {
        case HostBrand::Legion:
            return "拯救者主机";
        case HostBrand::MacPro:
            return "Mac Pro";
        case HostBrand::DellXPS:
            return "戴尔XPS主机";
    }
    return "未知主机";
}

// ─────────────────────────────────────────── 产品类：Computer ───────────────────────────────────────────

/**
 * @brief 电脑产品类
 */
class Computer {
private:
    std::vector<std::string> parts;

public:
    void addPart(std::string part) {
        parts.emplace_back(std::move(part));
    }

    void show() const {
        println("🖥️  当前电脑配置：");
        for (const auto &part: parts) {
            println("   → {}", part);
        }
    }

    // 支持移动语义
    Computer() = default;
    Computer(const Computer &) = default;
    Computer &operator=(const Computer &) = default;
    Computer(Computer &&) = default;
    Computer &operator=(Computer &&) = default;
};

// ─────────────────────────────────────────── 传统 GoF 风格：Builder + Director ───────────────────────────────────────────

/**
 * @brief 抽象建造者
 */
class Builder {
public:
    virtual ~Builder() = default;
    virtual void buildDisplay(DisplayBrand brand) = 0;
    virtual void buildMouse(MouseBrand brand) = 0;
    virtual void buildKeyboard(KeyboardBrand brand) = 0;
    virtual void buildHost(HostBrand brand) = 0;
    virtual std::unique_ptr<Computer> getComputer() = 0;
};

/**
 * @brief 具体建造者
 */

class ConcreteBuilder : public Builder {
    std::unique_ptr<Computer> computer = std::make_unique<Computer>();

public:
    void buildDisplay(DisplayBrand brand) override {
        computer->addPart(toString(brand));
    }

    void buildMouse(MouseBrand brand) override {
        computer->addPart(toString(brand));
    }

    void buildKeyboard(KeyboardBrand brand) override {
        computer->addPart(toString(brand));
    }

    void buildHost(HostBrand brand) override {
        computer->addPart(toString(brand));
    }

    std::unique_ptr<Computer> getComputer() override {
        return std::move(computer);
    }
};

/**
 * @brief 指挥者：控制构建过程
 */
class Director {
    Builder *builder;

public:
    explicit Director(Builder *b) : builder(b) {}

    std::unique_ptr<Computer> constructGamingPC() {
        builder->buildDisplay(DisplayBrand::Samsung);
        builder->buildMouse(MouseBrand::Razer);
        builder->buildKeyboard(KeyboardBrand::Astro);
        builder->buildHost(HostBrand::Legion);
        return builder->getComputer();
    }

    std::unique_ptr<Computer> constructOfficePC() {
        builder->buildDisplay(DisplayBrand::Dell);
        builder->buildMouse(MouseBrand::Microsoft);
        builder->buildKeyboard(KeyboardBrand::Keychron);
        builder->buildHost(HostBrand::DellXPS);
        return builder->getComputer();
    }
};

// ─────────────────────────────────────────── 现代流式风格：Fluent Builder ───────────────────────────────────────────

/**
 * @brief 流式建造者（现代 C++ 推荐）这样就不需要一个指挥者类了
 * @note 方法链式调用，返回 *this
 */
class ComputerBuilder {
    std::unique_ptr<Computer> computer = std::make_unique<Computer>();

public:
    ComputerBuilder &display(DisplayBrand brand) {
        computer->addPart(toString(brand));
        return *this;
    }

    ComputerBuilder &mouse(MouseBrand brand) {
        computer->addPart(toString(brand));
        return *this;
    }

    ComputerBuilder &keyboard(KeyboardBrand brand) {
        computer->addPart(toString(brand));
        return *this;
    }

    ComputerBuilder &host(HostBrand brand) {
        computer->addPart(toString(brand));
        return *this;
    }

    /**
     * @brief 构建完成，返回电脑对象
     * @return std::unique_ptr<Computer>
     */
    std::unique_ptr<Computer> build() {
        return std::move(computer);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试传统建造者模式
 */
void testTraditional() {
    println("\n🧪 测试 1: 传统建造者模式（GoF 风格）");

    ConcreteBuilder builder;
    Director director(&builder);

    auto gamingPC = director.constructGamingPC();
    gamingPC->show();

    auto officePC = director.constructOfficePC();
    officePC->show();
}

/**
 * @brief 测试现代流式建造者
 */
void testFluent() {
    println("\n🧪 测试 2: 流式建造者（Fluent Interface）");

    auto pc = ComputerBuilder{}
                      .display(DisplayBrand::Samsung)
                      .mouse(MouseBrand::Logitech)
                      .keyboard(KeyboardBrand::Astro)
                      .host(HostBrand::Legion)
                      .build();

    pc->show();
}

/**
 * @brief 测试构建过程复用
 */
void testReuse() {
    println("\n🧪 测试 3: 构建过程复用");

    // 定义一个通用构建函数
    auto buildCustomPC = [](DisplayBrand d, MouseBrand m, KeyboardBrand k, HostBrand h) {
        return ComputerBuilder{}
                .display(d)
                .mouse(m)
                .keyboard(k)
                .host(h)
                .build();
    };

    auto pc1 = buildCustomPC(DisplayBrand::LG, MouseBrand::Razer, KeyboardBrand::Filco, HostBrand::MacPro);
    pc1->show();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 建造者模式");

    testTraditional();
    testFluent();
    testReuse();

    println("\n✅ 所有测试完成！");
    return 0;
}