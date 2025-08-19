// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Visitor Pattern                                               ┃
// ┃  PURPOSE   ::  Add new operations to objects without modifying their classes ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    visitor.cpp
 * @brief   现代 C++20 实现访问者模式
 * @details 
 *          访问者模式（Visitor Pattern）是一种行为型设计模式，允许你在不修改对象结构的情况下
 *          定义新的操作。它通过“双分派”机制，让访问者根据对象的实际类型调用对应方法。
 * 
 * @par 核心思想
 *      - 被访问者（Element）接受访问者
 *      - 访问者（Visitor）为每种元素(被访问者)提供访问方法
 *      - 结构对象（ObjectStructure）遍历所有元素
 * 
 * @note    本实现提供传统双分派 + std::variant 替代方案
 * @warning 频繁添加新元素类型时，维护成本高
 * @todo    支持 const 重载访问
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>
#include <vector>

using std::println;

// ─────────────────────────────────────────── 前向声明 ───────────────────────────────────────────

class Grove;
class Lake;

// ─────────────────────────────────────────── 抽象访问者 ───────────────────────────────────────────

/**
 * @brief 抽象访问者接口
 */
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const Grove *grove) = 0;
    virtual void visit(const Lake *lake) = 0;
};

// ─────────────────────────────────────────── 抽象被访问者 ───────────────────────────────────────────

/**
 * @brief 抽象元素：公园中的景点
 */
class ParkElement {
public:
    virtual ~ParkElement() = default;
    virtual void accept(Visitor *visitor) const = 0;
    virtual std::string getAddr() const = 0;
};

// ─────────────────────────────────────────── 具体被访问者：树林、湖 ───────────────────────────────────────────

/**
 * @brief 树林
 */
class Grove : public ParkElement {
    std::string addr = "树林";

public:
    std::string getAddr() const override {
        return addr;
    }

    void accept(Visitor *visitor) const override {
        visitor->visit(this);
    }
};

/**
 * @brief 湖
 */
class Lake : public ParkElement {
    std::string addr = "湖边";

public:
    std::string getAddr() const override {
        return addr;
    }

    void accept(Visitor *visitor) const override {
        visitor->visit(this);
    }
};

// ─────────────────────────────────────────── 具体访问者：LWB, Muscle, Lovers ───────────────────────────────────────────

/**
 * @brief LWB 访问者
 */
class LWB : public Visitor {
    std::string name = "lwb";

public:
    void visit(const Grove *grove) override {
        println("{}在{}看美女", name, grove->getAddr());
    }

    void visit(const Lake *lake) override {
        println("{}在{}看美女", name, lake->getAddr());
    }
};

/**
 * @brief Muscle 访问者
 */
class Muscle : public Visitor {
    std::string name = "cmx";

public:
    void visit(const Grove *grove) override {
        println("{}在{}欣赏风景", name, grove->getAddr());
    }

    void visit(const Lake *lake) override {
        println("{}在{}欣赏风景", name, lake->getAddr());
    }
};

/**
 * @brief 小情侣访问者
 */
class Lovers : public Visitor {
    std::string name = "小情侣";

public:
    void visit(const Grove *grove) override {
        println("{}在{}约会", name, grove->getAddr());
    }

    void visit(const Lake *lake) override {
        println("{}在{}约会", name, lake->getAddr());
    }
};

// ─────────────────────────────────────────── 结构对象：Park（组合） ───────────────────────────────────────────

/**
 * @brief 公园：管理所有景点
 * @note 持有所有景点的智能指针
 */
class Park {
    std::vector<std::unique_ptr<ParkElement>> elements;

public:
    /**
     * @brief 添加景点
     */
    void add(std::unique_ptr<ParkElement> element) {
        elements.emplace_back(std::move(element));
    }

    /**
     * @brief 接受访问者
     */
    void accept(Visitor *visitor) const {
        for (const auto &elem: elements) {
            elem->accept(visitor);
        }
    }

    /**
     * @brief 获取景点数量
     */
    size_t size() const {
        return elements.size();
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试直接访问
 */
void testDirectVisit() {
    println("\n🧪 测试 1: 直接访问");

    auto lwb = std::make_unique<LWB>();
    auto muscle = std::make_unique<Muscle>();
    auto lovers = std::make_unique<Lovers>();

    auto grove = std::make_unique<Grove>();
    auto lake = std::make_unique<Lake>();

    // LWB
    grove->accept(lwb.get());
    lake->accept(lwb.get());

    // Muscle
    grove->accept(muscle.get());
    lake->accept(muscle.get());

    // Lovers
    grove->accept(lovers.get());
    lake->accept(lovers.get());
}

/**
 * @brief 测试结构对象管理
 */
void testStructureVisit() {
    println("\n🧪 测试 2: 结构对象管理");

    auto lwb = std::make_unique<LWB>();
    auto muscle = std::make_unique<Muscle>();
    auto lovers = std::make_unique<Lovers>();

    Park park;
    park.add(std::make_unique<Grove>());
    park.add(std::make_unique<Lake>());

    park.accept(lwb.get());
    park.accept(muscle.get());
    park.accept(lovers.get());
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 访问者模式");

    testDirectVisit();
    testStructureVisit();

    println("\n✅ 所有测试完成！");
    return 0;
}