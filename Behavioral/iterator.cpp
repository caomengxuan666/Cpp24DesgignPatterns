// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Iterator Pattern                                              ┃
// ┃  PURPOSE   ::  Provide a way to access elements of a collection sequentially ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    iterator.cpp
 * @brief   现代 C++20 实现迭代器模式
 * @details 
 *          迭代器模式（Iterator Pattern）是一种行为型设计模式，让你能在不暴露集合内部结构的情况下遍历其元素。
 *          在现代 C++ 中，我们通过实现符合 STL 规范的迭代器，支持范围 for 循环和算法库。
 * 
 * @par 核心思想
 *      - 自定义容器提供 `begin()` 和 `end()`
 *      - 迭代器封装底层遍历逻辑
 *      - 客户端使用标准语法遍历
 * 
 * @note    本实现支持范围 for、算法、C++20 Ranges
 * @warning 迭代器失效问题仍需注意
 * @todo    支持反向迭代器
 * @date    2025-08-19
 * @version 1.0
 */

#include <algorithm>
#include <print>
#include <vector>


using std::println, std::print;

// ─────────────────────────────────────────── 数据类：Data ───────────────────────────────────────────

/**
 * @brief 示例数据类
 */
class Data {
    int m_data;

public:
    explicit Data(int data = 0) : m_data(data) {}
    int getData() const { return m_data; }
    void setData(int data) { m_data = data; }
};

// ─────────────────────────────────────────── 自定义容器：Container<T> ───────────────────────────────────────────

/**
 * @brief 可迭代容器模板
 * @tparam T 存储的元素类型
 */
template<typename T>
class Container {
private:
    std::vector<T> data;

public:
    // 添加元素
    void add(const T &item) {
        data.push_back(item);
    }

    void add(T &&item) {
        data.push_back(std::move(item));
    }

    // 获取大小
    size_t size() const { return data.size(); }

    // --- 迭代器支持 begin/end ---

    // 普通迭代器
    auto begin() -> typename std::vector<T>::iterator {
        return data.begin();
    }

    auto end() -> typename std::vector<T>::iterator {
        return data.end();
    }

    // const 迭代器
    auto begin() const -> typename std::vector<T>::const_iterator {
        return data.begin();
    }

    auto end() const -> typename std::vector<T>::const_iterator {
        return data.end();
    }

    // C++11 起支持的 `cbegin`/`cend`
    auto cbegin() const -> typename std::vector<T>::const_iterator {
        return data.cbegin();
    }

    auto cend() const -> typename std::vector<T>::const_iterator {
        return data.cend();
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试 int 容器
 */
void testIntContainer() {
    println("\n🧪 测试 1: int 容器 + 范围 for");

    Container<int> container;
    for (int i = 0; i < 10; ++i) {
        container.add(i);
    }

    // ✅ 支持范围 for
    for (const auto &value: container) {
        print("{} ", value);
    }
    println("");
}

/**
 * @brief 测试 Data 容器
 */
void testDataContainer() {
    println("\n🧪 测试 2: Data 容器 + 算法");

    Container<Data> container;
    container.add(Data(168));
    container.add(Data(328));
    container.add(Data(648));

    // ✅ 支持 STL 算法
    std::for_each(container.begin(), container.end(), [](const Data &d) {
        print("{} ", d.getData());
    });
    println("");
}

/**
 * @brief 测试 C++20 Ranges（概念检查）
 */
void testRanges() {
    println("\n🧪 测试 3: C++20 Ranges 概念检查");

    Container<int> container;
    for (int i = 0; i < 5; ++i) container.add(i);

    if constexpr (std::ranges::range<decltype(container)>) {
        println("✅ 容器满足 std::ranges::range 概念");
    } else {
        println("❌ 容器不满足 range 概念");
    }

    // 使用 ranges 算法
    std::ranges::for_each(container, [](int x) {
        print("[{}] ", x);
    });
    println("");
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 迭代器模式");

    testIntContainer();
    testDataContainer();
    testRanges();

    println("\n✅ 所有测试完成！");
    return 0;
}