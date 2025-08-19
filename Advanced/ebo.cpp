// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Empty Base Optimization (EBO)                                 ┃
// ┃  PURPOSE   ::  Reduce memory footprint of empty base classes                 ┃
// ┃  CATEGORY  ::  Optimization / Metaprogramming                               ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

#include <print>
using std::println;
// ─────────────────────────────────────────── 空基类 ───────────────────────────────────────────

struct Empty {};

struct EmptyWithVirtual {
    virtual ~EmptyWithVirtual() = default;
};

// ─────────────────────────────────────────── 派生类 ───────────────────────────────────────────

struct Derived1 : Empty {
    int value;
};

struct Derived2 : EmptyWithVirtual {
    int value;
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testEBO() {
    println("\n🧪 空基类优化");

    println("sizeof(Empty): {}", sizeof(Empty));
    println("sizeof(EmptyWithVirtual): {}", sizeof(EmptyWithVirtual));
    println("sizeof(Derived1): {}", sizeof(Derived1));// EBO 可能生效
    println("sizeof(Derived2): {}", sizeof(Derived2));// 虚表指针阻止 EBO

    // 验证 EBO
    if (sizeof(Derived1) == sizeof(int)) {
        println("✅ EBO 生效：空基类不增加大小");
    } else {
        println("❌ EBO 未生效");
    }
}

int main() {
    testEBO();
}