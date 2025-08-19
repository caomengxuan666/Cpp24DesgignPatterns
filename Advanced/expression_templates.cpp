// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Expression Templates                                          ┃
// ┃  PURPOSE   ::  Lazy evaluation for mathematical expressions                  ┃
// ┃  CATEGORY  ::  Metaprogramming / High-Performance Computing                 ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

#include <print>
#include <vector>

using std::println;
// ─────────────────────────────────────────── 向量表达式基类 ───────────────────────────────────────────

template<typename T>
struct VectorExpr {
    const T &self() const { return static_cast<const T &>(*this); }
};

// ─────────────────────────────────────────── 向量类 ───────────────────────────────────────────

class Vector : public VectorExpr<Vector> {
    std::vector<double> data;

public:
    explicit Vector(size_t n) : data(n) {}
    Vector(std::initializer_list<double> il) : data(il) {}

    double &operator[](size_t i) { return data[i]; }
    const double &operator[](size_t i) const { return data[i]; }
    size_t size() const { return data.size(); }

    // 赋值：支持表达式模板
    template<typename E>
    Vector &operator=(const VectorExpr<E> &expr) {
        const E &e = expr.self();
        for (size_t i = 0; i < size(); ++i) {
            data[i] = e[i];
        }
        return *this;
    }
};

// ─────────────────────────────────────────── 加法表达式 ───────────────────────────────────────────

template<typename L, typename R>
struct AddExpr : VectorExpr<AddExpr<L, R>> {
    const L &lhs;
    const R &rhs;
    AddExpr(const L &l, const R &r) : lhs(l), rhs(r) {}
    double operator[](size_t i) const { return lhs[i] + rhs[i]; }
};

// ─────────────────────────────────────────── 操作符重载 ───────────────────────────────────────────

template<typename L, typename R>
AddExpr<L, R> operator+(const VectorExpr<L> &l, const VectorExpr<R> &r) {
    return AddExpr<L, R>(l.self(), r.self());
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testExpressionTemplates() {
    println("\n🧪 表达式模板：避免临时对象");

    Vector a = {1, 2, 3};
    Vector b = {4, 5, 6};
    Vector c = {7, 8, 9};
    Vector result(3);

    // 传统方式：创建临时对象
    // result = a + b + c;

    // 表达式模板：延迟计算，零临时对象
    result = a + b + c;

    for (size_t i = 0; i < result.size(); ++i) {
        println("result[{}] = {}", i, result[i]);
    }
}

int main() {
    testExpressionTemplates();
    return 0;
}