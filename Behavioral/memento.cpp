// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Memento Pattern                                               ┃
// ┃  PURPOSE   ::  Capture and restore object's previous state                   ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    memento.cpp
 * @brief   现代 C++20 实现备忘录模式
 * @details 
 *          备忘录模式（Memento Pattern）是一种行为型设计模式，允许在不暴露对象内部结构的情况下
 *          保存和恢复其先前状态。常用于撤销/重做、游戏存档、事务回滚等场景。
 * 
 * @par 核心思想
 *      - 原发器（Originator）创建备忘录（Memento）
 *      - 备忘录保存状态（通常是私有数据）
 *      - 管理者（Caretaker）存储备忘录，但不查看内容
 *      - 原发器可以从备忘录恢复状态
 * 
 * @note    本实现使用值语义 + 智能指针，避免内存泄漏
 * @warning 频繁创建快照会消耗大量内存
 * @todo    支持序列化（保存到文件）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>

using std::println;
// ─────────────────────────────────────────── 备忘录类（Memento） ───────────────────────────────────────────

/**
 * @brief 备忘录：保存 Chess 的状态
 * @note 私有构造，只有 Chess 能创建
 */
class Memento {
    std::string name;
    std::pair<int, int> pos;

    friend class Chess;
    Memento(std::string n, std::pair<int, int> p)
        : name(std::move(n)), pos(p) {}

public:
    // 禁止外部修改
    std::string getName() const { return name; }
    std::pair<int, int> getPos() const { return pos; }

    void show() const {
        println("{}:({},{})", name, pos.first, pos.second);
    }
};

// ─────────────────────────────────────────── 原发器：Chess ───────────────────────────────────────────

/**
 * @brief 棋子：原发器，可保存和恢复状态
 */
class Chess {
private:
    std::string name;
    std::pair<int, int> pos;

public:
    Chess(std::string n, std::pair<int, int> p)
        : name(std::move(n)), pos(p) {}

    void move(const std::string &n, std::pair<int, int> p) {
        name = n;
        pos = p;
        println("♟️  棋子移动到: {}({},{})", name, pos.first, pos.second);
    }

    /**
     * @brief 保存当前状态
     * @return std::unique_ptr<Memento>
     */
    std::unique_ptr<Memento> save() const {
        return std::unique_ptr<Memento>(new Memento(name, pos));
    }

    /**
     * @brief 从备忘录恢复状态
     * @param memento 备忘录
     */
    void restore(const Memento &memento) {
        name = memento.getName();
        pos = memento.getPos();
        println("↩️  恢复状态: {}({},{})", name, pos.first, pos.second);
    }

    void show() const {
        println("📍 当前状态: {}({},{})", name, pos.first, pos.second);
    }
};

// ─────────────────────────────────────────── 管理者：Caretaker（支持 undo/redo） ───────────────────────────────────────────

/**
 * @brief 管理者：存储备忘录历史
 * @note 支持撤销和重做
 */
class Caretaker {
private:
    std::vector<std::unique_ptr<Memento>> history;
    size_t current = 0;// 当前位置（支持重做）

public:
    /**
     * @brief 保存一个状态
     * @param memento 备忘录（转移所有权）
     */
    void add(std::unique_ptr<Memento> memento) {
        // 丢弃“重做”部分
        if (current < history.size()) {
            history.erase(history.begin() + current, history.end());
        }
        history.push_back(std::move(memento));
        current = history.size();
        println("💾 保存第 {} 步", current);
    }

    /**
     * @brief 撤销（undo）
     * @return Memento*，nullptr 表示无可撤销
     */
    const Memento *undo() {
        if (current == 0) {
            println("❌ 无可撤销操作");
            return nullptr;
        }
        --current;
        println("↩️  撤销到第 {} 步", current + 1);
        return history[current].get();
    }

    /**
     * @brief 重做（redo）
     * @return Memento*，nullptr 表示无可重做
     */
    const Memento *redo() {
        if (current >= history.size()) {
            println("❌ 无可重做操作");
            return nullptr;
        }
        auto *memento = history[current].get();
        ++current;
        println("↪️  重做到第 {} 步", current);
        return memento;
    }

    /**
     * @brief 显示所有历史
     */
    void showHistory() const {
        println("📜 操作历史:");
        for (size_t i = 0; i < history.size(); ++i) {
            println("  步骤 {}: ", i + 1);
            history[i]->show();
        }
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试备忘录模式
 */
void testMemento() {
    println("🎯 开始测试现代 C++20 备忘录模式");

    Chess chess("车", {4, 3});
    Caretaker caretaker;

    // 初始状态
    caretaker.add(chess.save());
    chess.show();

    // 移动 1
    chess.move("马", {1, 2});
    caretaker.add(chess.save());

    // 移动 2
    chess.move("兵", {5, 5});
    caretaker.add(chess.save());

    // 移动 3
    chess.move("炮", {3, 3});
    caretaker.add(chess.save());

    caretaker.showHistory();

    // 撤销两次
    if (const auto *m = caretaker.undo()) {
        chess.restore(*m);
    }
    if (const auto *m = caretaker.undo()) {
        chess.restore(*m);
    }

    // 重做一次
    if (const auto *m = caretaker.redo()) {
        chess.restore(*m);
    }

    chess.show();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    testMemento();
    println("\n✅ 所有测试完成！");
    return 0;
}