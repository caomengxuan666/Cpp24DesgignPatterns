// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Command Pattern                                               ┃
// ┃  PURPOSE   ::  Encapsulate a request as an object                           ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    command.cpp
 * @brief   现代 C++20 实现命令模式
 * @details 
 *          命令模式（Command Pattern）是一种行为型设计模式，将请求封装为对象，
 *          从而使你可以用不同的请求、队列、日志来参数化其他对象。支持撤销、重做、延迟执行等。
 * 
 * @par 核心思想
 *      - 命令（Command）封装“做什么”和“怎么做”
 *      - 接收者（Receiver）执行具体操作
 *      - 调用者（Invoker）持有命令队列
 *      - 客户端决定命令的创建和绑定
 * 
 * @note    本实现支持撤销、重做、批处理
 * @warning 命令应尽量无状态，避免生命周期问题
 * @todo    支持命令序列化（保存到文件）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <vector>

using std::println;


// ─────────────────────────────────────────── 接收者：Cook ───────────────────────────────────────────

/**
 * @brief 厨师：执行具体操作
 */
class Cook {
public:
    void makeVegetable() {
        println("🥗 制作蔬菜沙拉");
    }

    void makeSteak() {
        println("🥩 制作牛排");
    }

    void undoVegetable() {
        println("🗑️  撤销蔬菜沙拉");
    }

    void undoSteak() {
        println("🗑️  撤销牛排");
    }
};

// ─────────────────────────────────────────── 抽象命令：Command ───────────────────────────────────────────

/**
 * @brief 抽象命令接口
 */
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// ─────────────────────────────────────────── 具体命令：MakeVegetableCommand, MakeSteakCommand ───────────────────────────────────────────

/**
 * @brief 制作蔬菜沙拉命令
 */
class MakeVegetableCommand : public Command {
    Cook &cook;

public:
    explicit MakeVegetableCommand(Cook &c) : cook(c) {}

    void execute() override {
        cook.makeVegetable();
    }

    void undo() override {
        cook.undoVegetable();
    }
};

/**
 * @brief 制作牛排命令
 */
class MakeSteakCommand : public Command {
    Cook &cook;

public:
    explicit MakeSteakCommand(Cook &c) : cook(c) {}

    void execute() override {
        cook.makeSteak();
    }

    void undo() override {
        cook.undoSteak();
    }
};

// ─────────────────────────────────────────── 调用者：Order（命令队列 + 撤销栈） ───────────────────────────────────────────

/**
 * @brief 订单：命令队列 + 撤销栈
 */
class Order {
private:
    std::vector<std::unique_ptr<Command>> commands; // 已执行命令队列
    std::vector<std::unique_ptr<Command>> undoStack;// 撤销栈

public:
    /**
     * @brief 添加并执行命令
     * @param cmd 命令对象（转移所有权）
     */
    void addAndExecute(std::unique_ptr<Command> cmd) {
        cmd->execute();
        commands.emplace_back(std::move(cmd));
        println("✅ 点了一道菜");
    }

    /**
     * @brief 撤销最后一次操作
     */
    void undoLast() {
        if (commands.empty()) {
            println("❌ 没有可撤销的操作");
            return;
        }

        auto cmd = std::move(commands.back());
        cmd->undo();
        undoStack.emplace_back(std::move(cmd));
        commands.pop_back();
        println("↩️  退了一道菜（已撤销）");
    }

    /**
     * @brief 重做最后一次撤销的操作
     */
    void redoLast() {
        if (undoStack.empty()) {
            println("❌ 没有可重做的操作");
            return;
        }

        auto cmd = std::move(undoStack.back());
        cmd->execute();
        commands.emplace_back(std::move(cmd));
        undoStack.pop_back();
        println("↪️  重做一道菜");
    }

    /**
     * @brief 批量执行所有命令（重放）
     */
    void notify() {
        println("📢 开始批量执行订单：");
        for (const auto &cmd: commands) {
            cmd->execute();
        }
    }

    /**
     * @brief 显示当前订单状态
     */
    void displayStatus() const {
        println("📋 当前订单状态：");
        println("   已执行: {} 道菜", commands.size());
        println("   可重做: {} 道菜", undoStack.size());
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本命令功能
 */
void testBasicCommand() {
    println("\n🧪 测试 1: 基本命令模式");

    Cook cook;
    Order order;

    order.addAndExecute(std::make_unique<MakeVegetableCommand>(cook));
    order.addAndExecute(std::make_unique<MakeSteakCommand>(cook));

    order.displayStatus();
}

/**
 * @brief 测试撤销与重做
 */
void testUndoRedo() {
    println("\n🧪 测试 2: 撤销与重做");

    Cook cook;
    Order order;

    order.addAndExecute(std::make_unique<MakeVegetableCommand>(cook));
    order.addAndExecute(std::make_unique<MakeSteakCommand>(cook));

    order.undoLast();// 撤销牛排
    order.undoLast();// 撤销蔬菜

    order.redoLast();// 重做蔬菜
    order.displayStatus();
}

/**
 * @brief 测试批量执行
 */
void testBatchExecution() {
    println("\n🧪 测试 3: 批量执行");

    Cook cook;
    Order order;

    order.addAndExecute(std::make_unique<MakeVegetableCommand>(cook));
    order.addAndExecute(std::make_unique<MakeSteakCommand>(cook));

    order.notify();// 重放
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 命令模式");

    testBasicCommand();
    testUndoRedo();
    testBatchExecution();

    println("\n✅ 所有测试完成！");
    return 0;
}