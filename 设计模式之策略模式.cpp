//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是策略模式
 * 策略模式是一种行为设计模式，可以定义一系列算法，并将每种算法分别独立放入独立的类中，以使算法的对象能够互相替换
 *
 * 为什么使用策略模式
 * 策略模式可以将对象关联至可以不同方式执行特定子任务的不同子对象，从而以间接方式在运行时更改对象行为。策略模式可以将不同
 * 行为抽取到一个独立类层次结构中，并且将原始类组合成同一个，从而减少重复代码。策略模式可以将各种算法的代码，内部数据和依赖关系
 * 与其他的代码隔离开来。不同客户端可以通过一个简单接口执行算法，并能在运行时进行切换。策略模式将所有继承自同样接口的算法
 * 抽取到独立类中，因此不再需要条件语句。原始对象并不实现所有算法的变体，而是将执行工作委派给其中的一个独立算法对象。
 * 从而解决在同一算法的不同变体中切换时避免使用多重条件判断
 *
 * 策略模式实现步骤
 * 1.提供一个抽象策略类：定义抽象的函数算法让继承的子类实现
 * 2.提供具体策略类：封装了继续相关的算法和行为，即函数的具体功能的实现
 * 3.提供一个策略的容器类：根据不同策略执行不同的行为，策略由外部环境决定
 *
 * 策略模式优缺点
 * 优点
 * 可以在运行时切换对象内的算法
 * 你可以将算法的实现和算法的代码隔离开来
 * 可以使用组合来代替继承
 * 开闭原则：无序对上下文进行修改就能引入新的策略
 * 缺点
 * 如果你的算法极少发生改变，那么没有任何理由引入新的类和接口，使用该模式只会让程序过于复杂
 * 客户端必须知晓策略间的不同，因为它需要选择合适的策略
 * 借助额外的类和接口可能会让代码不够简洁
 */
#include "iostream"

using namespace std;

class Strategy {
public:
    virtual int Execute(int left, int right) = 0;
};

class Add : public Strategy {
public:
    int Execute(int left, int right) {
        return left + right;
    }
};

class Sub : public Strategy {
public:
    int Execute(int left, int right) {
        return left - right;
    }
};

class Mul : public Strategy {
public:
    int Execute(int left, int right) {
        return left * right;
    }
};

class Div : public Strategy {
public:
    int Execute(int left, int right) {
        if (right == 0) {
            cout << "除数不能为0" << endl;
            return 0;
        }
        return left / right;
    }
};

//策略容器类
class Context {
public:
    //设置使用算法方式
    void SetStrategy(Strategy *ps) {
        this->ps = ps;
    }

    int ExecuteStrategy(int left, int right) {
        return ps->Execute(left, right);
    }

private:
    Strategy *ps;
};

void test01() {
    Context *pcontext = new Context;
    int left, right;
    char key;
    while (1) {
        cin >> left >> key >> right;
        switch (key) {
            case '+':
                pcontext->SetStrategy(new Add);
                break;
            case '-':
                pcontext->SetStrategy(new Sub);
                break;
            case '*':
                pcontext->SetStrategy(new Mul);
                break;
            case '/':
                pcontext->SetStrategy(new Div);
                break;
        }
        cout << pcontext->ExecuteStrategy(left, right) << endl;
    }
}

int main() {
    test01();
}