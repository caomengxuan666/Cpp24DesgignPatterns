// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Named Constructor Idiom                                       ┃
// ┃  PURPOSE   ::  Provide meaningful names for constructor variants            ┃
// ┃  CATEGORY  ::  Implementation Pattern                                       ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    named_constructor.cpp

 * @brief   Named Constructor 惯用法实现
 * @details 
 *          当一个类有多种构造方式时，使用命名静态工厂函数代替多个构造函数，
 *          提高代码可读性和类型安全。常用于坐标、单位转换、配置初始化等场景。
 * 
 * @par 核心思想
 *      - 私有构造函数
 *      - 公共静态工厂函数提供语义化接口
 *      - 避免构造函数重载歧义
 * 
 * @note    本实现支持 Cartesian、Polar、Zero、UnitX/Y
 * @warning 构造函数必须私有，防止绕过工厂
 * @todo    支持 constexpr 和 C++20 Concepts 约束
 * @date    2025-08-19
 * @version 1.0
 */

#include <cmath>
#include <print>
#include <string>

constexpr double M_PI = 3.14159;

using std::println;
// ─────────────────────────────────────────── 2D 点类 ───────────────────────────────────────────

class Point {
    double x, y;

    // 私有构造函数，禁止直接构造
    explicit Point(double x, double y) : x(x), y(y) {}

public:
    // 🌟 命名构造函数：笛卡尔坐标
    static Point Cartesian(double x, double y) {
        return Point(x, y);
    }

    // 🌟 命名构造函数：极坐标（半径 + 角度）
    static Point Polar(double r, double theta) {
        return Point(r * cos(theta), r * sin(theta));
    }

    // 🌟 命名构造函数：零点
    static Point Zero() {
        return Point(0, 0);
    }

    // 🌟 命名构造函数：单位向量 X
    static Point UnitX() {
        return Point(1, 0);
    }

    // 🌟 命名构造函数：单位向量 Y
    static Point UnitY() {
        return Point(0, 1);
    }

    // 🌟 命名构造函数：中点（两个点的中心）
    static Point Midpoint(const Point &a, const Point &b) {
        return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    }

    // 🌟 命名构造函数：从字符串解析（工厂 + 解析）
    static Point FromString(const std::string &str) {
        // 简单解析 "x,y" 格式
        auto comma = str.find(',');
        if (comma == std::string::npos) {
            throw std::invalid_argument("Invalid format: expected 'x,y'");
        }
        double x = std::stod(str.substr(0, comma));
        double y = std::stod(str.substr(comma + 1));
        return Point(x, y);
    }

    // 公共接口
    double getX() const { return x; }
    double getY() const { return y; }
    double distance() const { return std::sqrt(x * x + y * y); }

    void print() const {
        println("📍 Point({:.2f}, {:.2f}) | 距离原点: {:.2f}", x, y, distance());
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testCartesianAndPolar() {
    println("\n🧪 测试 1: 笛卡尔 vs 极坐标");

    auto p1 = Point::Cartesian(3, 4);
    auto p2 = Point::Polar(5, M_PI / 4);// 45度

    p1.print();
    p2.print();
}

void testPredefinedPoints() {
    println("\n🧪 测试 2: 预定义点");

    auto origin = Point::Zero();
    auto x_axis = Point::UnitX();
    auto y_axis = Point::UnitY();

    origin.print();
    x_axis.print();
    y_axis.print();
}

void testMidpoint() {
    println("\n🧪 测试 3: 中点计算");

    auto a = Point::Cartesian(0, 0);
    auto b = Point::Cartesian(4, 6);
    auto mid = Point::Midpoint(a, b);

    a.print();
    b.print();
    mid.print();
}

void testFromString() {
    println("\n🧪 测试 4: 从字符串创建");

    try {
        auto p = Point::FromString("2.5,3.7");
        p.print();

        auto q = Point::FromString("10,-5");
        q.print();
    } catch (const std::exception &e) {
        println("❌ 解析失败: {}", e.what());
    }
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试 Named Constructor 惯用法");

    testCartesianAndPolar();
    testPredefinedPoints();
    testMidpoint();
    testFromString();

    println("\n✅ 所有测试完成！");
    return 0;
}