// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Strategy Pattern                                              ┃
// ┃  PURPOSE   ::  Encapsulate algorithms and make them interchangeable         ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    strategy.cpp
 * @brief   现代 C++20 实现策略模式
 * @details 
 *          策略模式（Strategy Pattern）是一种行为型设计模式，将算法封装成独立的对象，
 *          使它们可以互相替换。它让你能在运行时改变对象的行为，而无需修改其代码。
 * 
 * @par 核心思想
 *      - 抽象策略（Strategy）定义算法接口
 *      - 具体策略（ConcreteStrategy）实现具体算法
 *      - 上下文（Context）持有当前策略并委托执行
 * 
 * @note    本实现支持继承式 + 函数式两种策略
 * @warning 避免策略对象生命周期问题
 * @todo    支持策略注册表（Strategy Registry）
 * @date    2025-08-19
 * @version 1.0
 */

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <string>

using std::println;

// ─────────────────────────────────────────── 抽象策略接口 ───────────────────────────────────────────

/**
 * @brief 策略接口：定义计算操作
 */
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int execute(int left, int right) const = 0;
    virtual std::string getName() const = 0;
};

// ─────────────────────────────────────────── 具体策略：加减乘除 ───────────────────────────────────────────

/**
 * @brief 加法策略
 */
class AddStrategy : public Strategy {
public:
    int execute(int left, int right) const override {
        return left + right;
    }

    std::string getName() const override {
        return "加法 (+)";
    }
};

/**
 * @brief 减法策略
 */
class SubStrategy : public Strategy {
public:
    int execute(int left, int right) const override {
        return left - right;
    }

    std::string getName() const override {
        return "减法 (-)";
    }
};

/**
 * @brief 乘法策略
 */
class MulStrategy : public Strategy {
public:
    int execute(int left, int right) const override {
        return left * right;
    }

    std::string getName() const override {
        return "乘法 (*)";
    }
};

/**
 * @brief 除法策略（带错误处理）
 */
class DivStrategy : public Strategy {
public:
    int execute(int left, int right) const override {
        if (right == 0) {
            println("❌ 除数不能为 0");
            return 0;
        }
        return left / right;
    }

    std::string getName() const override {
        return "除法 (/)";
    }
};

// ─────────────────────────────────────────── 上下文：Context ───────────────────────────────────────────

/**
 * @brief 上下文：使用策略的对象
 */
class Context {
private:
    std::unique_ptr<Strategy> strategy;

public:
    /**
     * @brief 设置策略
     * @param s 策略对象（转移所有权）
     */
    void setStrategy(std::unique_ptr<Strategy> s) {
        strategy = std::move(s);
        println("🎯 切换策略: {}", strategy->getName());
    }

    /**
     * @brief 执行计算
     * @param left 左操作数
     * @param right 右操作数
     * @return int 结果
     */
    int execute(int left, int right) const {
        if (!strategy) {
            throw std::runtime_error("未设置策略");
        }
        int result = strategy->execute(left, right);
        println("🧮 计算: {} {} {} = {}", left, getOperator(), right, result);
        return result;
    }

    /**
     * @brief 获取当前操作符（用于显示）
     * @return std::string
     */
    std::string getOperator() const {
        if (!strategy) return "?";
        auto name = strategy->getName();
        if (name == "加法 (+)") return "+";
        if (name == "减法 (-)") return "-";
        if (name == "乘法 (*)") return "*";
        if (name == "除法 (/)") return "/";
        return "?";
    }
};

// ─────────────────────────────────────────── 函数式策略（现代 C++ 风格） ───────────────────────────────────────────

/**
 * @brief 使用 std::function 实现的策略容器
 */
class FunctionalContext {
private:
    std::function<int(int, int)> strategy;
    std::string opName;

public:
    /**
     * @brief 设置策略函数
     * @param func 算法函数
     * @param name 名称
     */
    void setStrategy(std::function<int(int, int)> func, std::string name) {
        strategy = std::move(func);
        opName = std::move(name);
        println("🎯 切换函数式策略: {}", opName);
    }

    /**
     * @brief 执行计算
     */
    int execute(int left, int right) const {
        if (!strategy) {
            throw std::runtime_error("未设置策略");
        }
        int result = strategy(left, right);
        println("🧮 计算: {} {} {} = {}", left, getOperator(), right, result);
        return result;
    }

    std::string getOperator() const {
        if (opName == "加法") return "+";
        if (opName == "减法") return "-";
        if (opName == "乘法") return "*";
        if (opName == "除法") return "/";
        return "?";
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试继承式策略
 */
void testInheritanceStrategy() {
    println("\n🧪 测试 1: 继承式策略模式");

    Context context;

    context.setStrategy(std::make_unique<AddStrategy>());
    context.execute(10, 5);

    context.setStrategy(std::make_unique<SubStrategy>());
    context.execute(10, 5);

    context.setStrategy(std::make_unique<MulStrategy>());
    context.execute(10, 5);

    context.setStrategy(std::make_unique<DivStrategy>());
    context.execute(10, 5);
    context.execute(10, 0);// 测试除零
}

/**
 * @brief 测试函数式策略（支持 lambda）
 */
void testFunctionalStrategy() {
    println("\n🧪 测试 2: 函数式策略（std::function + lambda）");

    FunctionalContext context;

    context.setStrategy([](int a, int b) { return a + b; }, "加法");
    context.execute(8, 3);

    context.setStrategy([](int a, int b) { return a * b; }, "乘法");
    context.execute(8, 3);

    // 自定义策略：幂运算
    context.setStrategy([](int a, int b) -> int {
        int result = 1;
        for (int i = 0; i < b; ++i) result *= a;
        return result;
    },
                        "幂运算");
    context.execute(2, 3);
}

/**
 * @brief 测试交互式计算器
 */
void testInteractiveCalculator() {
    println("\n🧪 测试 3: 交互式计算器（输入: 左值 运算符 右值）");
    println("📌 输入 'quit' 退出");

    Context context;
    std::map<char, std::unique_ptr<Strategy>> strategies;

    // 使用 try_emplace 插入（C++17）
    strategies.try_emplace('+', std::make_unique<AddStrategy>());
    strategies.try_emplace('-', std::make_unique<SubStrategy>());
    strategies.try_emplace('*', std::make_unique<MulStrategy>());
    strategies.try_emplace('/', std::make_unique<DivStrategy>());

    int left, right;
    char op;

    while (std::cin >> left >> op >> right) {
        auto it = strategies.find(op);
        if (it != strategies.end()) {
            context.setStrategy(std::move(it->second));
            context.execute(left, right);
        } else {
            println("❌ 不支持的操作符: {}", op);
        }
    }
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 策略模式");

    testInheritanceStrategy();
    testFunctionalStrategy();
    // testInteractiveCalculator();  // 取消注释以启用交互模式

    println("\n✅ 所有测试完成！");
    return 0;
}