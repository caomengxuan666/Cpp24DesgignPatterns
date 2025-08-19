// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Pimpl Idiom (4 Implementations)                               ┃
// ┃  PURPOSE   ::  Hide implementation with different modern C++ techniques      ┃
// ┃  CATEGORY  ::  Implementation Pattern                                       ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    pimpl.cpp
 * @brief   Pimpl 惯用法的 4 种现代 C++ 实现
 * @details 
 *          Pimpl（Pointer to Implementation）是一种用于隐藏类实现细节的 C++ 惯用法。
 *          它能减少编译依赖、加快编译速度、实现二进制兼容性。
 * 
 * @par 4 种实现方式
 *      1. 经典 Pimpl：std::unique_ptr + 前向声明
 *      2. 完美 Pimpl：自定义删除器，避免包含 <memory>
 *      3. 多态 Pimpl：接口 + 多态实现，支持插件化
 *      4. 模块化 Pimpl：C++20 模块（模拟）
 * 
 * @note    本实现展示工业级 Pimpl 设计
 * @warning 析构函数必须在 cpp 文件中定义
 * @todo    支持移动语义的完美转发
 * @date    2025-08-19
 * @version 1.0
 */

#include <print>
#include <memory>
#include <string>
#include <vector>

using std::println;
// ─────────────────────────────────────────── 1️⃣ 经典 Pimpl（Classic Pimpl） ───────────────────────────────────────────

/**
 * @brief 经典 Pimpl：最常见、最稳定的实现
 */
class WidgetClassic {
    class Impl;  // 前向声明
    std::unique_ptr<Impl> pImpl;

public:
    WidgetClassic();
    ~WidgetClassic();  // 必须在 cpp 中定义
    WidgetClassic(WidgetClassic&&) = default;
    WidgetClassic& operator=(WidgetClassic&&) = default;

    void process(const std::string& data);
    std::string getStatus() const;
};

// 实现在“cpp 文件”中
class WidgetClassic::Impl {
public:
    std::string status = "idle";
    std::vector<std::string> log;

    void process(const std::string& data) {
        log.push_back("Processed: " + data);
        status = "running";
        println("🔧 Classic: 处理 {}", data);
    }

    std::string getStatus() const {
        return std::format("{} ({} 条日志)", status, log.size());
    }
};

WidgetClassic::WidgetClassic() : pImpl(std::make_unique<Impl>()) {}
WidgetClassic::~WidgetClassic() = default;

void WidgetClassic::process(const std::string& data) {
    pImpl->process(data);
}

std::string WidgetClassic::getStatus() const {
    return pImpl->getStatus();
}

// ─────────────────────────────────────────── 2️⃣ 完美 Pimpl（Perfect Pimpl） ───────────────────────────────────────────

/**
 * @brief 完美 Pimpl
 * @note 使用自定义删除器，Impl 可以是不完整类型
 */
class WidgetPerfect {
    struct Impl;  // 仅前向声明，不定义
    struct ImplDeleter {
        void operator()(Impl* p) const;
    };
    std::unique_ptr<Impl, ImplDeleter> pImpl;

public:
    WidgetPerfect();
    ~WidgetPerfect();
    void process(const std::string& data);
    std::string getStatus() const;
};

// ✅ 所有 Impl 的定义和使用都在“cpp 文件”中
struct WidgetPerfect::Impl {
    std::string status = "perfect";
    int count = 0;

    void process(const std::string& data) {
        count++;
        println("✨ Perfect: 处理 {} (第 {} 次)", data, count);
    }

    std::string getStatus() const {
        return std::format("{} (count={})", status, count);
    }
};

void WidgetPerfect::ImplDeleter::operator()(WidgetPerfect::Impl* p) const {
    delete p;
}

WidgetPerfect::WidgetPerfect() : pImpl(new Impl) {}
WidgetPerfect::~WidgetPerfect() = default;

void WidgetPerfect::process(const std::string& data) {
    pImpl->process(data);
}

std::string WidgetPerfect::getStatus() const {
    return pImpl->getStatus();
}

// ─────────────────────────────────────────── 3️⃣ 多态 Pimpl（Polymorphic Pimpl） ───────────────────────────────────────────

/**
 * @brief 多态 Pimpl：支持运行时切换实现（插件化）
 */
class Processor {
public:
    virtual ~Processor() = default;
    virtual void process(const std::string& data) = 0;
    virtual std::string getName() const = 0;
};

class FastProcessor : public Processor {
public:
    void process(const std::string& data) override {
        println("⚡ Fast: 快速处理 {}", data);
    }
    std::string getName() const override { return "Fast"; }
};

class SafeProcessor : public Processor {
public:
    void process(const std::string& data) override {
        println("🛡️  Safe: 安全处理 {}", data);
    }
    std::string getName() const override { return "Safe"; }
};

class WidgetPolymorphic {
    std::unique_ptr<Processor> processor;

public:
    // 可注入不同实现
    explicit WidgetPolymorphic(std::unique_ptr<Processor> proc)
        : processor(std::move(proc)) {}

    void process(const std::string& data) {
        processor->process(data);
    }

    std::string getProcessorName() const {
        return processor->getName();
    }
};

// ─────────────────────────────────────────── 4️⃣ 模块化 Pimpl（C++20 模拟） ───────────────────────────────────────────

/**
 * @brief 模块化 Pimpl：使用 C++20 模块（语法模拟）
 * @note 实际使用需编译器支持 modules
 */
// 模拟模块接口文件（.ixx）
class WidgetModular {
    class Impl;  // 仍隐藏实现
    std::unique_ptr<Impl> pImpl;

public:
    WidgetModular();
    ~WidgetModular();
    void process(const std::string& data);
};

// 模拟模块实现文件（.cppm）
class WidgetModular::Impl {
public:
    void process(const std::string& data) {
        println("📦 Modular: 处理 {}", data);
    }
};

WidgetModular::WidgetModular() : pImpl(std::make_unique<Impl>()) {}
WidgetModular::~WidgetModular() = default;

void WidgetModular::process(const std::string& data) {
    pImpl->process(data);
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testClassic() {
    println("\n🧪 1. 经典 Pimpl");
    WidgetClassic w;
    w.process("Hello");
    println("📊 状态: {}", w.getStatus());
}

void testPerfect() {
    println("\n🧪 2. 完美 Pimpl");
    WidgetPerfect w;
    w.process("World");
    println("📊 状态: {}", w.getStatus());
}

void testPolymorphic() {
    println("\n🧪 3. 多态 Pimpl");
    WidgetPolymorphic fast(std::make_unique<FastProcessor>());
    WidgetPolymorphic safe(std::make_unique<SafeProcessor>());

    fast.process("Task1");
    safe.process("Task2");

    println("🔧 Fast 使用: {}", fast.getProcessorName());
    println("🔧 Safe 使用: {}", safe.getProcessorName());
}

void testModular() {
    println("\n🧪 4. 模块化 Pimpl（模拟）");
    WidgetModular w;
    w.process("Modular Data");
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试 Pimpl 惯用法的 4 种实现");

    testClassic();
    testPerfect();
    testPolymorphic();
    testModular();

    println("\n✅ 所有测试完成！");
    return 0;
}