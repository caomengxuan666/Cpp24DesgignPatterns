// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Type Erasure                                                  ┃
// ┃  PURPOSE   ::  Hide concrete types behind a uniform interface                ┃
// ┃  CATEGORY  ::  Metaprogramming / Polymorphism                               ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

#include <memory>
#include <print>
#include <vector>

using std::println;

// ─────────────────────────────────────────── 类型擦除接口 ───────────────────────────────────────────

class AnyDrawable {
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template<typename T>
    struct Model : Concept {
        T data;
        Model(T d) : data(std::move(d)) {}
        void draw() const override { data.draw(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(data);
        }
    };

    std::unique_ptr<Concept> self;

public:
    template<typename T>
    AnyDrawable(T data) : self(std::make_unique<Model<T>>(std::move(data))) {}

    AnyDrawable(const AnyDrawable &other) : self(other.self->clone()) {}
    AnyDrawable &operator=(const AnyDrawable &other) {
        self = other.self->clone();
        return *this;
    }

    void draw() const { self->draw(); }
};

// ─────────────────────────────────────────── 示例类型 ───────────────────────────────────────────

struct Circle {
    void draw() const { println("⭕ 画一个圆"); }
};

struct Square {
    void draw() const { println("⬜ 画一个正方形"); }
};

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

void testTypeErasure() {
    println("\n🧪 类型擦除");

    std::vector<AnyDrawable> shapes;
    shapes.emplace_back(Circle{});
    shapes.emplace_back(Square{});

    for (const auto &shape: shapes) {
        shape.draw();
    }
}

int main() {
    testTypeErasure();
}