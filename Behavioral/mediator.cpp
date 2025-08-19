// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Mediator Pattern                                              ┃
// ┃  PURPOSE   ::  Reduce coupling between components via a central mediator    ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    mediator.cpp
 * @brief   现代 C++20 实现中介者模式
 * @details 
 *          中介者模式（Mediator Pattern）是一种行为型设计模式，用一个中介对象封装一系列对象之间的交互，
 *          使对象之间不需要显式地相互引用，从而降低耦合度。常用于 GUI、游戏对象通信、模块化系统。
 * 
 * @par 核心思想
 *      - 所有模块通过中介者通信
 *      - 中介者封装交互逻辑
 *      - 模块之间无直接依赖
 * 
 * @note    本实现使用事件总线 + 回调机制，避免 dynamic_cast
 * @warning 避免中介者变成“上帝对象”
 * @todo    支持消息过滤、优先级
 * @date    2025-08-19
 * @version 1.0
 */

#include <functional>
#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using std::println;
// ─────────────────────────────────────────── 消息类型定义 ───────────────────────────────────────────

// 使用 std::variant 定义多种消息
struct AppToMac {
    std::string content;
};

struct AppToWin {
    std::string content;
};

struct WinToMac {
    std::string content;
};

using Message = std::variant<AppToMac, AppToWin, WinToMac>;

// ─────────────────────────────────────────── 事件总线（中介者） ───────────────────────────────────────────

/**
 * @brief 事件总线：现代中介者实现
 */
class EventBus {
private:
    // 消息类型 → 回调列表
    std::unordered_map<std::string, std::vector<std::function<void(const Message &)>>> listeners;

    // 获取类型名字符串（用于注册）
    template<typename T>
    std::string type_name() {
        if constexpr (std::is_same_v<T, AppToMac>) return "AppToMac";
        if constexpr (std::is_same_v<T, AppToWin>) return "AppToWin";
        if constexpr (std::is_same_v<T, WinToMac>) return "WinToMac";
        return "Unknown";
    }

public:
    /**
     * @brief 注册监听器
     * @tparam T 消息类型
     * @param callback 回调函数
     */
    template<typename T>
    void subscribe(std::function<void(const T &)> callback) {
        auto wrapper = [callback](const Message &msg) {
            if (const auto *payload = std::get_if<T>(&msg)) {
                callback(*payload);
            }
        };
        listeners[type_name<T>()].push_back(wrapper);
    }

    /**
     * @brief 发布消息
     * @tparam T 消息类型
     * @param msg 消息对象
     */
    template<typename T>
    void publish(const T &msg) {
        const auto &type = type_name<T>();
        println("📨 发布消息: {}", type);

        for (const auto &listener: listeners[type]) {
            listener(msg);
        }
    }
};

// ─────────────────────────────────────────── 模块（同事类） ───────────────────────────────────────────

/**
 * @brief App 模块
 */
class App {
    std::shared_ptr<EventBus> bus;

public:
    explicit App(std::shared_ptr<EventBus> b) : bus(b) {}

    void sendMessageToMac(const std::string &content) {
        bus->publish(AppToMac{content});
    }

    void sendMessageToWin(const std::string &content) {
        bus->publish(AppToWin{content});
    }
};

/**
 * @brief Windows 模块
 */
class Windows {
    std::shared_ptr<EventBus> bus;

public:
    explicit Windows(std::shared_ptr<EventBus> b) : bus(b) {}

    void sendMessageToMac(const std::string &content) {
        bus->publish(WinToMac{content});
    }

    // 监听来自 App 的消息
    void listenToApp() {
        bus->subscribe<AppToWin>([this](const AppToWin &msg) {
            println("🖥️  Windows 收到: {}", msg.content);
        });
    }
};

/**
 * @brief Mac 模块
 */
class Mac {
    std::shared_ptr<EventBus> bus;

public:
    explicit Mac(std::shared_ptr<EventBus> b) : bus(b) {}

    // 监听来自 App 和 Windows 的消息
    void listen() {
        bus->subscribe<AppToMac>([this](const AppToMac &msg) {
            println("🍎 Mac 收到 (App): {}", msg.content);
        });

        bus->subscribe<WinToMac>([this](const WinToMac &msg) {
            println("🍎 Mac 收到 (Windows): {}", msg.content);
        });
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试中介者模式
 */
void testMediator() {
    println("🎯 开始测试现代 C++20 中介者模式");

    auto bus = std::make_shared<EventBus>();
    auto app = std::make_shared<App>(bus);
    auto win = std::make_shared<Windows>(bus);
    auto mac = std::make_shared<Mac>(bus);

    // 模块注册监听
    win->listenToApp();
    mac->listen();

    // 发送消息
    app->sendMessageToMac("Hello from App");
    app->sendMessageToWin("Wake up, Windows!");
    win->sendMessageToMac("Windows says Hi");
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    testMediator();
    println("\n✅ 所有测试完成！");
    return 0;
}