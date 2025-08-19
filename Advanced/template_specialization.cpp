// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Template Specialization                                       ┃
// ┃  PURPOSE   ::  Provide specialized implementations for specific types        ┃
// ┃  CATEGORY  ::  Metaprogramming                                              ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
#include <print>
#include <string>

using std::println;

// ─────────────────────────────────────────── 通用模板 ───────────────────────────────────────────

template<typename T>
struct Printer {
    void print(const T &value) {
        println("🖨️  通用打印: {}", value);
    }
};

// ─────────────────────────────────────────── 特化：std::string ───────────────────────────────────────────

template<>
struct Printer<std::string> {
    void print(const std::string &value) {
        println("🔤 字符串打印: \"{}\"", value);
    }
};

// ─────────────────────────────────────────── 特化：const char* ───────────────────────────────────────────

template<>
struct Printer<const char *> {
    void print(const char *value) {
        println("🔠 C 字符串打印: \"{}\"", value);
    }
};

// ─────────────────────────────────────────── 偏特化：指针类型 ───────────────────────────────────────────

template<typename T>
struct Printer<T *> {
    void print(T *ptr) {
        if (ptr) {
            println("🔗 指针打印: {} (值: {})", static_cast<void *>(ptr), *ptr);
        } else {
            println("❌ 空指针");
        }
    }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testTemplateSpecialization() {
    println("\n🧪 模板特化");

    Printer<int>{}.print(42);
    Printer<std::string>{}.print("Hello");
    Printer<const char *>{}.print("World");
    int x = 100;
    Printer<int *>{}.print(&x);
    Printer<int *>{}.print(nullptr);
}

int main() {
    testTemplateSpecialization();
}