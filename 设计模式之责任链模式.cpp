//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是责任链模式
 * 责任链模式是一种行为设计模式，允许你将请求沿着处理者链进行发送，收到请求后，每个处理者均可对请求进行处理，或将其传递给链上的下个处理者
 * 比如有三个项目经理，顺序是A->B->C,把所有的项目都先传递给A,A拿到所有的项目之后，把属于自己负责的项目留下来，把剩下的传递给B，
 * 然后B把属于自己的留下来，把不属于自己的再传递给C
 *
 * 为什么使用责任链模式
 * 该模式能将多个处理者连接成一条链，接收到请求后，它会“询问“每个处理者是否能对其进行处理，这样所有的处理者都有机会来处理请求
 * 无论以何种顺序将处理者连接成一条链，所有请求都会严格按照顺序通过链上的处理者。如果在处理者类中有对引用成员变量的设定方法，
 * 将能动态地插入和移除处理者，或者改变其顺序。总的来说链条式处理事情、工作流程化、消息处理流程化、事物流程化。
 *
 * 责任链模式实现步骤
 * 1.提供处理类的抽象父类，抽象客户端如何将请求数据传递给方法
 * 2.依次创建具体处理者子类并使用其处理方法
 *  是否自行处理这个请求
 *  是否将该请求沿着链进行传递
 * 3.客户端处理链(组装链+触发处理者)，客户端就需要准备好处理以下情况
 *  链中可能只有单个链接
 *  部分请求可能无法到达链尾
 *  其他请求可能直到链尾都未被处理
 *
 *  责任链模式优缺点
 *  优点
 *  可以控制请求处理的程序
 *  单一职责原则：可对发起操作和执行操作的类进行解耦
 *  开闭原则：可以在不更改先有代码的情况下在程序中新增处理者
 *  缺点
 *  部分请求可能未被处理
 */
#include "iostream"

using namespace std;

//抽象一个处理者类
class Handler {
public:
    Handler() : pnext(nullptr) {}

    //转接下一位
    void SetNextHandler(Handler *pnext) { this->pnext = pnext; }

    //具体请求
    virtual void HandlerRequest(int days) = 0;

protected:
    Handler *pnext;
};

class Director : public Handler {
public:
    void HandlerRequest(int days) {
        cout << "主管回复:" << endl;
        if (days <= 1) {
            cout << "主管同意请假" << endl;
        } else {
            cout << "请假太长，寻求经理" << endl;
            pnext->HandlerRequest(days);
        }
    }
};

class Manager : public Handler {
public:
    void HandlerRequest(int days) {
        cout << "经理回复:" << endl;
        if (days <= 3) {
            cout << "经理同意请假" << endl;
        } else {
            cout << "请假太长，寻求老板" << endl;
            pnext->HandlerRequest(days);
        }
    }
};

class Boss : public Handler {
public:
    void HandlerRequest(int days) {
        cout << "老板回复:" << endl;
        if (days <= 7) {
            cout << "老板同意请假" << endl;
        } else {
            cout << "请假太长，不予批准" << endl;
        }
    }
};


void test01() {
    //组装链
    Handler *director = new Director;
    Handler *manager = new Manager;
    Handler *boss = new Boss;
    director->SetNextHandler(manager);
    director->SetNextHandler(boss);

    cout << "请假1天结果" << endl;
    director->HandlerRequest(1);

    cout << "请假3天结果" << endl;
    director->HandlerRequest(3);

    cout << "请假10天结果" << endl;
    director->HandlerRequest(10);
}

int main() {
    test01();
}