// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Bridge Pattern                                                ┃
// ┃  PURPOSE   ::  Decouple abstraction from implementation                      ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    bridge.cpp
 * @brief   现代 C++20 实现桥接模式
 * @details 
 *          桥接模式（Bridge Pattern）是一种结构型设计模式，将抽象与其实现分离，使两者可以独立变化。
 *          它通过“组合”代替“继承”，解决多维度扩展时类爆炸问题（如：形状 × 颜色 = N×M 个类）。
 * 
 * @par 核心思想
 *      - 抽象部分（Abstraction）包含对实现部分（Implementor）的引用
 *      - 实现部分（Implementor）提供接口，由具体实现类实现
 *      - 客户端只依赖抽象，不关心具体实现
 * 
 * @note    本实现使用 std::unique_ptr 避免内存泄漏
 * @warning 不要直接继承实现类，应通过组合注入
 * @todo    支持运行时切换实现（动态换肤）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>

using std::println;
// ─────────────────────────────────────────── 实现部分：颜色（Implementor） ───────────────────────────────────────────

/**
 * @brief 颜色接口：桥接模式的“实现部分”
 */
class Color {
public:
    virtual ~Color() = default;
    virtual void fillColor() const = 0;
};

/**
 * @brief 红色实现
 */
class Red : public Color {
public:
    void fillColor() const override {
        println("🎨 填充颜色: 红色");
    }
};

/**
 * @brief 蓝色实现
 */
class Blue : public Color {
public:
    void fillColor() const override {
        println("🎨 填充颜色: 蓝色");
    }
};

// ─────────────────────────────────────────── 抽象部分：形状（Abstraction） ───────────────────────────────────────────

/**
 * @brief 形状抽象类：桥接模式的“抽象部分”
 */
class Shape {
protected:
    std::unique_ptr<Color> color;// 组合：桥接的关键

    /**
     * @brief 构造函数（依赖注入）
     * @param c 颜色实现
     */
    explicit Shape(std::unique_ptr<Color> c) : color(std::move(c)) {}

    /**
     * @brief 绘制形状（具体由子类实现）
     */
    virtual void drawShape() const = 0;

public:
    /**
     * @brief 显示形状：高层逻辑
     * @note 这里的show是依赖于具体实现的
     */
    void show() const {
        drawShape();
        color->fillColor();
    }

    /**
     * @brief 设置颜色（支持运行时切换）
     * @param c 新颜色
     */
    void setColor(std::unique_ptr<Color> c) {
        color = std::move(c);
    }
};

/**
 * @brief 圆形
 */
class Circle : public Shape {
public:
    explicit Circle(std::unique_ptr<Color> c) : Shape(std::move(c)) {}

private:
    void drawShape() const override {
        println("🖌️  绘制圆形");
    }
};

/**
 * @brief 矩形
 */
class Rect : public Shape {
public:
    explicit Rect(std::unique_ptr<Color> c) : Shape(std::move(c)) {}

private:
    void drawShape() const override {
        println("🖌️  绘制矩形");
    }
};

// ─────────────────────────────────────────── 流式接口（现代 C++ 增强） ───────────────────────────────────────────

/**
 * @brief 支持流式设置的形状构建器
 */
template<typename ShapeType>
class ShapeBuilder {
    std::unique_ptr<Color> color;

public:
    ShapeBuilder &withColor(std::unique_ptr<Color> c) {
        color = std::move(c);
        return *this;
    }

    std::unique_ptr<ShapeType> build() {
        return std::make_unique<ShapeType>(std::move(color));
    }
};

// 类型别名简化使用
using CircleBuilder = ShapeBuilder<Circle>;
using RectBuilder = ShapeBuilder<Rect>;

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本桥接功能
 */
void testBasicBridge() {
    println("\n🧪 测试 1: 基本桥接模式");

    // 圆 + 红
    auto circle = std::make_unique<Circle>(std::make_unique<Red>());
    circle->show();

    // 圆 + 蓝
    circle->setColor(std::make_unique<Blue>());
    circle->show();

    // 矩形 + 红
    auto rect = std::make_unique<Rect>(std::make_unique<Red>());
    rect->show();
}

/**
 * @brief 测试流式构建
 */
void testFluentBuilder() {
    println("\n🧪 测试 2: 流式构建器");

    auto circle = CircleBuilder{}
                          .withColor(std::make_unique<Red>())
                          .build();

    auto rect = RectBuilder{}
                        .withColor(std::make_unique<Blue>())
                        .build();

    circle->show();
    rect->show();
}

/**
 * @brief 测试运行时切换实现
 */
void testRuntimeSwitch() {
    println("\n🧪 测试 3: 运行时切换颜色");

    auto shape = std::make_unique<Circle>(std::make_unique<Red>());
    shape->show();

    println("🔄 切换为蓝色...");
    shape->setColor(std::make_unique<Blue>());
    shape->show();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 桥接模式");

    testBasicBridge();
    testFluentBuilder();
    testRuntimeSwitch();

    println("\n✅ 所有测试完成！");
    return 0;
}