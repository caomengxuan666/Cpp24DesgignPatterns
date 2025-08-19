// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Observer Pattern                                              ┃
// ┃  PURPOSE   ::  Notify observers when subject state changes                   ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    observer.cpp
 * @brief   现代 C++20 实现观察者模式
 * @details 
 *          观察者模式（Observer Pattern）是一种行为型设计模式，定义对象间的一对多依赖关系，
 *          当一个对象状态改变时，所有依赖它的对象都会自动收到通知。常用于事件系统、GUI、发布-订阅。
 * 
 * @par 核心思想
 *      - 被观察者（Subject）维护观察者列表
 *      - 观察者（Observer）注册监听
 *      - 状态变化时自动通知
 * 
 * @note    本实现使用 std::function + RAII 自动注销
 * @warning 避免循环引用（shared_ptr）
 * @todo    支持异步通知
 * @date    2025-08-19
 * @version 1.0
 */

#include <functional>
#include <memory>
#include <print>
#include <string>
#include <vector>

using std::println;
// ─────────────────────────────────────────── 事件总线：EventBus（发布-订阅） ───────────────────────────────────────────

/**
 * @brief 事件总线：支持类型安全的发布-订阅
 * @tparam T 事件类型
 */
template<typename T>
class EventBus {
private:
    // 使用 weak_ptr 避免悬空指针
    mutable std::vector<std::weak_ptr<std::function<void(const T &)>>> listeners;

public:
    /**
     * @brief 订阅事件
     * @param callback 回调函数
     * @return std::shared_ptr<void> 用于自动注销（RAII）
     */
    std::shared_ptr<void> subscribe(std::function<void(const T &)> callback) {
        auto shared_callback = std::make_shared<std::function<void(const T &)>>(std::move(callback));
        listeners.emplace_back(std::weak_ptr<std::function<void(const T &)>>(shared_callback));

        // 返回一个“句柄”，当它被销毁时，自动从列表中清理
        return shared_callback;
    }

    /**
     * @brief 发布事件
     * @param event 事件对象
     */
    void publish(const T &event) const {
        println("📢 发布事件: {}", event);

        // 清理已销毁的观察者
        listeners.erase(
                std::remove_if(listeners.begin(), listeners.end(),
                               [](const std::weak_ptr<std::function<void(const T &)>> &wp) {
                                   return wp.expired();
                               }),
                listeners.end());

        // 通知所有有效观察者
        for (const auto &weak_cb: listeners) {
            if (auto cb = weak_cb.lock()) {
                (*cb)(event);
            }
        }
    }
};

// ─────────────────────────────────────────── 被观察者：Boss（主题） ───────────────────────────────────────────

/**
 * @brief 老板：被观察者（主题）
 */
class Boss {
    std::string action;
    EventBus<std::string> eventBus;

public:
    /**
     * @brief 设置动作并通知观察者
     * @param act 动作描述
     */
    void setAction(const std::string &act) {
        action = act;
        eventBus.publish(action);
    }

    /**
     * @brief 获取当前状态
     * @return std::string
     */
    std::string getAction() const {
        return action;
    }

    /**
     * @brief 订阅事件
     * @param callback 回调函数
     * @return RAII 句柄，自动注销
     */
    std::shared_ptr<void> onActionChange(std::function<void(const std::string &)> callback) {
        return eventBus.subscribe(std::move(callback));
    }
};

// ─────────────────────────────────────────── 观察者：Staff（具体观察者） ───────────────────────────────────────────

/**
 * @brief 员工：观察者
 */
class Staff {
    std::string name;
    std::shared_ptr<void> subscription;// RAII 句柄

public:
    explicit Staff(std::string n) : name(std::move(n)) {}

    /**
     * @brief 开始监听老板
     * @param boss 老板对象
     */
    void startWatching(Boss &boss) {
        subscription = boss.onActionChange([this](const std::string &action) {
            println("{} 收到讯息: {}", name, action);
            if (action == "老板来了") {
                println("   💼 开启老板来了模式");
            } else if (action == "老板走了") {
                println("   🎮 原神，启动！");
            }
        });
        println("👀 {} 开始监听老板", name);
    }

    /**
     * @brief 停止监听（自动通过 RAII 实现）
     */
    void stopWatching() {
        subscription.reset();
        println("🛑 {} 停止监听老板", name);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本观察者功能
 */
void testBasicObserver() {
    println("\n🧪 测试 1: 基本观察者模式");

    Boss boss;
    Staff staff1("oi");
    Staff staff2("O.O");

    staff1.startWatching(boss);
    staff2.startWatching(boss);

    boss.setAction("老板来了");
    boss.setAction("老板走了");
}

/**
 * @brief 测试自动注销（RAII）
 */
void testAutoUnsubscribe() {
    println("\n🧪 测试 2: 自动注销（离开作用域自动停止监听）");

    Boss boss;

    {
        Staff tempStaff("临时员工");
        tempStaff.startWatching(boss);

        boss.setAction("老板来了");
    }// tempStaff 析构 → 自动注销

    println("临时员工已离开");
    boss.setAction("老板走了");// 不会通知临时员工
}

/**
 * @brief 测试手动注销
 */
void testManualUnsubscribe() {
    println("\n🧪 测试 3: 手动注销");

    Boss boss;
    Staff staff("主动离职员工");

    staff.startWatching(boss);
    boss.setAction("老板来了");

    staff.stopWatching();
    boss.setAction("老板走了");// 不会通知
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 观察者模式");

    testBasicObserver();
    testAutoUnsubscribe();
    testManualUnsubscribe();

    println("\n✅ 所有测试完成！");
    return 0;
}