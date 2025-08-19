// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃  MODULE   ::  Composite Pattern                                             ┃
// ┃  PURPOSE   ::  Compose objects into tree structures and treat them uniformly ┃
// ┃  CATEGORY  ::  Structural Pattern                                           ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

/**
 * @file    composite.cpp
 * @brief   现代 C++20 实现组合模式
 * @details 
 *          组合模式（Composite Pattern）是一种结构型设计模式，将对象组合成树形结构以表示“部分-整体”的层次结构。
 *          它使得客户端可以统一处理单个对象和组合对象，常用于文件系统、UI 组件树、组织结构等场景。
 * 
 * @par 核心思想
 *      - 所有节点实现统一接口（Component）
 *      - 叶子节点（Leaf）无子节点
 *      - 容器节点（Composite）包含子节点列表
 *      - 客户端无需区分叶子与容器
 * 
 * @note    本实现使用 std::unique_ptr 自动管理树结构
 * @warning 不要循环引用（否则内存泄漏）
 * @todo    支持路径查找、事件冒泡
 * @date    2025-08-19
 * @version 1.0
 */

#include <memory>
#include <print>
#include <string>
#include <vector>


using std::println;
// ─────────────────────────────────────────── 组件接口：Component ───────────────────────────────────────────

/**
 * @brief 组件接口：所有节点的公共接口
 */
class Component {
public:
    virtual ~Component() = default;

    /**
     * @brief 显示节点（含缩进）
     * @param depth 缩进层级
     */
    virtual void display(int depth = 0) const = 0;

    /**
     * @brief 添加子节点（默认不支持）
     * @param child 子节点
     * @throws std::runtime_error 叶子节点不支持添加
     */
    virtual void add(std::unique_ptr<Component> child) {
        throw std::runtime_error("Cannot add to a leaf component");
    }

    /**
     * @brief 获取子节点数量
     * @return size_t 子节点数
     */
    virtual size_t size() const {
        return 0;
    }

    /**
     * @brief 克隆对象
     * @return std::unique_ptr<Component> 克隆的新对象
     */
    virtual std::unique_ptr<Component> clone() const = 0;
};

// ─────────────────────────────────────────── 叶子节点：File ───────────────────────────────────────────

/**
 * @brief 文件节点（叶子）
 */
class File : public Component {
    std::string name;

public:
    explicit File(std::string n) : name(std::move(n)) {}

    void display(int depth = 0) const override {
        println("{}📄 {}", std::string(depth * 4, ' '), name);
    }

    std::unique_ptr<Component> clone() const override {
        return std::make_unique<File>(name);
    }
};

// ─────────────────────────────────────────── 容器节点：Folder ───────────────────────────────────────────

/**
 * @brief 文件夹节点（容器）
 */
class Folder : public Component {
    std::string name;
    std::vector<std::unique_ptr<Component>> children;

public:
    explicit Folder(std::string n) : name(std::move(n)) {}

    void display(int depth = 0) const override {
        println("{}📁 {}", std::string(depth * 4, ' '), name);
        for (const auto &child: children) {
            child->display(depth + 1);
        }
    }

    void add(std::unique_ptr<Component> child) override {
        children.push_back(std::move(child));
    }

    size_t size() const override {
        return children.size();
    }

    std::unique_ptr<Component> clone() const override {
        auto newFolder = std::make_unique<Folder>(name);
        for (const auto &child: children) {
            newFolder->add(child->clone());
        }
        return newFolder;
    }
};

// ─────────────────────────────────────────── 流式构建器（Fluent Builder） ───────────────────────────────────────────

/**
 * @brief 目录树构建器
 * @note 支持链式调用创建复杂树结构
 */
class TreeBuilder {
    std::unique_ptr<Component> root;

public:
    explicit TreeBuilder(std::unique_ptr<Component> r) : root(std::move(r)) {}

    /**
     * @brief 添加子节点
     * @param child 子节点
     * @return TreeBuilder& 支持链式调用
     */
    TreeBuilder &add(std::unique_ptr<Component> child) {
        if (auto *folder = dynamic_cast<Folder *>(root.get())) {
            folder->add(std::move(child));
        } else {
            throw std::runtime_error("Cannot add to non-folder root");
        }
        return *this;
    }

    /**
     * @brief 构建完成
     * @return std::unique_ptr<Component>
     */
    std::unique_ptr<Component> build() {
        return std::move(root);
    }
};

// ─────────────────────────────────────────── 工厂函数简化创建 ───────────────────────────────────────────

std::unique_ptr<File> file(std::string name) {
    return std::make_unique<File>(std::move(name));
}

std::unique_ptr<Folder> folder(std::string name) {
    return std::make_unique<Folder>(std::move(name));
}

// ─────────────────────────────────────────── 测试函数 ───────────────────────────────────────────

/**
 * @brief 测试基本组合功能
 */
void testBasicComposite() {
    println("\n🧪 测试 1: 基本组合模式");

    auto root = folder("C:");
    auto dir1 = folder("dir1");
    auto dir2 = folder("dir2");
    auto t1 = file("t1.txt");
    auto t2 = file("t2.txt");

    dir1->add(t1->clone());
    dir2->add(t2->clone());
    dir2->add(t1->clone());

    root->add(std::move(dir1));
    root->add(std::move(dir2));

    root->display();
}

/**
 * @brief 测试流式构建
 */
void testFluentBuilder() {
    println("\n🧪 测试 2: 流式构建器");

    auto tree = TreeBuilder(folder("Project"))
                        .add(file("main.cpp"))
                        .add(file("CMakeLists.txt"))
                        .add(TreeBuilder(folder("src"))
                                     .add(file("utils.cpp"))
                                     .add(file("helper.cpp"))
                                     .build())
                        .add(TreeBuilder(folder("include"))
                                     .add(file("utils.h"))
                                     .build())
                        .build();

    tree->display();
}

/**
 * @brief 测试运行时动态修改
 */
void testDynamicModification() {
    println("\n🧪 测试 3: 运行时动态修改");

    auto root = folder("Root");
    root->add(file("a.txt"));
    root->display();

    println("🔄 添加新文件...");
    root->add(file("b.txt"));
    root->display();
}

// ─────────────────────────────────────────── 主函数 ───────────────────────────────────────────

/**
 * @brief 主函数
 * @return int
 */
int main() {
    println("🎯 开始测试现代 C++20 组合模式");

    testBasicComposite();
    testFluentBuilder();
    testDynamicModification();

    println("\n✅ 所有测试完成！");
    return 0;
}