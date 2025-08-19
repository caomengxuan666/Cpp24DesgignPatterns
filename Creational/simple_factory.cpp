// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Simple Factory Pattern                                        ┃
// ┃  PURPOSE   ::  Create objects without exposing instantiation logic          ┃
// ┃  CATEGORY  ::  Creational Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    simple_factory.cpp
 * @brief   现代 C++20 实现简单工厂模式
 * @details 
 *          简单工厂模式（Simple Factory Pattern）是一种创建型设计模式，用于封装对象的创建过程。
 *          它通过一个工厂类或函数，根据输入参数返回不同的具体产品实例，客户端无需知道具体类的实现。
 * 
 * @par 传统实现 vs 现代 C++ 实现
 *      | 传统方式               | 现代 C++20 方式                     |
 *      |------------------------|-------------------------------------|
 *      | 裸指针 + new/delete    | std::unique_ptr + RAII             |
 *      | switch 分支创建        | std::map + std::function           |
 *      | 固定产品列表           | 支持运行时注册扩展                 |
 *      | 无异常处理             | 完整异常安全                       |
 * 
 * @warning 不要直接实例化具体产品类，应通过工厂创建
 * @note    本实现支持动态注册新操作（如 '^' 幂运算）
 * @todo    添加 Operation 的克隆模式支持深拷贝
 * @date    2025-08-19
 * @version 1.0
 */

#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>



// ─────────────────────────────────────────── 抽象产品基类 ───────────────────────────────────────────

/**
 * @brief      抽象产品类：所有操作的基类
 * @details    定义了所有具体操作共有的接口，包括左值、右值和计算结果。
 *             使用纯虚函数确保多态行为。
 * 
 * @par 设计要点
 *      - 虚析构函数确保多态析构安全
 *      - clone() 支持对象复制（原型模式）
 *      - getResult() 标记为 const，不修改对象状态
 */
class Operation {
public:
    double left; ///< 左操作数
    double right;///< 右操作数

    /**
     * @brief 构造函数
     * @param l 左操作数
     * @param r 右操作数
     */
    Operation(double l, double r) : left(l), right(r) {}

    /**
     * @brief 计算结果（纯虚函数）
     * @return double 计算结果
     */
    virtual double getResult() const = 0;

    /**
     * @brief 虚析构函数，确保多态析构
     */
    virtual ~Operation() = default;

    /**
     * @brief 克隆接口（支持原型模式）
     * @return std::unique_ptr<Operation> 新对象副本
     */
    virtual std::unique_ptr<Operation> clone() const = 0;
};

// ─────────────────────────────────────────── 具体产品类 ───────────────────────────────────────────

/**
 * @brief 加法操作
 */
class AddOperation : public Operation {
public:
    using Operation::Operation;// 继承构造函数

    double getResult() const override {
        return left + right;
    }

    std::unique_ptr<Operation> clone() const override {
        return std::make_unique<AddOperation>(*this);
    }
};

/**
 * @brief 减法操作
 */
class SubOperation : public Operation {
public:
    using Operation::Operation;

    double getResult() const override {
        return left - right;
    }

    std::unique_ptr<Operation> clone() const override {
        return std::make_unique<SubOperation>(*this);
    }
};

/**
 * @brief 乘法操作
 */
class MulOperation : public Operation {
public:
    using Operation::Operation;

    double getResult() const override {
        return left * right;
    }

    std::unique_ptr<Operation> clone() const override {
        return std::make_unique<MulOperation>(*this);
    }
};

/**
 * @brief 除法操作
 * @warning 除数为零时抛出异常
 */
class DivOperation : public Operation {
public:
    using Operation::Operation;

    double getResult() const override {
        if (right == 0) {
            throw std::runtime_error("Division by zero");
        }
        return left / right;
    }

    std::unique_ptr<Operation> clone() const override {
        return std::make_unique<DivOperation>(*this);
    }
};

// ─────────────────────────────────────────── 工厂实现 ───────────────────────────────────────────

/**
 * @brief 传统工厂类（静态方法创建对象）
 * @details 使用 switch-case 分支创建不同操作
 * @warning 不支持扩展，需修改源码
 */
class OperationFactory {
public:
    /**
     * @brief 创建操作对象
     * @param op 操作符 ('+', '-', '*', '/')
     * @param left 左操作数
     * @param right 右操作数
     * @return std::unique_ptr<Operation> 智能指针管理的对象
     * @throws std::invalid_argument 不支持的操作符
     */
    static std::unique_ptr<Operation> create(char op, double left, double right) {
        switch (op) {
            case '+':
                return std::make_unique<AddOperation>(left, right);
            case '-':
                return std::make_unique<SubOperation>(left, right);
            case '*':
                return std::make_unique<MulOperation>(left, right);
            case '/':
                return std::make_unique<DivOperation>(left, right);
            default:
                throw std::invalid_argument(std::format("Unsupported operation: {}", op));
        }
    }
};

