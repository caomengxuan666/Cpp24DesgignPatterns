// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Three-way Comparison Operator (<=>)                           ┃
// ┃  PURPOSE   ::  Auto-generate comparison operators in C++20                  ┃
// ┃  CATEGORY  ::  Modern C++20 Idiom                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    three_way_comparison.cpp
 * @brief   C++20 三路比较运算符（<=>）实战
 * @details 
 *          C++20 引入了 <=> 运算符（太空船运算符），可以自动生成 ==, !=, <, <=, >, >=。
 *          它简化了类型比较逻辑，是现代 C++ 的核心特性之一。
 * 
 * @par 核心思想
 *      - 使用 auto operator<=>(const T&) const = default
 *      - 编译器自动生成所有比较操作
 *      - 支持强类型安全比较
 * 
 * @note    需要 C++20 编译器支持（MSVC 19.20+, GCC 10+, Clang 10+）
 * @warning 不适用于需要自定义比较逻辑的复杂类型
 * @todo    结合 Concepts 约束可比较类型
 * @date    2025-08-19
 * @version 1.0
 */

#include <compare>// 三路比较所需头文件
#include <format>
#include <iostream>
#include <print>
#include <string>

using std::println;

// ─────────────────────────────────────────── 1️⃣ 基本使用：默认生成 ───────────────────────────────────────────

struct Point {
    int x, y;

    // ✅ 自动生成所有比较操作
    auto operator<=>(const Point &) const = default;
};

// ─────────────────────────────────────────── 2️⃣ 自定义比较逻辑 ───────────────────────────────────────────

struct Version {
    int major, minor, patch;

    // 自定义比较逻辑
    auto operator<=>(const Version &other) const {
        if (auto cmp = major <=> other.major; cmp != 0) return cmp;
        if (auto cmp = minor <=> other.minor; cmp != 0) return cmp;
        return patch <=> other.patch;
    }

    // 仍需手动实现 ==（C++20 中 <=> 不自动推导 ==，但可以优化）
    bool operator==(const Version &other) const = default;
};

// ─────────────────────────────────────────── 3️⃣ 部分自动生成（混合模式） ───────────────────────────────────────────

struct Name {
    std::string first, last;

    // 自动生成 == 和 !=
    bool operator==(const Name &) const = default;

    // 自动生成 <, <=, >, >=
    auto operator<=>(const Name &) const = default;
};

// ─────────────────────────────────────────── 4️⃣ 强类型比较（返回类型） ───────────────────────────────────────────

enum class Priority {
    Low,
    Medium,
    High
};

struct Task {
    std::string name;
    Priority priority;

    // 返回强类型 std::strong_ordering
    auto operator<=>(const Task &other) const {
        // 优先按 priority 比较
        if (auto cmp = priority <=> other.priority; cmp != 0) return cmp;
        // 再按 name 比较
        return name <=> other.name;
    }
    
    // 默认实现
    bool operator==(const Task &other) const = default;
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testBasicComparison() {
    println("\n🧪 1. 基本比较（Point）");

    Point p1{1, 2}, p2{1, 3}, p3{1, 2};

    println("p1 < p2: {}", p1 < p2);
    println("p1 == p3: {}", p1 == p3);
    println("p1 != p2: {}", p1 != p2);
}

void testCustomLogic() {
    println("\n🧪 2. 自定义比较（Version）");

    Version v1{1, 2, 3}, v2{1, 3, 0}, v3{1, 2, 3};

    println("v1 < v2: {}", v1 < v2);
    println("v1 == v3: {}", v1 == v3);
    println("v2 > v1: {}", v2 > v1);
}

void testMixedMode() {
    println("\n🧪 3. 混合模式（Name）");

    Name n1{"Alice", "Smith"}, n2{"Bob", "Smith"}, n3{"Alice", "Smith"};

    println("n1 == n3: {}", n1 == n3);
    println("n1 < n2: {}", n1 < n2);
}

void testStrongOrdering() {
    println("\n🧪 4. 强类型比较（Task）");

    Task t1{"Task A", Priority::High};
    Task t2{"Task B", Priority::Medium};
    Task t3{"Task C", Priority::High};

    println("t1 > t2: {}", t1 > t2);
    println("t1 == t3: {}", t1 == t3);
    println("t1 < t3: {}", t1 < t3);// 同优先级，按名字比较
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

int main() {
    println("🎯 开始测试 C++20 三路比较运算符");

    testBasicComparison();
    testCustomLogic();
    testMixedMode();
    testStrongOrdering();

    println("\n✅ 所有测试完成！");
    return 0;
}