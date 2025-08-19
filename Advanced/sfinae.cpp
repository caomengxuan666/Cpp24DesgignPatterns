// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  SFINAE (4 Implementations)                                    ┃
// ┃  PURPOSE   ::  Enable function overloads based on type traits               ┃
// ┃  CATEGORY  ::  Template Metaprogramming                                     ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    sfinae.cpp
 * @brief   SFINAE 惯用法的 4 种现代实现
 * @details 
 *          SFINAE（替换失败不是错误）是 C++ 模板系统的核心机制，允许根据类型特征
 *          条件性地启用或禁用函数模板。它是 type traits、库泛型设计的基础。
 * 
 * @par 4 种实现方式
 *      1. enable_if + decltype：经典 SFINAE
 *      2. void_t 技巧：检测成员是否存在
 *      3. if constexpr：C++17 编译时分支
 *      4. Concepts：C++20 类型约束
 * 
 * @note    SFINAE 是现代 C++ 泛型编程的基石
 * @warning 过度使用会导致编译错误难以理解
 * @todo    结合 Concepts 实现更安全的约束
 * @date    2025-08-19
 * @version 1.0
 */

#include <format>
#include <list>
#include <print>
#include <string>
#include <type_traits>
#include <vector>

using std::println, std::print;


// ─────────────────────────────────────────── 工具：void_t (C++17) ───────────────────────────────────────────

template<typename...>
using void_t = void;

// ─────────────────────────────────────────── 1️⃣ enable_if + decltype ───────────────────────────────────────────

/**
 * @brief 仅当 T 有 .size() 方法时启用
 */
template<typename T>
auto print_size(const T &container) -> std::enable_if_t<
        std::is_same_v<decltype(container.size()), std::size_t>,
        void> {
    println("📏 容器大小: {}", container.size());
}

// 备用版本（当没有 size() 时）

void print_size(...) {
    println("❌ 该类型没有 size() 方法");
}

// ─────────────────────────────────────────── 2️⃣ void_t 技巧：检测成员 ───────────────────────────────────────────

// 检测是否有 .begin() 和 .end()
template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, void_t<
                              decltype(std::declval<T>().begin()),
                              decltype(std::declval<T>().end())>> : std::true_type {};

// 便捷别名
template<typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

// 根据是否可迭代选择函数
template<typename T>
std::enable_if_t<is_iterable_v<T>>
print_container(const T &c) {
    println("📦 可迭代容器:");
    for (const auto &item: c) {
        print("{} ", item);
    }
    println("");
}

template<typename T>
std::enable_if_t<!is_iterable_v<T>>
print_container(const T &c) {
    println("📦 不可迭代: {}", c);
}

// ─────────────────────────────────────────── 3️⃣ if constexpr 替代（C++17） ───────────────────────────────────────────

/**
 * @brief 使用 if constexpr 实现更清晰的条件逻辑
 */
template<typename T>
void print_type_info(const T &value) {
    if constexpr (std::is_integral_v<T>) {
        println("🔢 整型: {}", value);
    } else if constexpr (std::is_floating_point_v<T>) {
        println("🪄 浮点型: {:.2f}", value);
    } else if constexpr (is_iterable_v<T>) {
        println("🧩 可迭代类型，大小: {}", value.size());
    } else {
        println("📦 其他类型");
    }
}

// ─────────────────────────────────────────── 4️⃣ Concepts 替代（C++20 模拟） ───────────────────────────────────────────

// 模拟 Concepts（C++20 风格）
#ifdef __cpp_concepts
#define USE_CONCEPTS 1
#else
#define USE_CONCEPTS 0
#endif

#if USE_CONCEPTS
#include <concepts>

void print_integral(std::integral auto value) {
    println("✅ Concept: 整型 = {}", value);
}

void print_floating(std::floating_point auto value) {
    println("✅ Concept: 浮点 = {:.2f}", value);
}

#else
// 降级为 enable_if
template<typename T>
std::enable_if_t<std::is_integral_v<T>>
print_integral(T value) {
    println("✅ enable_if: 整型 = {}", value);
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>>
print_floating(T value) {
    println("✅ enable_if: 浮点 = {:.2f}", value);
}
#endif

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testEnableIf() {
    println("\n🧪 1. enable_if + decltype");

    std::vector<int> vec = {1, 2, 3};
    std::string str = "Hello";
    int x = 42;

    print_size(vec);// 有 size()
    print_size(str);// 有 size()
    print_size(x);  // 没有 size()
}

void testVoidT() {
    println("\n🧪 2. void_t 技巧");

    std::vector<int> vec = {1, 2, 3};
    std::list<double> lst = {1.1, 2.2};
    std::string str = "ABC";
    int n = 100;

    print_container(vec);
    print_container(lst);
    print_container(str);
    print_container(n);
}

void testIfConstexpr() {
    println("\n🧪 3. if constexpr");

    print_type_info(42);
    print_type_info(3.14);
    print_type_info(std::string("test"));
    print_type_info('A');
}

void testConcepts() {
    println("\n🧪 4. Concepts (or enable_if fallback)");

    print_integral(123);
    print_floating(3.14159);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

int main() {
    println("🎯 开始测试 SFINAE 的 4 种实现");

    testEnableIf();
    testVoidT();
    testIfConstexpr();
    testConcepts();

    println("\n✅ 所有测试完成！");
    return 0;
}