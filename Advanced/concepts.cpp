// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  C++20 Concepts                                                ┃
// ┃  PURPOSE   ::  Constrain templates with clear, reusable requirements        ┃
// ┃  CATEGORY  ::  Modern C++20 Feature                                         ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    concepts.cpp
 * @brief   C++20 Concepts 实战
 * @details 
 *          Concepts 是 C++20 引入的模板约束机制，允许你为模板参数定义清晰的语义要求。
 *          它解决了传统模板“SFINAE 地狱”的问题，让泛型代码更安全、更易读、更易维护。
 * 
 * @par 核心思想
 *      - 定义可重用的类型约束（Concept）
 *      - 在模板中使用 concept 约束参数
 *      - 编译器在模板声明时检查约束
 * 
 * @note    需要 C++20 编译器支持（MSVC 19.23+, GCC 10+, Clang 10+）
 * @warning 滥用 concept 会导致编译时间增加
 * @todo    结合 requires 表达式实现复杂约束
 * @date    2025-08-19
 * @version 1.0
 */

#include <concepts>
#include <format>
#include <iostream>
#include <list>
#include <print>
#include <string>
#include <vector>

using std::print, std::println;
// ─────────────────────────────────────────── 1️⃣ 基础概念：Arithmetic ───────────────────────────────────────────

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
T add(T a, T b) {
    return a + b;
}

// 重载：非 Arithmetic 类型
template<typename T>
    requires(!Arithmetic<T>)
std::string add(const T &a, const T &b) {
    return std::format("📎 {} + {}", a, b);
}

// ─────────────────────────────────────────── 2️⃣ 复合概念：Addable ───────────────────────────────────────────

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T combine(const std::vector<T> &vec) {
    T result = T{};
    for (const auto &item: vec) {
        result = result + item;
    }
    return result;
}

// ─────────────────────────────────────────── 3️⃣ 可迭代概念：Iterable ───────────────────────────────────────────

template<typename T>
concept Iterable = requires(T t) {
    t.begin();
    t.end();
} || std::ranges::range<T>;

template<Iterable T>
void print_range(const T &range) {
    println("📦 可迭代容器:");
    for (const auto &item: range) {
        print("{} ", item);
    }
    println("");
}

// ─────────────────────────────────────────── 4️⃣ 可打印概念：Printable ───────────────────────────────────────────
template<typename T>
concept Printable = requires(T t, std::ostream &os) {
    os << t;
};

template<Printable T>
void print_value(const T &value) {
    println("🖨️  打印值: {}", value);
}

// ─────────────────────────────────────────── 5️⃣ 策略式设计 + Concepts ───────────────────────────────────────────

// 创建策略
struct OpNewCreator {
    static void *create() {
        println("🔧 使用 new 创建");
        return new int(42);
    }
    static void destroy(void *p) {
        println("🧹 使用 delete 销毁");
        delete static_cast<int *>(p);
    }
};

// 验证策略
struct RangeValidator {
    static void validate(int value) {
        if (value < 0 || value > 100) {
            throw std::invalid_argument(std::format("值 {} 超出范围", value));
        }
        println("✅ 值 {} 有效", value);
    }
};

// 概念定义
template<typename T>
concept Creator = requires {
    { T::create() } -> std::same_as<void *>;
    { T::destroy(std::declval<void *>()) } -> std::same_as<void>;
};

template<typename T>
concept Validator = requires(int value) {
    { T::validate(value) } -> std::same_as<void>;
};

// 主类模板
template<Creator C, Validator V>
class Widget : public C, public V {
    int *data;

public:
    Widget() : data(nullptr) {
        data = static_cast<int *>(C::create());
        if (data) *data = 0;
    }

    void setValue(int value) {
        V::validate(value);
        if (data) *data = value;
    }

    int getValue() const {
        return data ? *data : -1;
    }

    ~Widget() {
        if (data) {
            C::destroy(data);
            data = nullptr;
        }
    }
};

// 类型别名
using MyWidget = Widget<OpNewCreator, RangeValidator>;

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testArithmetic() {
    println("\n🧪 1. Arithmetic 概念");
    println("✅ add(3, 5) = {}", add(3, 5));
    println("✅ add(3.14, 2.86) = {:.2f}", add(3.14, 2.86));
    println("✅ add(\"Hello\", \", World\") = {}", add(std::string("Hello"), std::string(", World")));
}

void testAddable() {
    println("\n🧪 2. Addable 概念");
    std::vector<int> ints = {1, 2, 3, 4, 5};
    std::vector<std::string> strs = {"A", "B", "C"};

    println("✅ 整数和: {}", combine(ints));
    println("✅ 字符串拼接: {}", combine(strs));
}

void testIterable() {
    println("\n🧪 3. Iterable 概念");
    std::vector<int> vec = {1, 2, 3};
    std::list<std::string> lst = {"X", "Y", "Z"};
    int arr[] = {4, 5, 6};

    print_range(vec);
    print_range(lst);
    print_range(arr);
}

void testPrintable() {
    println("\n🧪 4. Printable 概念");
    print_value(42);
    print_value(3.14159);
    print_value(std::string("Hello"));
    print_value('A');
}

void testPolicyWithConcepts() {
    println("\n🧪 5. 策略式设计 + Concepts");
    MyWidget w;
    w.setValue(50);
    println("值: {}", w.getValue());
    try {
        w.setValue(150);
    } catch (const std::exception &e) {
        println("❌ 异常: {}", e.what());
    }
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

int main() {
    println("🎯 开始测试 C++20 Concepts");

    testArithmetic();
    testAddable();
    testIterable();
    testPrintable();
    testPolicyWithConcepts();

    println("\n✅ 所有测试完成！");
    return 0;
}