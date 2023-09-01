//
// Created by Administrator on 2023/8/30.
//
/*
 * 什么是模板模式
 * 模板方法是一种行为设计模式，它在基类中定义了一个算法的框架，允许子类在不修改结构的情况下重写算法的特定步骤
 *
 * 为什么使用模板模式
 * 在抽象类中统一操作步骤，并规定好接口，让子类实现接口。这样可以把各个具体的子类和操作步骤解耦合。
 * 我们只希望客户端扩展某个特定算法步骤，而不是整个算法或其结构时，可使用模板方法模式。
 * 当多个类的算法除一些细微不同之外几乎完全一样时，可以使用该模式
 * 但是后果是，只要算法发生变化，可能就需要修改所有的类
 *
 * 模板模式实现步骤
 * 1.提供一个抽象类：负责规定好接口，让子类实现接口，定义一个模板方法
 * 2.提供具体实现子类：实现抽象类的抽象接口即可
 *
 * 模板模式优缺点
 * 优点
 * 可仅允许客户端重写一个大型算法中的特定部分，使得算法其他部分的修改对其所造成的影响最小
 * 可将重复代码提取到一个超类中
 * 缺点
 * 部分客户端可能会受到算法框架的限制
 * 通过子类抑制默认步骤实现可能会导致违反里氏替换原则
 * 模板方法中的步骤越多，其维护工作可能就会越困难
 */
#include "iostream"

using namespace std;

class MakeObject {
public:
    virtual void MakeHead() = 0;

    virtual void MakeBody() = 0;

    virtual void Garb() = 0;

    void Make() {
        MakeHead();
        MakeBody();
        Garb();
    };
};

class MakeGirlFriend : public MakeObject {
public:
    void MakeHead() {
        cout << "阿狸的头" << endl;
    }

    void MakeBody() {
        cout << "卡莎的身材" << endl;
    }

    void Garb() {
        cout << "健身服" << endl;
    }
};

class MakeBoyFriend : public MakeObject {
public:
    void MakeHead() {
        cout << "暮光之眼的头" << endl;
    }

    void MakeBody() {
        cout << "慎的身材" << endl;
    }

    void Garb() {
        cout << "腰子的衣服" << endl;
    }
};

void test01() {
    MakeObject*p=new MakeGirlFriend;
    p->Make();
    delete p;
    p=new MakeBoyFriend;
    p->Make();
}

int main() {
    test01();
}