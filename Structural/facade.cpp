// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Facade Pattern                                                ┃
// ┃  PURPOSE   ::  Provide a unified interface to a set of interfaces           ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    facade.cpp
 * @brief   现代 C++20 实现外观模式
 * @details 
 *          外观模式（Facade Pattern）是一种结构型设计模式，为复杂子系统提供一个统一的接口。
 *          它让客户端无需了解子系统的复杂性，只需通过外观类调用高层方法即可完成复杂操作。
 * 
 * @par 核心思想
 *      - 外观类（Facade）封装多个子系统
 *      - 客户端只依赖外观，不关心子系统细节
 *      - 降低耦合，提升可维护性
 * 
 * @note    本实现使用依赖注入 + 智能指针，支持可测试性
 * @warning 避免外观类变成“上帝对象”（了解过多）
 * @todo    支持插件式子系统替换
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>

using std::println;

// ─────────────────────────────────────────── 子系统接口定义 ───────────────────────────────────────────

/**
 * @brief 语法分析器接口
 */
class SyntaxParser {
public:
    virtual ~SyntaxParser() = default;
    virtual void parse() const = 0;
};

/**
 * @brief 中间代码生成器接口
 */
class MidCodeGenerator {
public:
    virtual ~MidCodeGenerator() = default;
    virtual void generateMidCode() const = 0;
};

/**
 * @brief 汇编代码生成器接口
 */
class AssemblyCodeGenerator {
public:
    virtual ~AssemblyCodeGenerator() = default;
    virtual void generateAssembly() const = 0;
};

/**
 * @brief 链接器接口
 */
class Linker {
public:
    virtual ~Linker() = default;
    virtual void link() const = 0;
};

// ─────────────────────────────────────────── 具体子系统实现 ───────────────────────────────────────────

/**
 * @brief C++ 语法分析器
 */
class CSyntaxParser : public SyntaxParser {
public:
    void parse() const override {
        println("🔤 语法分析中：解析 C++ 源码");
    }
};

/**
 * @brief 中间代码生成器
 */
class CMidCode : public MidCodeGenerator {
public:
    void generateMidCode() const override {
        println("⚙️  生成中间代码：LLVM IR");
    }
};

/**
 * @brief 汇编代码生成器
 */
class CAssemblyCode : public AssemblyCodeGenerator {
public:
    void generateAssembly() const override {
        println("📝 生成汇编代码：x86-64");
    }
};

/**
 * @brief 链接器
 */
class Clink : public Linker {
public:
    void link() const override {
        println("🔗 链接成可执行程序：a.out");
    }
};

// ─────────────────────────────────────────── 外观类：CompilerFacade ───────────────────────────────────────────

/**
 * @brief 编译器外观类
 * @details 封装编译过程的四个步骤，提供简单接口
 */
class CompilerFacade {
private:
    std::unique_ptr<SyntaxParser> parser;
    std::unique_ptr<MidCodeGenerator> midGen;
    std::unique_ptr<AssemblyCodeGenerator> asmGen;
    std::unique_ptr<Linker> linker;

public:
    /**
     * @brief 构造函数（依赖注入）
     * @param p 语法分析器
     * @param m 中间代码生成器
     * @param a 汇编代码生成器
     * @param l 链接器
     */
    CompilerFacade(
            std::unique_ptr<SyntaxParser> p,
            std::unique_ptr<MidCodeGenerator> m,
            std::unique_ptr<AssemblyCodeGenerator> a,
            std::unique_ptr<Linker> l) : parser(std::move(p)), midGen(std::move(m)), asmGen(std::move(a)), linker(std::move(l)) {}

    /**
     * @brief 编译：一键执行所有步骤
     */
    void compile() const {
        println("🚀 开始编译...");
        parser->parse();
        midGen->generateMidCode();
        asmGen->generateAssembly();
        linker->link();
        println("🎉 程序编译完成，正在运行...");
    }
};

// ─────────────────────────────────────────── 工厂函数简化创建 ───────────────────────────────────────────

std::unique_ptr<CompilerFacade> makeDefaultCompiler() {
    return std::make_unique<CompilerFacade>(
            std::make_unique<CSyntaxParser>(),
            std::make_unique<CMidCode>(),
            std::make_unique<CAssemblyCode>(),
            std::make_unique<Clink>());
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试默认编译器外观
 */
void testDefaultFacade() {
    println("\n🧪 测试 1: 默认编译器外观");

    auto compiler = makeDefaultCompiler();
    compiler->compile();
}

/**
 * @brief 测试自定义子系统（如替换为 Go 编译器）
 */
void testCustomFacade() {
    println("\n🧪 测试 2: 自定义子系统（模拟 Go 编译器）");

    auto goCompiler = std::make_unique<CompilerFacade>(
            std::make_unique<CSyntaxParser>(),// 可替换
            std::make_unique<CMidCode>(),
            std::make_unique<CAssemblyCode>(),
            std::make_unique<Clink>());

    goCompiler->compile();
}

/**
 * @brief 测试模块化替换
 */
void testModularReplacement() {
    println("\n🧪 测试 3: 模块化替换（使用 Mock 测试）");

    struct MockParser : SyntaxParser {
        void parse() const override {
            println("✅ [Mock] 语法分析通过");
        }
    };

    auto mockCompiler = std::make_unique<CompilerFacade>(
            std::make_unique<MockParser>(),
            std::make_unique<CMidCode>(),
            std::make_unique<CAssemblyCode>(),
            std::make_unique<Clink>());

    mockCompiler->compile();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 外观模式");

    testDefaultFacade();
    testCustomFacade();
    testModularReplacement();

    println("\n✅ 所有测试完成！");
    return 0;
}