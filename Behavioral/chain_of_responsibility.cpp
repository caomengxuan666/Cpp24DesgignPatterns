// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Chain of Responsibility Pattern                               ┃
// ┃  PURPOSE   ::  Pass request along a chain of handlers until one handles it  ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    chain_of_responsibility.cpp
 * @brief   现代 C++20 实现责任链模式
 * @details 
 *          责任链模式（Chain of Responsibility Pattern）是一种行为型设计模式，将请求沿处理者链传递，
 *          直到有一个处理者处理它为止。它常用于审批流程、事件处理、日志系统等场景。
 * 
 * @par 核心思想
 *      - 处理者（Handler）持有下一个处理者的引用
 *      - 每个处理者决定是否处理请求或转发
 *      - 客户端只需将请求发送给链头
 * 
 * @note    本实现使用智能指针 + 流式构建器，避免内存泄漏
 * @warning 链断裂时应有 fallback 机制
 * @todo    支持运行时动态插入/移除处理者
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>

using std::println;

// ─────────────────────────────────────────── 抽象处理者：Handler ───────────────────────────────────────────

/**
 * @brief 抽象处理者：定义处理请求的接口
 */
class Handler {
    friend class ChainBuilder;

protected:
    std::unique_ptr<Handler> next;// 下一个处理者（自动管理生命周期）

    /**
     * @brief 处理请求（模板方法）
     * @param days 请假天数
     */
    virtual void handleRequestImpl(int days) const = 0;

    /**
     * @brief 获取处理者名称（用于日志）
     * @return std::string
     */
    virtual std::string getName() const = 0;

public:
    virtual ~Handler() = default;

    /**
     * @brief 设置下一个处理者
     * @param handler 下一个处理者（转移所有权）
     * @return Handler& 支持链式调用
     */
    Handler &setNext(std::unique_ptr<Handler> handler) {
        next = std::move(handler);
        return *this;
    }

    /**
     * @brief 处理请求
     * @param days 请假天数
     */
    void handleRequest(int days) const {
        println("📨 请求进入: {}", getName());
        handleRequestImpl(days);
    }
};

// ─────────────────────────────────────────── 具体处理者：主管、经理、老板 ───────────────────────────────────────────

/**
 * @brief 主管处理者
 */
class Director : public Handler {
    void handleRequestImpl(int days) const override {
        if (days <= 1) {
            println("✅ 主管同意请假 {} 天", days);
        } else {
            println("⏭️  主管：请假太长，转交经理");
            if (next) {
                next->handleRequest(days);
            } else {
                println("❌ 无后续处理者，请求被拒绝");
            }
        }
    }

    std::string getName() const override {
        return "主管";
    }
};

/**
 * @brief 经理处理者
 */
class Manager : public Handler {
    void handleRequestImpl(int days) const override {
        if (days <= 3) {
            println("✅ 经理同意请假 {} 天", days);
        } else {
            println("⏭️  经理：请假太长，转交老板");
            if (next) {
                next->handleRequest(days);
            } else {
                println("❌ 无后续处理者，请求被拒绝");
            }
        }
    }

    std::string getName() const override {
        return "经理";
    }
};

/**
 * @brief 老板处理者
 */
class Boss : public Handler {
    void handleRequestImpl(int days) const override {
        if (days <= 7) {
            println("✅ 老板同意请假 {} 天", days);
        } else {
            println("❌ 老板：请假太长，不予批准");
        }
    }

    std::string getName() const override {
        return "老板";
    }
};

// ─────────────────────────────────────────── 流式构建器（Fluent Builder） ───────────────────────────────────────────

/**
 * @brief 责任链构建器
 * @note 支持链式调用创建处理链
 */
class ChainBuilder {
    std::unique_ptr<Handler> head;
    Handler *tail = nullptr;

public:
    /**
     * @brief 添加处理者到链尾
     * @param handler 处理者（转移所有权）
     * @return ChainBuilder& 支持链式调用
     */
    ChainBuilder &add(std::unique_ptr<Handler> handler) {
        if (!head) {
            head = std::move(handler);
            tail = head.get();
        } else {
            tail->setNext(std::move(handler));
            // 更新 tail（需要遍历到最后一个）
            Handler *current = head.get();
            while (current->next) {
                current = current->next.get();
            }
            tail = current;
        }
        return *this;
    }

    /**
     * @brief 构建完成
     * @return std::unique_ptr<Handler> 链头
     */
    std::unique_ptr<Handler> build() {
        return std::move(head);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本责任链
 */
void testBasicChain() {
    println("\n🧪 测试 1: 基本责任链模式");

    auto director = std::make_unique<Director>();
    auto manager = std::make_unique<Manager>();
    auto boss = std::make_unique<Boss>();

    manager->setNext(std::move(boss));    // manager → boss
    director->setNext(std::move(manager));// director → manager

    println("📅 请假 1 天:");
    director->handleRequest(1);

    println("\n📅 请假 3 天:");
    director->handleRequest(3);

    println("\n📅 请假 10 天:");
    director->handleRequest(10);
}

/**
 * @brief 测试流式构建器
 */
void testFluentBuilder() {
    println("\n🧪 测试 2: 流式构建器");

    auto chain = ChainBuilder{}
                         .add(std::make_unique<Director>())
                         .add(std::make_unique<Manager>())
                         .add(std::make_unique<Boss>())
                         .build();

    println("📅 请假 2 天:");
    chain->handleRequest(2);
}

/**
 * @brief 测试链断裂情况
 */
void testBrokenChain() {
    println("\n🧪 测试 3: 链断裂处理");

    auto director = std::make_unique<Director>();// 没有设置 next

    println("📅 请假 5 天（无后续处理者）:");
    director->handleRequest(5);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 责任链模式");

    testBasicChain();
    testFluentBuilder();
    testBrokenChain();

    println("\n✅ 所有测试完成！");
    return 0;
}