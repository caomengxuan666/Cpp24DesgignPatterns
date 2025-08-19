// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  State Pattern                                                 ┃
// ┃  PURPOSE   ::  Allow an object to alter its behavior when its state changes  ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    state.cpp
 * @brief   现代 C++20 实现状态模式
 * @details 
 *          状态模式（State Pattern）是一种行为型设计模式，允许对象在其内部状态改变时改变其行为。
 *          它让对象看起来像是修改了其类。常用于有限状态机（FSM）、游戏角色行为、UI 状态管理等。
 * 
 * @par 核心思想
 *      - 上下文（Context）持有当前状态
 *      - 状态（State）封装特定行为
 *      - 状态切换时行为自动变化
 * 
 * @note    本实现支持 enter/exit 回调和模板化状态切换
 * @warning 避免状态切换时的无限递归
 * @todo    支持状态栈（push/pop）
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>

using std::println;

// ─────────────────────────────────────────── 前向声明 ───────────────────────────────────────────

class Context;

// ─────────────────────────────────────────── 抽象状态基类 ───────────────────────────────────────────

/**
 * @brief 抽象状态接口
 */
class State {
public:
    virtual ~State() = default;

    /**
     * @brief 处理请求
     * @param context 上下文
     */
    virtual void handle(Context &context) = 0;

    /**
     * @brief 状态进入时调用
     */
    virtual void onEnter(Context &context) {
        // 可选重写
    }

    /**
     * @brief 状态退出时调用
     */
    virtual void onExit(Context &context) {
        // 可选重写
    }
};

// ─────────────────────────────────────────── 上下文：Context ───────────────────────────────────────────

/**
 * @brief 上下文：持有当前状态
 */
class Context {
private:
    std::unique_ptr<State> currentState;

public:
    /**
     * @brief 构造上下文并设置初始状态
     * @tparam StateType 初始状态类型
     * @param args 状态构造参数
     */
    template<typename StateType, typename... Args>
    static Context create(Args &&...args) {
        Context context;
        context.changeState<StateType>(std::forward<Args>(args)...);
        return context;
    }


    /**
     * @brief 请求处理（委托给当前状态）
     */
    void request() {
        if (currentState) {
            currentState->handle(*this);
        } else {
            println("⚠️  无当前状态");
        }
    }

    /**
     * @brief 切换状态
     * @tparam StateType 新状态类型
     * @param args 构造参数
     */
    template<typename StateType, typename... Args>
    void changeState(Args &&...args) {
        if (currentState) {
            currentState->onExit(*this);
            auto &currentStateRef = *currentState;
            println("🔚 退出状态: {}", typeid(currentStateRef).name());
        }

        currentState = std::make_unique<StateType>(std::forward<Args>(args)...);
        currentState->onEnter(*this);
        auto &currentStateRef = *currentState;
        println("🚀 进入状态: {}", typeid(currentStateRef).name());
    }
};

// ─────────────────────────────────────────── 具体状态：ConcreteStateNone, ConcreteStateExist ───────────────────────────────────────────

/**
 * @brief 无状态
 */
class ConcreteStateNone : public State {
public:
    void handle(Context &context) override {
        println("📭 没有......");
    }

    void onEnter(Context &context) override {
        println("👉 进入「无」状态：准备接收新任务");
    }
};

/**
 * @brief 存在状态
 */
class ConcreteStateExist : public State {
public:
    void handle(Context &context) override {
        println("📬 有......");
    }

    void onEnter(Context &context) override {
        println("👉 进入「有」状态：开始处理任务");
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本状态切换
 */
void testBasicState() {
    println("\n🧪 测试 1: 基本状态模式");

    auto context = Context::create<ConcreteStateNone>();

    context.request();

    context.changeState<ConcreteStateExist>();
    context.request();
}
/**
 * @brief 测试模板化状态切换
 */
void testTemplateState() {
    println("\n🧪 测试 2: 模板化状态切换");

    auto context = Context::create<ConcreteStateNone>();

    context.request();

    context.changeState<ConcreteStateExist>();
    context.request();

    context.changeState<ConcreteStateNone>();
    context.request();
}
/**
 * @brief 测试状态切换回调
 */
void testStateLifecycle() {
    println("\n🧪 测试 3: 状态生命周期回调");

    auto context = Context::create<ConcreteStateNone>();

    context.changeState<ConcreteStateExist>();
    context.changeState<ConcreteStateNone>();
}


// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 状态模式");

    testBasicState();
    testTemplateState();
    testStateLifecycle();

    println("\n✅ 所有测试完成！");
    return 0;
}