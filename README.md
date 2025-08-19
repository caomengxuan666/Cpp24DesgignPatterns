# Cpp24DesignPatterns

> 🎯 用现代 C++20 实现 24 种经典设计模式  
> 📦 项目结构清晰，支持自动化构建与文档生成  
> 🧩 每个模式独立可运行，注释完整，适合学习与工程实践  
> 📚 支持 Doxygen 自动生成 HTML 文档  
> 🚀 零裸指针、零内存泄漏、RAII + 智能指针

---

## 🧱 创建型模式（Creational Patterns）

创建型模式关注 **对象的创建机制**，帮助系统独立于对象的创建、组合和表示方式。它们通过抽象化实例化过程，使代码更具灵活性和可扩展性。

| 模式 | 说明 | 现代 C++ 实现要点 |
|------|------|------------------|
| **Singleton** | 确保一个类只有一个实例，并提供全局访问点 | Meyers' Singleton + CRTP 基类 |
| **Simple Factory** | 通过工厂类创建不同产品，客户端无需知道具体类 | `std::map<char, std::function<unique_ptr<T>()>>` |
| **Factory Method** | 将对象创建延迟到子类 | 模板工厂 `SimpleFactory<T>` |
| **Abstract Factory** | 创建一组相关或依赖对象的家族 | 模板族工厂 `SimpleDeviceFactory<K, M>` |
| **Builder** | 将复杂对象的构建与表示分离 | 流式接口（Fluent Interface）+ 移动语义 |
| **Prototype** | 通过克隆现有对象创建新对象 | `clone()` + `std::unique_ptr` + CRTP |

---

## 🔗 结构型模式（Structural Patterns）

结构型模式关注 **如何组合类和对象以获得更大的结构**。它们通过继承或组合来设计更灵活的结构。

| 模式 | 说明 | 现代 C++ 实现要点 |
|------|------|------------------|
| **Adapter** | 将一个类的接口转换成客户端期望的另一个接口 | 对象适配器 + 智能指针 |
| **Bridge** | 分离抽象与实现，使两者独立变化 | 组合 + 依赖注入 |
| **Composite** | 将对象组合成树形结构以表示“部分-整体”层次 | `Component` 接口 + 递归遍历 |
| **Decorator** | 动态地为对象添加职责 | 组合 + 多层包装 |
| **Facade** | 为复杂子系统提供一个简单接口 | 门面类封装多步骤流程 |
| **Flyweight** | 通过共享技术支持大量细粒度对象 | 内在状态共享 + 外在状态外部化 |
| **Proxy** | 控制对对象的访问 | 代理链 + 权限/日志增强 |

---

## 🔄 行为型模式（Behavioral Patterns）

行为型模式关注 **对象之间的职责分配与通信机制**。它们帮助对象在运行时动态地交互。

| 模式 | 说明 | 现代 C++ 实现要点 |
|------|------|------------------|
| **Observer** | 定义对象间的一对多依赖关系 | 事件总线 + `std::function` + RAII 注销 |
| **Strategy** | 定义一系列算法并封装 | `std::function` + 策略注入 + 函数式风格 |
| **Command** | 将请求封装为对象 | 命令队列 + 撤销/重做 + 智能指针 |
| **State** | 允许对象在其内部状态改变时改变行为 | `std::unique_ptr<State>` + enter/exit 回调 |
| **Template Method** | 定义算法骨架，子类重写步骤 | 非虚接口（NVI） + 钩子方法 |
| **Iterator** | 提供一种方式遍历聚合对象 | 自定义迭代器 + 支持范围 `for` 循环 |
| **Chain of Responsibility** | 将请求沿链传递 | 流式构建器 + 撤销栈 |
| **Mediator** | 用中介对象封装对象间的交互 | 事件总线 + 发布-订阅 + `std::weak_ptr` |
| **Memento** | 捕获并外部化对象的内部状态 | `std::unique_ptr<Memento>` + undo/redo 栈 |
| **Visitor** | 在不修改类的前提下添加新操作 | 双分派 + `std::variant` 替代方案 |
| **Interpreter** | 定义语言的文法表示 | 递归下降解析器 + AST + 异常安全 |

---

## 🔧 高级惯用法（Advanced Idioms）

这些是 C++ 工程师的“内功心法”，用于构建高性能、可维护的现代 C++ 库。

| 模式 | 说明 | 现代 C++ 实现要点 |
|------|------|------------------|
| **Pimpl** | 隐藏实现细节，减少编译依赖 | `std::unique_ptr<Impl>` + 多种实现方式 |
| **Named Constructor** | 提供语义化构造方式 | 静态工厂函数 + 可读性提升 |
| **CRTP** | 静态多态，零成本抽象 | 奇异递归模板 + 混入（Mixin） |
| **SFINAE** | 条件编译，模板重载控制 | `std::enable_if` + `void_t` 技巧 |
| **Rule of Zero** | 依赖 RAII，避免手动资源管理 | 智能指针 + 默认特殊成员函数 |
| **Policy-Based Design** | 策略式设计，行为可组合 | 模板参数注入行为 |
| **C++20 Concepts** | 类型约束，替代 SFINAE | `template<Arithmetic T>` |
| **C++20 Coroutines** | 协程，异步编程新范式 | `generator<T>` + `co_yield` |
| **Three-way Comparison** | 三路比较运算符 | `<=>` 替代手动实现 `==`, `<` 等 |

---

## 🛠 构建与运行

```bash
# 1. 创建构建目录
mkdir build && cd build

# 2. 配置项目（使用 Ninja）
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug

# 3. 构建所有模式
cmake --build .

# 4. 运行某个模式
./singleton
./factory_method
./builder
./observer
./strategy
./visitor
# ...
```