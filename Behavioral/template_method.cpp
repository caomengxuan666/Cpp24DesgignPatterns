// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Template Method Pattern                                       ┃
// ┃  PURPOSE   ::  Define algorithm skeleton in base class                      ┃
// ┃  CATEGORY  ::  Behavioral Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    template_method.cpp
 * @brief   现代 C++20 实现模板方法模式
 * @details 
 *          模板方法模式（Template Method Pattern）是一种行为型设计模式，它在抽象基类中定义算法的骨架，
 *          将某些步骤延迟到子类中实现。它让你在不改变算法结构的情况下重新定义算法的某些步骤。
 * 
 * @par 核心思想
 *      - 抽象类定义算法骨架（模板方法）
 *      - 具体子类实现抽象步骤
 *      - 模板方法通常为 final，防止被重写
 * 
 * @note    本实现使用 NVI（非虚接口）模式，提高安全性
 * @warning 避免在构造函数/析构函数中调用虚函数
 * @todo    支持编译时多态（CRTP）
 * @date    2025-08-19
 * @version 1.0
 */

#include <print>
using std::println;

// ─────────────────────────────────────────── 抽象类：MakeObject ───────────────────────────────────────────

/**
 * @brief 抽象基类：定义对象制作流程
 */
class MakeObject {
protected:
    /**
     * @brief 步骤1：制作头部（抽象）
     */
    virtual void makeHead() const = 0;

    /**
     * @brief 步骤2：制作身体（抽象）
     */
    virtual void makeBody() const = 0;

    /**
     * @brief 步骤3：穿衣（抽象）
     */
    virtual void garb() const = 0;

    /**
     * @brief 钩子方法：是否需要特殊处理（可选重写）
     * @return bool
     */
    virtual bool needSpecialProcessing() const {
        return false;
    }

    /**
     * @brief 钩子方法：特殊处理（默认空实现）
     */
    virtual void specialProcessing() const {
        // 子类可重写
    }

public:
    /**
     * @brief 模板方法：定义算法骨架（NVI 模式）
     * @note 声明为 final，防止子类修改流程
     */
    void make() const {
        println("🔧 开始制作对象...");
        makeHead();
        makeBody();
        if (needSpecialProcessing()) {
            specialProcessing();
        }
        garb();
        println("✅ 制作完成！");
    }

    virtual ~MakeObject() = default;
};

// ─────────────────────────────────────────── 具体子类：MakeGirlFriend ───────────────────────────────────────────

/**
 * @brief 制作女朋友
 */
class MakeGirlFriend final : public MakeObject {
    void makeHead() const override {
        println("🧏 阿狸的头");
    }

    void makeBody() const override {
        println("💃 卡莎的身材");
    }

    void garb() const override {
        println("👕 健身服");
    }

    bool needSpecialProcessing() const override {
        return true;
    }

    void specialProcessing() const override {
        println("✨ 添加可爱滤镜 + 美颜");
    }
};

// ─────────────────────────────────────────── 具体子类：MakeBoyFriend ───────────────────────────────────────────

/**
 * @brief 制作男朋友
 */
class MakeBoyFriend final : public MakeObject {
    void makeHead() const override {
        println("🥷 暮光之眼的头");
    }

    void makeBody() const override {
        println("🫡 慎的身材");
    }

    void garb() const override {
        println("🩱 腰子的衣服");
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 使用对象制作器
 * @param maker 制作器引用
 */
void useMaker(const MakeObject& maker) {
    maker.make();
}

/**
 * @brief 测试模板方法模式
 */
void testTemplateMethod() {
    println("\n🧪 测试 1: 模板方法模式");

    MakeGirlFriend girl;
    MakeBoyFriend boy;

    println("👩 制作女朋友：");
    useMaker(girl);

    println("\n👨 制作男朋友：");
    useMaker(boy);
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 模板方法模式");

    testTemplateMethod();

    println("\n✅ 所有测试完成！");
    return 0;
}