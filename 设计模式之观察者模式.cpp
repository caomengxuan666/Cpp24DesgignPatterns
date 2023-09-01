//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是观察者模式
 * 观察者模式是行为模式之一，它的作用是当一个对象的状态发生变化时，能够自动通知其他关联对象，自动刷新对象状态
 *
 * 为什么使用观察者模式
 * 当一个状态的改变需要改变其他对象，或实际对象是事先位置的或动态变化的时，观察者模式适合描述此类问题
 * 当应用中的一些对象必须观察其他对象时，可以使用该模式，但仅能在有限时间或特定情况下使用
 *
 * 观察者模式实现步骤
 * 1.提供一个抽象目标类：被观察的对象，抽象的基本属性及其他操作
 * 2.提供一个具体目标类：被观察的对象对象具体实现
 * 3.提供一个抽象观察者类：抽象观察者的具体的业务逻辑
 * 4.提供一个具体观察者类：观察者的具体实现，得到通知后将完成一些具体的业务逻辑处理
 *
 * 观察者模式优缺点
 * 优点
 * 开闭原则：无需修改发布者代码就能引入新的订阅者类(如果是发布者接口则可以轻松引入发布者类)
 * 可以在运行时建立对象之间的联系
 * 依赖倒置：让耦合的双方都依赖于抽象，而不是依赖于具体
 * 缺点
 * 被观察者对象有很多直接和间接的观察者的话，将所有的观察者都通知会花费很多时间
 */
#include "iostream"
#include "string"
#include "vector"

using namespace std;

class Boss;


class Staff {
public:
    Staff(string name, Boss *pBoss) : name(name), pBoss(pBoss) {}

    void Update(string action) {
        cout << name << "收到讯息" << action << endl;
        if (action == "老板来了") {
            cout << "---->开启老板来了模式" << endl;
        }
        if (action == "老板走了") {
            cout << "---->原神，启动" << endl;
        }
    }

private:
    string name;
    Boss *pBoss;
};

class Boss {
public:
    void AddObserver(Staff *pstaff) { staff.push_back(pstaff); }

    void SetAction(string action) {
        this->action = action;
        Notify(action);
    }

    void Notify(string action) {
        for (auto &v: staff) {
            v->Update(action);
        }
    }

private:
    string action;
    vector<Staff *> staff;
};

void test01() {
    //创建被观察者
    Boss *pBoss = new Boss;
    //创建观察者
    Staff *p1 = new Staff("oi", pBoss);
    Staff *p2 = new Staff("O.O", pBoss);
    //加入到观察者中
    pBoss->AddObserver(p1);
    pBoss->AddObserver(p2);

    pBoss->SetAction("老板来了");
    pBoss->SetAction("老板走了");
}

int main() {
    test01();
}





