//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是中介者模式
 * 中介者模式是一种行为设计模式，能减少对象之间混乱无序的依赖关系。该模式会限制对象之间的相互交互，迫使它们通过一个中介者对象进行合作
 *
 * 为什么使用中介者模式
 * 该模式可以将对象间的所有关系抽取为一个单独的类，以使对于特定组件的修改工作独立于其他组件，由于所有组件间关系都被包含在中介者中
 * 因此无需修改组件就能方便地新建中介者类以定义新的组件合作方式。一般来说，大型的软件在开发的过程中会使用模块化的开发方式
 * 但是无论模块直接如何独立，总是会有需要模块间通信的时候，如果模块有很多，且每个模块都向外提供了唯一的接收消息的接口，
 * 那么一个模块如果需要与其他所有模块通讯，则需要记录其他所有模块的通信接口。同理每个模块都要计入所有与自己相关联的模块的通信接口，
 * 尤其是后者，在公司项目开发过程中是很寻常的，那么这种情况下所有模块关于其他模块通信接口的记录可能都需要修改或增加，
 * 这在模块数量庞大的项目中，会产生极大的工作量，所以此时可以考虑中介者模式
 *
 * 中介者模式实现步骤
 * 1.抽象一个中介者类：定义对象到中介者对象中的接口
 * 2.提供一个具体中介者类：实现抽象接口，并且实现关联类数据处理
 * 3.提供一个关联类（同事类）的抽象父类：多个关联类之间的交互操作的抽象
 * 4.提供一个或多个具体的关联类(同事类)：实现抽象，通过中介者类完成操作
 *
 * 中介者模式优缺点
 * 优点
 * 单一职责原则：可以将多个组件的交流抽取到同一位置，使其更容易理解和维护
 * 开闭原则：无需修改实际组件就能增加新的中介者
 * 可以减轻应用中多个组件的耦合情况
 * 可以更方便地复用各个组件
 * 缺点
 * 一段时间后，中介者可能会演化为上帝对象
 */
#include "iostream"
#include "string"

using namespace std;

//转发协议
enum class Message {
    ATW_MESSAGE,    //App-->Win
    ATM_MESSAGE,    //App-->Mac
    WTM_MESSAGE     //Win-->Mac
};

class AbstractModule;

//提供转发接口父类
class AbstractMediator {
public:
    virtual void Transmit(Message message, AbstractModule *pm) = 0;
};

//模块的父类
class AbstractModule {
public:
    AbstractModule(AbstractMediator *pm) : pm(pm) {}

    void SendMessage(Message message) {
        pm->Transmit(message, this);
    }

    virtual void AcceptMessage(Message message) = 0;

protected:
    AbstractMediator *pm;
};

class App : public AbstractModule {
public:
    App(AbstractMediator *pM) : AbstractModule(pM) {}

    void AcceptMessage(Message message) {
        switch (message) {
            case Message::ATM_MESSAGE:
                cout << "App to Mac" << endl;
                break;
            case Message::ATW_MESSAGE:
                cout << "App to Windows" << endl;
                break;
        }
    }
};

class Windows : public AbstractModule {
public:
    Windows(AbstractMediator *pM) : AbstractModule(pM) {}

    void AcceptMessage(Message message) {
        switch (message) {
            case Message::ATW_MESSAGE:
                cout << "App to Windows" << endl;
                break;
            case Message::WTM_MESSAGE:
                cout << "Windows to Mac" << endl;
                break;
        }
    }
};

class Mac : public AbstractModule {
public:
    Mac(AbstractMediator *pM) : AbstractModule(pM) {}

    void AcceptMessage(Message message) {
        switch (message) {
            case Message::ATM_MESSAGE:
                cout << "App to Mac" << endl;
                break;
            case Message::WTM_MESSAGE:
                cout << "Windows to Mac" << endl;
                break;
        }
    }
};

//中介者类实现
class ConcreteMediator : public AbstractMediator {
public:
    //为中介者设定模块
    void SetModuleApp(AbstractModule *app) {
        this->app = app;
    }

    void SetModuleWin(AbstractModule *win) {
        this->win = win;
    }

    void SetModuleMac(AbstractModule *mac) {
        this->mac = mac;
    }

    //转发
    void Transmit(Message message, AbstractModule *pM) {
        switch (message) {
            case Message::ATM_MESSAGE: {
                //通过dynamic_cast转换确认指针指向的子类类型
                App *app = dynamic_cast<App *>(pM);
                if (app != nullptr) {
                    mac->AcceptMessage(message);
                } else {
                    this->app->AcceptMessage(message);
                }
                break;
            }
            case Message::ATW_MESSAGE: {
                App *app = dynamic_cast<App *>(pM);
                if (app != nullptr) {
                    win->AcceptMessage(message);
                } else {
                    this->app->AcceptMessage(message);
                }
                break;
            }

            case Message::WTM_MESSAGE: {
                Windows *win = dynamic_cast<Windows *>(pM);
                if (win != nullptr) {
                    mac->AcceptMessage(message);
                } else {
                    this->win->AcceptMessage(message);
                }
                break;
            }
        }
    }

protected:
    AbstractModule *app = nullptr;
    AbstractModule *win = nullptr;
    AbstractModule *mac = nullptr;
};

void test01() {
    AbstractMediator *pM = new ConcreteMediator;
    //指定中介者
    AbstractModule *app = new App(pM);
    AbstractModule *win = new Windows(pM);
    AbstractModule *mac = new Mac(pM);


    //为中介者设定模块
    ConcreteMediator *pc = dynamic_cast<ConcreteMediator *>(pM);
    pc->SetModuleApp(app);
    pc->SetModuleWin(win);
    pc->SetModuleMac(mac);

    //不同模块之间进行通信
    app->SendMessage(Message::ATM_MESSAGE);
    app->SendMessage(Message::ATW_MESSAGE);
    win->SendMessage(Message::WTM_MESSAGE);
}

int main() {
    test01();
}