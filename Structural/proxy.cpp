// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Proxy Pattern                                                 ┃
// ┃  PURPOSE   ::  Provide a surrogate to control access to an object           ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    proxy.cpp
 * @brief   现代 C++20 实现代理模式
 * @details 
 *          代理模式（Proxy Pattern）是一种行为型设计模式，为其他对象提供一种代理以控制对这个对象的访问。
 *          它常用于延迟加载、权限控制、日志记录、缓存等场景。
 * 
 * @par 核心思想
 *      - 抽象主题（Subject）定义接口
 *      - 真实主题（RealSubject）实现业务逻辑
 *      - 代理（Proxy）持有真实对象，控制访问
 *      - 客户端通过代理访问真实对象
 * 
 * @note    本实现使用智能指针 + 依赖注入，避免内存泄漏
 * @warning 代理应调用真实对象，而非完全替代
 * @todo    支持动态代理（运行时生成）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>

using std::println;

// ─────────────────────────────────────────── 抽象主题：Subject ───────────────────────────────────────────

/**
 * @brief 抽象主题：购票接口
 */
class Subject {
public:
    virtual ~Subject() = default;
    virtual void purchaseTicket() = 0;
};

// ─────────────────────────────────────────── 真实主题：User ───────────────────────────────────────────

/**
 * @brief 真实用户：执行实际购票逻辑
 */
class User : public Subject {
public:
    void purchaseTicket() override {
        println("🎫 用户买票：选择座位、支付、出票");
    }
};

// ─────────────────────────────────────────── 代理基类（可选） ───────────────────────────────────────────

/**
 * @brief 代理基类（可选，用于统一接口）
 */
class ProxyBase : public Subject {
protected:
    std::unique_ptr<Subject> realSubject;

public:
    explicit ProxyBase(std::unique_ptr<Subject> subject)
        : realSubject(std::move(subject)) {}
};

// ─────────────────────────────────────────── 具体代理：携程购票代理 ───────────────────────────────────────────

/**
 * @brief 携程代理：购票中介
 */
class CtripProxy : public ProxyBase {
public:
    using ProxyBase::ProxyBase;

    void purchaseTicket() override {
        println("🏨 携程代理：开始购票流程");
        println("🔍 携程代理：验证用户身份");
        println("💳 携程代理：添加保险服务（+20元）");

        // ✅ 调用真实对象
        if (realSubject) {
            realSubject->purchaseTicket();
        } else {
            println("❌ 携程代理：未设置真实对象");
        }

        println("📧 携程代理：发送电子票到邮箱");
        println("📊 携程代理：记录用户行为日志");
    }
};

// ─────────────────────────────────────────── 增强代理：日志 + 权限代理 ───────────────────────────────────────────

/**
 * @brief 日志代理：记录方法调用
 */
class LoggingProxy : public ProxyBase {
public:
    using ProxyBase::ProxyBase;

    void purchaseTicket() override {
        println("📝 日志代理：方法 purchaseTicket() 调用开始");
        if (realSubject) {
            realSubject->purchaseTicket();
        }
        println("📝 日志代理：方法 purchaseTicket() 调用结束");
    }
};

/**
 * @brief 权限代理：控制访问
 */
class AuthProxy : public ProxyBase {
    std::string userRole;

public:
    AuthProxy(std::unique_ptr<Subject> subject, std::string role)
        : ProxyBase(std::move(subject)), userRole(std::move(role)) {}

    void purchaseTicket() override {
        if (userRole == "admin" || userRole == "user") {
            println("🔐 权限代理：用户角色 '{}' 通过验证", userRole);
            if (realSubject) {
                realSubject->purchaseTicket();
            }
        } else {
            println("🚫 权限代理：用户角色 '{}' 无权限购票", userRole);
        }
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 使用代理购票
 */
void purchaseWithProxy(Subject &subject) {
    subject.purchaseTicket();
}

/**
 * @brief 测试基本代理
 */
void testBasicProxy() {
    println("\n🧪 测试 1: 基本代理模式（携程）");

    auto user = std::make_unique<User>();
    auto ctrip = std::make_unique<CtripProxy>(std::move(user));

    purchaseWithProxy(*ctrip);
}

/**
 * @brief 测试代理链（装饰者风格）
 */
void testProxyChain() {
    println("\n🧪 测试 2: 代理链（日志 + 权限 + 真实对象）");

    auto user = std::make_unique<User>();
    auto auth = std::make_unique<AuthProxy>(std::move(user), "user");
    auto logging = std::make_unique<LoggingProxy>(std::move(auth));

    purchaseWithProxy(*logging);
}

/**
 * @brief 测试权限控制
 */
void testAuthControl() {
    println("\n🧪 测试 3: 权限控制");

    auto user = std::make_unique<User>();

    // 普通用户
    auto authUser = std::make_unique<AuthProxy>(std::move(user), "user");
    purchaseWithProxy(*authUser);

    // 游客
    auto guest = std::make_unique<User>();
    auto authGuest = std::make_unique<AuthProxy>(std::move(guest), "guest");
    purchaseWithProxy(*authGuest);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 代理模式");

    testBasicProxy();
    testProxyChain();
    testAuthControl();

    println("\n✅ 所有测试完成！");
    return 0;
}