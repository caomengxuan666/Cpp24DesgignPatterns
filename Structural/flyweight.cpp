// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Flyweight Pattern                                             ┃
// ┃  PURPOSE   ::  Use sharing to support large numbers of fine-grained objects ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    flyweight.cpp
 * @brief   现代 C++20 实现享元模式
 * @details 
 *          享元模式（Flyweight Pattern）是一种结构型设计模式，通过共享技术有效地支持大量细粒度的对象。
 *          它将对象的状态分为“内在状态”（共享）和“外在状态”（外部传入），从而减少内存使用。
 * 
 * @par 核心思想
 *      - 内在状态（Intrinsic State）：可共享，如汽车品牌、型号、颜色
 *      - 外在状态（Extrinsic State）：不可共享，如车主、车牌
 *      - 享元工厂（FlyweightFactory）缓存并复用享元对象
 * 
 * @note    本实现使用 std::shared_ptr + 值语义，避免内存泄漏
 * @warning 外在状态必须由客户端维护
 * @todo    支持线程安全的工厂
 * @date    2025-08-19
 * @version 1.0
 */


#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <sstream>
#include <string>

using std::println;
// ─────────────────────────────────────────── 内在状态：SharedState ───────────────────────────────────────────

/**
 * @brief 内在状态：可共享的汽车属性
 */
struct SharedState {
    std::string brand;
    std::string model;
    std::string color;

    SharedState(std::string b, std::string m, std::string c)
        : brand(std::move(b)), model(std::move(m)), color(std::move(c)) {}

    // 生成唯一键
    std::string getKey() const {
        std::ostringstream oss;
        oss << brand << "_" << model << "_" << color;
        return oss.str();
    }

    friend std::ostream &operator<<(std::ostream &out, const SharedState &ss) {
        return out << "[" << ss.brand << ", " << ss.model << ", " << ss.color << "]";
    }
};

// ─────────────────────────────────────────── 享元类：Flyweight ───────────────────────────────────────────

/**
 * @brief 享元对象：包含共享的内在状态
 */
class Flyweight {
private:
    std::shared_ptr<SharedState> shared_state;

public:
    // 构造函数（共享状态通过 shared_ptr 管理）
    explicit Flyweight(std::shared_ptr<SharedState> state) : shared_state(std::move(state)) {}

    // 显示完整信息（传入外在状态）
    void show(const std::string &owner, const std::string &plates) const {
        std::cout << "🚗 共享数据: " << *shared_state << std::endl;
        std::cout << "👤 专有数据: [owner: " << owner << ", plates: " << plates << "]" << std::endl;
    }

    // 获取共享状态（用于调试）
    const SharedState &getState() const {
        return *shared_state;
    }
};

// ─────────────────────────────────────────── 享元工厂：FlyweightFactory ───────────────────────────────────────────

/**
 * @brief 享元工厂：负责创建和管理享元对象
 */
class FlyweightFactory {
private:
    std::map<std::string, Flyweight> flyweights;

    // 生成 key
    static std::string getKey(const SharedState &ss) {
        return ss.getKey();
    }

public:
    // 构造函数：预加载初始享元
    FlyweightFactory(std::initializer_list<SharedState> states) {
        for (const auto &state: states) {
            std::string key = getKey(state);
            flyweights.emplace(key, Flyweight(std::make_shared<SharedState>(state)));
            std::cout << "📥 预加载车型: " << key << std::endl;
        }
    }

    // 获取享元（返回 const 引用，避免拷贝）
    const Flyweight &getFlyweight(const SharedState &sharedState) {
        std::string key = sharedState.getKey();

        if (flyweights.find(key) == flyweights.end()) {
            println("🔍 车库未找到该型号: {}, 正在入库...", key);
            flyweights.emplace(key, Flyweight(std::make_shared<SharedState>(sharedState)));
            println("✅ 入库成功: {}", key);
        } else {
            std::cout << "🟢 车库已存在: " << key << std::endl;
        }

        return flyweights.at(key);
    }

    // 列出所有缓存的享元
    void listFlyweights() const {
        std::cout << "📋 车库总信息 (共 " << flyweights.size() << " 辆):" << std::endl;
        for (const auto &[key, fw]: flyweights) {
            std::cout << "   → " << key << std::endl;
        }
    }
};

// ─────────────────────────────────────────── 工具函数：添加汽车 ───────────────────────────────────────────

/**
 * @brief 添加一辆车（演示享元使用）
 */
void addCar(FlyweightFactory &factory, const std::string &plates, const std::string &owner,
            const std::string &brand, const std::string &model, const std::string &color) {
    std::cout << std::endl
              << "📝 添加新车: " << owner << " (" << plates << ")" << std::endl;
    SharedState shared{brand, model, color};
    const Flyweight &flyweight = factory.getFlyweight(shared);
    flyweight.show(owner, plates);
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试享元模式
 */
void testFlyweight() {
    std::cout << "🎯 开始测试现代 C++20 享元模式" << std::endl;

    // 创建工厂并预加载常见车型
    FlyweightFactory factory({SharedState{"奥迪", "2023", "red"},
                              SharedState{"奔驰", "C43", "black"},
                              SharedState{"丰田", "AE86", "white"}});

    // 添加车辆（复用或创建新享元）
    addCar(factory, "京A-88888", "张三", "宝马", "M6", "blue");
    addCar(factory, "沪B-66666", "李四", "奔驰", "C43", "black");// 复用
    addCar(factory, "粤Z-99999", "王五", "奥迪", "2023", "red"); // 复用

    // 显示缓存状态
    factory.listFlyweights();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    testFlyweight();
    std::cout << std::endl
              << "✅ 所有测试完成！" << std::endl;
    return 0;
}