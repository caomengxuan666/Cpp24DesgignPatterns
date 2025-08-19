// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Singleton Pattern                                             ┃
// ┃  PURPOSE   ::  Ensure a class has only one instance and provide a global    ┃
// ┃                access point to it                                           ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    singleton.cpp
 * @brief   现代 C++ 实现单例模式
 * @details 
 *          单例模式（Singleton Pattern）是一种创建型设计模式，它保证一个类只有一个实例，
 *          并提供一个全局访问点来访问该实例。单例模式在多线程环境中需要特别注意线程安全问题。
 * 
 * @par 核心思想
 *      - 构造函数私有化，防止外部直接实例化
 *      - 提供静态方法作为全局访问点
 *      - 在类内部维护唯一实例
 * 
 * @par 适用场景
 *      - 需要频繁实例化然后销毁的对象
 *      - 创建对象耗时过多或者耗费资源过多，但又要经常用到的对象
 *      - 方便资源相互通信的环境
 *      - 需要生成唯一序列号的环境
 * 
 * @par 实际应用案例
 *      - 多线程中的网络资源初始化
 *      - 回收站机制
 *      - 任务管理器
 *      - 应用程序日志管理
 * 
 * @note    本实现使用 std::call_once 确保线程安全
 * @warning 不要直接实例化单例类，应通过 getInstance 方法获取实例
 * @todo    支持延迟初始化的配置选项
 * @date    2025-05-13
 * @version 1.0
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>


///////////////////////////////////////////////////////
// Singleton 模板类
///////////////////////////////////////////////////////
//
// 使用 Curiously Recurring Template Pattern (CRTP) 实现通用单例模式
// 线程安全，使用 std::call_once 确保只初始化一次

template<class T>
class Singleton {
    inline static std::unique_ptr<T> instance;
    inline static std::once_flag flag;

public:
    // 获取单例实例（无参数版本）
    static T &getInstance() {
        static T instance;
        return instance;
    }

    // 获取单例实例（带参数版本）
    template<typename... Args>
    static T &getInstance(Args &&...args) {
        std::call_once(flag, [&]() {
            instance.reset(new T(std::forward<Args>(args)...));
        });
        return *instance;
    }

    // 销毁单例实例
    static void destroy() {
        instance.reset();
    }
};

///////////////////////////////////////////////////////
// Logger 类
///////////////////////////////////////////////////////
//
// 无参单例示例类
// 用于演示基本的单例模式使用方法

/**
 * @brief 日志记录器类：无参单例示例
 * @details 演示了如何使用 Singleton 模板创建无参单例类
 */
class Logger : public Singleton<Logger> {
    friend class Singleton<Logger>;

private:
    Logger()
        : level_("INFO") {
        std::cout << "[Logger] 初始化: " << level_ << " 级别日志器创建" << std::endl;
    }

public:
    /**
     * @brief 设置日志级别
     * @param level 日志级别字符串
     */
    void setLevel(const std::string &level) {
        level_ = level;
    }

    /**
     * @brief 记录日志信息
     * @param msg 日志消息
     */
    void log(const std::string &msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "[Logger] " << level_ << ": " << msg << std::endl;
    }

    /**
     * @brief 析构函数
     */
    ~Logger() {
        std::cout << "[Logger] 销毁: " << level_ << " 级别日志器" << std::endl;
    }

private:
    std::string level_;
    mutable std::mutex mtx_;
};

///////////////////////////////////////////////////////
// ConfiguredLogger 类
///////////////////////////////////////////////////////
//
// 带参单例示例类
// 用于演示带参数的单例模式使用方法

/**
 * @brief 配置化日志记录器类：带参单例示例
 * @details 演示了如何使用 Singleton 模板创建带参单例类
 */
class ConfiguredLogger : public Singleton<ConfiguredLogger> {
    friend class Singleton<ConfiguredLogger>;

private:
    ConfiguredLogger(std::string name, int id)
        : name_(std::move(name)), id_(id) {
        std::cout << "[ConfiguredLogger] 构造: " << name_ << " (ID: " << id_ << ")" << std::endl;
    }

public:
    /**
     * @brief 记录日志信息
     * @param msg 日志消息
     */
    void log(const std::string &msg) {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "[" << name_ << "] ID" << id_ << ": " << msg << std::endl;
    }

    /**
     * @brief 析构函数
     */
    ~ConfiguredLogger() {
        std::cout << "[ConfiguredLogger] 析构: " << name_ << " (ID: " << id_ << ") 销毁" << std::endl;
    }

private:
    std::string name_;
    int id_;
    mutable std::mutex mtx_;
};

// ---------------------------------------------------
// 测试函数
// 多线程测试函数，演示单例在多线程环境下的使用

void worker(int id) {
    // 无参单例
    auto &logger = Logger::getInstance();

    std::ostringstream oss1;
    oss1 << "我是线程 #" << id;
    logger.log(oss1.str());

    // 带参单例（所有线程共享同一实例）
    auto &configuredLogger = ConfiguredLogger::getInstance("GlobalLogger", 9527);

    std::ostringstream oss2;
    oss2 << "线程 " << id << " 正在使用日志器";
    configuredLogger.log(oss2.str());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// ---------------------------------------------------
// 主函数 - 演示单例模式的使用
//
// 测试内容：
// 1. 无参单例的使用和唯一性验证
// 2. 带参单例的使用和多线程环境下的行为
// 3. 单例实例的全局唯一性验证


int main() {
    std::cout << "🎯 开始测试 CRTP 单例模式" << std::endl
              << std::endl;

    // === 测试 1：无参单例 ===
    std::cout << "🧪 测试 1: 无参单例（CRTP + Meyers）" << std::endl;
    auto &logger1 = Logger::getInstance();
    auto &logger2 = Logger::getInstance();
    logger1.setLevel("DEBUG");
    logger1.log("第一次日志");
    logger2.log("第二次日志（同一实例）");

    std::ostringstream oss3;
    oss3 << "📍 地址检查: &logger1 = " << &logger1 << ", &logger2 = " << &logger2 << " → " << (&logger1 == &logger2 ? "相同" : "不同");
    std::cout << oss3.str() << std::endl
              << std::endl;

    // === 测试 2：带参单例 + 多线程 ===
    std::cout << "🧪 测试 2: 带参单例 + 多线程并发访问" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 1; i <= 5; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto &t: threads) {
        t.join();
    }

    // === 测试 3：验证唯一性 ===
    std::cout << std::endl
              << "🧪 测试 3: 验证 ConfiguredLogger 全局唯一性" << std::endl;
    auto &l1 = ConfiguredLogger::getInstance("GlobalLogger", 9527);
    auto &l2 = ConfiguredLogger::getInstance("GlobalLogger", 9527);

    std::ostringstream oss4;
    oss4 << "📍 地址检查: &l1 = " << &l1 << ", &l2 = " << &l2 << " → " << (&l1 == &l2 ? "相同" : "不同");
    std::cout << oss4.str() << std::endl;

    std::cout << std::endl
              << "🔚 CRTP 单例模式测试完成，程序结束。" << std::endl;
    return 0;
}