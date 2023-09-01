//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是代理模式
 * 代理模式是构造型的设计模式之一，它可以为其他对象提供一种代理以控制对这个对象的访问。所谓代理，是指具有与代理元（被代理的对象)具有相同的接口的类
 * 客户端必须通过代理与被代理的目标交互，而代理在一半的交互过程中(交互前后)进行某些特别的处理
 *
 * 为什么使用代理模式
 * 在目标对象实现的基础上，增强额外的功能操作，即扩展目标对象的功能
 * 代理模式的作用:AOP实现，拦截器，中介，黄牛，解耦，专人做专事
 * AOP:面向切面编程，通过预编译方式和运行期间动态代理实现程序功能的统一维护的一种技术
 *
 * 代理模式实现步骤
 * 1.提供一个抽象主题角色:真实主题与代理主题的共同接口
 * 2.提供一个真实主题角色:定义了代理角色所代表的真实对象
 * 3.提供一个代理主题角色:含有对真实主题角色的引用
 *
 * 代理模式优缺点
 * 优点
 * 职责清晰：真实角色就是实现实际业务逻辑，不关心其他非本职责事务，通过后期代理完成一件事物，附带结果就是编程简介清晰
 * 高扩展性，具体主题角色可变
 * 缺点
 * 代理模式可能会造成请求的处理速度变慢
 */
#include "iostream"
#include "string"

using namespace std;

class AbstractSubject {
public:
    virtual void PurchaseTicket() = 0;
};

class User : public AbstractSubject {
public:
    void PurchaseTicket() {
        cout << "用户买票" << endl;
    }
};

class Ctrip : public AbstractSubject {
public:
    Ctrip(AbstractSubject*base):pBase(base){}
    void PurchaseTicket() {
        cout << "携程购票" << endl;
    }

private:
    AbstractSubject *pBase;
};

void test01() {
    AbstractSubject*pBase=new User;
    pBase->PurchaseTicket();

    Ctrip*proxy=new Ctrip(pBase);
    proxy->PurchaseTicket();
}

int main() {
    test01();
}