/**
 * @brief 函数式工厂（现代 C++ 推荐）
 * @details 使用 std::map 和 std::function 实现可扩展工厂
 * @note    支持运行时注册新操作，符合开闭原则
 */
class FunctionalFactory {
    /// 工厂映射表：操作符 → 创建函数
    inline static const std::map<char, std::function<std::unique_ptr<Operation>(double, double)>>
            creators = {
                    {'+', [](double l, double r) { return std::make_unique<AddOperation>(l, r); }},
                    {'-', [](double l, double r) { return std::make_unique<SubOperation>(l, r); }},
                    {'*', [](double l, double r) { return std::make_unique<MulOperation>(l, r); }},
                    {'/', [](double l, double r) { return std::make_unique<DivOperation>(l, r); }},
    };

public:
    /**
     * @brief 创建操作对象
     * @param op 操作符
     * @param left 左操作数
     * @param right 右操作数
     * @return std::unique_ptr<Operation>
     * @throws std::invalid_argument 操作符未注册
     */
    static std::unique_ptr<Operation> create(char op, double left, double right) {
        auto it = creators.find(op);
        if (it == creators.end()) {
            throw std::invalid_argument(std::format("No such operation: '{}'", op));
        }
        return it->second(left, right);
    }

    /**
     * @brief 注册新操作（扩展工厂）
     * @param op 操作符
     * @param creator 创建函数
     * @warning 此操作会覆盖已有操作符
     */
    static void registerOperation(char op, std::function<std::unique_ptr<Operation>(double, double)> creator) {
        const_cast<std::map<char, std::function<std::unique_ptr<Operation>(double, double)>> &>(creators)[op] = std::move(creator);
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试传统工厂
 */
void testSimpleFactory() {
    std::cout << "\n🧪 测试 1: 传统工厂类" << std::endl;
    try {
        auto op1 = OperationFactory::create('+', 10, 5);
        auto op2 = OperationFactory::create('-', 10, 5);
        auto op3 = OperationFactory::create('*', 10, 5);
        auto op4 = OperationFactory::create('/', 10, 5);

        std::cout << "10 + 5 = " << op1->getResult() << std::endl;
        std::cout << "10 - 5 = " << op2->getResult() << std::endl;
        std::cout << "10 * 5 = " << op3->getResult() << std::endl;
        std::cout << "10 / 5 = " << op4->getResult() << std::endl;
    } catch (const std::exception &e) {
        std::cout << "❌ 异常: " << e.what() << std::endl;
    }
}

/**
 * @brief 测试函数式工厂
 */
void testFunctionalFactory() {
    std::cout << "\n🧪 测试 2: 函数式工厂" << std::endl;
    try {
        auto op = FunctionalFactory::create('+', 3.14, 2.86);
        std::cout << "3.14 + 2.86 = " << op->getResult() << std::endl;

        auto div = FunctionalFactory::create('/', 10, 0);
        std::cout << "10 / 0 = " << div->getResult() << std::endl;
    } catch (const std::exception &e) {
        std::cout << "❌ 除零异常捕获: " << e.what() << std::endl;
    }
}

/**
 * @brief 测试工厂扩展性
 */
void testExtensibility() {
    std::cout << "\n🧪 测试 3: 扩展工厂（注册新操作）" << std::endl;

    // 定义 PowerOperation 类
    class PowerOperation : public Operation {
    public:
        using Operation::Operation;
        double getResult() const override {
            return std::pow(left, right);
        }
        std::unique_ptr<Operation> clone() const override {
            return std::make_unique<PowerOperation>(*this);
        }
    };

    // 注册幂运算
    FunctionalFactory::registerOperation('^', [](double l, double r) -> std::unique_ptr<Operation> {
        return std::make_unique<PowerOperation>(l, r);
    });


    try {
        auto powOp = FunctionalFactory::create('^', 2, 3);
        std::cout << "2 ^ 3 = " << powOp->getResult() << std::endl;
    } catch (const std::exception &e) {
        std::cout << "❌ 扩展操作异常: " << e.what() << std::endl;
    }
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int 程序退出码
 */
int main() {
    std::cout << "🎯 开始测试现代 C++20 简单工厂模式" << std::endl;

    testSimpleFactory();
    testFunctionalFactory();
    testExtensibility();

    std::cout << "\n✅ 所有测试完成！" << std::endl;
    return 0;
}