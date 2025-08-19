// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  C++20 Coroutines                                              ┃
// ┃  PURPOSE   ::  Write asynchronous code in synchronous style                  ┃
// ┃  CATEGORY  ::  Concurrency / Modern C++                                     ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    coroutines.cpp
 * @brief   C++20 协程实战
 * @details 
 *          协程（Coroutines）是 C++20 引入的核心特性，允许函数在执行过程中暂停和恢复，
 *          适用于生成器、异步 I/O、游戏逻辑、事件驱动系统等场景。
 * 
 * @par 核心思想
 *      - co_yield：暂停并返回值
 *      - co_await：等待异步操作
 *      - co_return：返回最终结果
 * 
 * @note    需要 C++20 编译器支持（MSVC 19.29+, Clang 14+, GCC 11+）
 * @warning 协程状态分配在堆上（除非优化）
 * @todo    结合 awaitable 网络操作
 * @date    2025-08-19
 * @version 1.0
 */

#include <coroutine>
#include <print>

using std::println;
// ─────────────────────────────────────────── 1️⃣ 生成器（Generator） ───────────────────────────────────────────

template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value;
        bool finished = false;

        Generator get_return_object() {
            return Generator{handle_type::from_promise(*this)};
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() { finished = true; }

        std::suspend_always yield_value(T v) {
            value = v;
            return {};
        }

        void unhandled_exception() { std::terminate(); }
    };

    handle_type coro;

    explicit Generator(handle_type h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }

    // 禁止拷贝
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    // 支持移动
    Generator(Generator&& other) noexcept : coro(other.coro) {
        other.coro = nullptr;
    }

    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (coro) coro.destroy();
            coro = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }

    // 迭代器接口
    class Iterator {
        handle_type handle;
    public:
        explicit Iterator(handle_type h) : handle(h) {}

        bool operator!=(std::default_sentinel_t) const {
            return !handle.done();
        }

        void operator++() {
            handle.resume();
        }

        T operator*() const {
            return handle.promise().value;
        }
    };

    Iterator begin() {
        if (coro) coro.resume();  // 启动协程
        if (!coro.done()) return Iterator{coro};
        return Iterator{nullptr};
    }

    std::default_sentinel_t end() {
        return std::default_sentinel;
    }
};

// ─────────────────────────────────────────── 2️⃣ Fibonacci 生成器 ───────────────────────────────────────────

Generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        std::swap(a, b);
        b += a;
    }
}

// ─────────────────────────────────────────── 3️⃣ 范围生成器（Range） ───────────────────────────────────────────

Generator<int> range(int start, int end, int step = 1) {
    for (int i = start; i < end; i += step) {
        co_yield i;
    }
}

// ─────────────────────────────────────────── 4️⃣ 树遍历生成器（中序遍历） ───────────────────────────────────────────

struct TreeNode {
    int value;
    std::unique_ptr<TreeNode> left, right;

    TreeNode(int v) : value(v) {}
};

Generator<int> inorder(const TreeNode* node) {
    if (node) {
        if (node->left) {
            for (auto val : inorder(node->left.get())) {
                co_yield val;
            }
        }
        co_yield node->value;
        if (node->right) {
            for (auto val : inorder(node->right.get())) {
                co_yield val;
            }
        }
    }
}

// ─────────────────────────────────────────── 5️⃣ 异步任务（Task） ───────────────────────────────────────────

// 简单 Task（不支持 co_await，仅 co_return）
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

Task async_task() {
    println("🔧 开始异步任务...");
    // 模拟异步操作
    co_return;
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testFibonacci() {
    println("\n🧪 1. Fibonacci 生成器");
    int count = 0;
    for (auto n : fibonacci()) {
        println("{}", n);
        if (++count >= 10) break;
    }
}

void testRange() {
    println("\n🧪 2. 范围生成器");
    for (auto i : range(1, 6)) {
        println("{}", i);
    }
}

void testTreeTraversal() {
    println("\n🧪 3. 树遍历生成器");

    auto root = std::make_unique<TreeNode>(4);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(6);
    root->left->left = std::make_unique<TreeNode>(1);
    root->left->right = std::make_unique<TreeNode>(3);
    root->right->left = std::make_unique<TreeNode>(5);
    root->right->right = std::make_unique<TreeNode>(7);

    for (auto val : inorder(root.get())) {
        println("{}", val);
    }
}

void testAsyncTask() {
    println("\n🧪 4. 异步任务");
    async_task();
    println("✅ 异步任务完成");
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

int main() {
    println("🎯 开始测试 C++20 协程");

    testFibonacci();
    testRange();
    testTreeTraversal();
    testAsyncTask();

    println("\n✅ 所有测试完成！");
    return 0;
}