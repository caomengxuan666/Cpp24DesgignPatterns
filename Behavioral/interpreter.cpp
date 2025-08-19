// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Interpreter Pattern                                           ┃
// ┃  PURPOSE   ::  Define a grammar and its interpreter                          ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    interpreter.cpp
 * @brief   现代 C++20 实现解释器模式
 * @details 
 *          解释器模式（Interpreter Pattern）是一种行为型设计模式，用于定义语言的文法，
 *          并提供一个解释器来处理该语言中的句子。常用于简单 DSL、表达式计算、规则引擎等。
 * 
 * @par 核心思想
 *      - 抽象表达式（Expression）定义解释接口
 *      - 终结符表达式（VarExpression）表示变量
 *      - 非终结符表达式（Add/Sub）表示运算
 *      - 环境（Environment）存储变量值
 * 
 * @note    本实现使用智能指针 + 递归下降解析，避免内存泄漏
 * @warning 复杂表达式可能导致栈溢出
 * @todo    支持乘除、括号优先级
 * @date    2025-08-19
 * @version 1.0
 */

#include <format>
#include <map>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>

using std::println;
// ─────────────────────────────────────────── 环境：变量上下文 ───────────────────────────────────────────

/**
 * @brief 变量环境：存储变量名到值的映射
 */
using Environment = std::map<std::string, int>;

// ─────────────────────────────────────────── 抽象表达式：Expression ───────────────────────────────────────────

/**
 * @brief 抽象表达式接口
 */
class Expression {
public:
    virtual ~Expression() = default;

    /**
     * @brief 解释表达式
     * @param env 变量环境
     * @return int 计算结果
     */
    virtual int interpret(const Environment &env) const = 0;
};

// ─────────────────────────────────────────── 具体表达式：变量、加法、减法 ───────────────────────────────────────────

/**
 * @brief 变量表达式（终结符）
 */
class VarExpression : public Expression {
    std::string name;

public:
    explicit VarExpression(std::string n) : name(std::move(n)) {}

    int interpret(const Environment &env) const override {
        auto it = env.find(name);
        if (it == env.end()) {
            throw std::runtime_error(std::format("未定义变量: {}", name));
        }
        return it->second;
    }
};

/**
 * @brief 二元表达式基类
 */
class BinaryExpression : public Expression {
protected:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    BinaryExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}
};

/**
 * @brief 加法表达式
 */
class AddExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    int interpret(const Environment &env) const override {
        return left->interpret(env) + right->interpret(env);
    }
};

/**
 * @brief 减法表达式
 */
class SubExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    int interpret(const Environment &env) const override {
        return left->interpret(env) - right->interpret(env);
    }
};

// ─────────────────────────────────────────── 表达式解析器（递归下降） ───────────────────────────────────────────

/**
 * @brief 简单表达式解析器
 * @note 支持 a+b-c 形式（左结合，无优先级）
 */
class Parser {
    std::string expr;
    size_t pos = 0;

    // 获取当前字符
    char peek() const {
        return pos < expr.size() ? expr[pos] : '\0';
    }

    // 消费当前字符
    void consume() {
        if (pos < expr.size()) ++pos;
    }

    // 解析变量名（单字符）
    std::unique_ptr<Expression> parseVariable() {
        if (isalpha(peek())) {
            std::string name(1, peek());
            consume();
            return std::make_unique<VarExpression>(name);
        }
        throw std::runtime_error(std::format("期望变量名，实际: '{}'", peek()));
    }

    // 解析加减表达式
    std::unique_ptr<Expression> parseExpression() {
        auto left = parseVariable();

        while (peek() == '+' || peek() == '-') {
            char op = peek();
            consume();
            auto right = parseVariable();

            if (op == '+') {
                left = std::make_unique<AddExpression>(std::move(left), std::move(right));
            } else {
                left = std::make_unique<SubExpression>(std::move(left), std::move(right));
            }
        }

        return left;
    }

public:
    explicit Parser(std::string e) : expr(std::move(e)) {}

    std::unique_ptr<Expression> parse() {
        pos = 0;
        return parseExpression();
    }
};

// ─────────────────────────────────────────── 计算器封装类 ───────────────────────────────────────────

/**
 * @brief 计算器：解析并求值表达式
 */
class Calculator {
    std::unique_ptr<Expression> expression;

public:
    /**
     * @brief 构造并解析表达式
     * @param expr 表达式字符串（如 "a+b-c"）
     */
    explicit Calculator(const std::string &expr) {
        try {
            Parser parser(expr);
            expression = parser.parse();
            println("✅ 语法树构建成功: {}", expr);
        } catch (const std::exception &e) {
            println("❌ 解析失败: {}", e.what());
            throw;
        }
    }

    /**
     * @brief 运行计算
     * @param env 变量环境
     * @return int 结果
     */
    int run(const Environment &env) const {
        if (!expression) {
            throw std::runtime_error("表达式为空");
        }
        return expression->interpret(env);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本表达式
 */
void testBasicExpression() {
    println("\n🧪 测试 1: 基本表达式 a+b-c");

    Environment env{{"a", 10}, {"b", 100}, {"c", 30}};

    try {
        Calculator calc("a+b-c");
        int result = calc.run(env);
        println("📊 计算结果: a+b-c = 10+100-30 = {}", result);
    } catch (const std::exception &e) {
        println("❌ 错误: {}", e.what());
    }
}

/**
 * @brief 测试未定义变量
 */
void testUndefinedVariable() {
    println("\n🧪 测试 2: 未定义变量");

    Environment env{{"a", 10}, {"b", 20}};

    try {
        Calculator calc("a+b+c");
        calc.run(env);
    } catch (const std::exception &e) {
        println("✅ 捕获预期异常: {}", e.what());
    }
}

/**
 * @brief 测试复杂表达式
 */
void testComplexExpression() {
    println("\n🧪 测试 3: 复杂表达式 a-b+c-d");

    Environment env{{"a", 100}, {"b", 10}, {"c", 5}, {"d", 3}};

    try {
        Calculator calc("a-b+c-d");
        int result = calc.run(env);
        println("📊 计算结果: a-b+c-d = 100-10+5-3 = {}", result);
    } catch (const std::exception &e) {
        println("❌ 错误: {}", e.what());
    }
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 解释器模式");

    testBasicExpression();
    testUndefinedVariable();
    testComplexExpression();

    println("\n✅ 所有测试完成！");
    return 0;
}