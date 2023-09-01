//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是装饰模式
 * 装饰模式又叫做包装模式，通过一种对客户端透明的方式来扩展对象的功能，是继承关系的一个替换方案。装饰模式就是把要添加的附加功能
 * 分别放在单独的类中，并让这个类包含它装饰的对象，当需要执行时，客户端就可以有选择地、按顺序地使用装饰功能包装对象
 *
 * 为什么使用装饰模式
 * 装饰模式可以动态的给一个对象添加一些额外的职责，就功能来说，此模式比生成子类更为灵活，通过子类继承的方式，但是如果后续无法继续增加功能的话
 * 便要继续继承先有的类，如此就会使得继承的层次越来越深，不利于代码的维护和可读性，故最好的方式便是通过装饰者模式实现
 *
 * 装饰模式实现步骤
 * 1.提供一个抽象组件类:抽象被装饰者的行为
 * 2.提供一个或多个具体组件类:被装饰者的行为具体实现
 * 3.提供一个抽象装饰器类:抽象组件指针与抽象组件一致接口
 * 4.提供一个具体的装饰器类，为具体组件附加责任
 *
 * 装饰模式优缺点
 * 优点
 * 易于扩展对象功能
 * 通过装饰类排列组合，可以创造出很多不同行为的组合
 * 缺点
 * 会出现更多的代码，更多的类，增加程序复杂性
 * 动态装饰时，多层装饰会更加复杂
 */

#include "iostream"

using namespace std;

class AbstractComponent {
public:
    virtual void Show() = 0;

    string pStr;
};

class Girl : public AbstractComponent {
public:
    Girl() {
        this->pStr = "naked girl";
    }

    void Show() {
        cout << this->pStr << endl;
    }
};

class AbstractDecorator : public AbstractComponent {
public:
    virtual void Show() = 0;

protected:
    AbstractComponent *pBase;
};

class Miniskirt : public AbstractDecorator {
public:
    Miniskirt(AbstractComponent *base) {
        this->pBase = base;
    }

    void Show() {
        this->pStr=pBase->pStr+" wear Miniskirt";
        cout<<this->pStr<<endl;
    }
};

class BlackSilk:public AbstractDecorator{
public:
    BlackSilk(AbstractComponent*base){
        this->pBase=base;
    }
    void Show(){
        this->pStr=this->pBase->pStr+" wear BlackSilk";
        cout<<this->pStr<<endl;
    }
};

void test01() {
    Girl*pg=new Girl;
    pg->Show();

    Miniskirt*pm=new Miniskirt(pg);
    pm->Show();

    BlackSilk*pb=new BlackSilk(pm);
    pb->Show();

    BlackSilk*pb2=new BlackSilk(pg);
    pb2->Show();
}

int main() {
    test01();
}