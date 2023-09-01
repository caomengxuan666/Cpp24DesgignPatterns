//
// Created by Administrator on 2023/9/1.
//
/*
 * 什么是状态模式
 * 状态模式是一种行为设计模式，让你能在一个对象的内部状态变化时改变其行为，使其看上去就像改变了自身所属的类一样
 *
 * 为什么使用状态模式
 * 模式建议将所有特定于状态的代码抽取到一组独立的类中。这样一来，可以在独立与其他状态的情况下添加新状态或修改已有状态
 * 从而减少维护成本，状态模式让我们能够生成状态类层次结构，通过将公用代码抽取到抽象基类中来减少重复
 *
 * 状态模式实现步骤
 * 1.提供一个上下文环境类：抽象一个客户程序所需要的接口，将与状态相关的操作系统委托给当前的具体状态类对象来处理
 * 2.提供一个抽象给状态类：定义一个接口以封装使用上下文环境的一个特定状态相关的行为
 * 3.提供一个具体状态类:实现抽象状态定义的接口
 *
 * 状态模式优缺点
 * 优点
 * 单一职责原则：将与特定状态相关的代码放在独立的类中
 * 开闭原则：无需修改已有状态类和上下文就能引入新状态
 * 通过消除臃肿的状态机条件语句简化上下文代码
 * 缺点
 * 如果状态机只有很少的几个状态，或者很少发生改变，那么应用该模式可能会显得小题大做
 */
#include "iostream"
#include "string"

using namespace std;

class Context;

class AbstractState {
public:
    virtual void Handle(Context *p) = 0;
};

class ConcreteStateNone : public AbstractState {
public:
    void Handle(Context *p) {
        cout << "没有......" << endl;
    }
};

class ConcreteStateExist : public AbstractState {
public:
    void Handle(Context *p) {
        cout << "有......" << endl;
    }
};

class Context {
public:
    Context(AbstractState *pstate) : pstate(pstate) {}

    void Request() {
        if (pstate) {
            pstate->Handle(this);
        }
    }

    void ChangeState(AbstractState *pstate) {
        this->pstate = pstate;
    }

private:
    AbstractState *pstate;
};

void test01() {
    AbstractState *pNo = new ConcreteStateNone;
    AbstractState *pEx = new ConcreteStateExist;
    Context*pc=new Context(pNo);
    pc->Request();
    pc->ChangeState(pEx);
    pc->Request();
}

int main() {
    test01();
}