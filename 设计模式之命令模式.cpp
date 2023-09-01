//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是命令模式
 * 命令模式是一种行为设计模式，它可以将请求转换为一个包含与请求相关的所有信息的独立对象。
 * 该转换让我们可以根据不同的请求将方法参数化、延迟请求执行或者将其放入队列中，且能实现可撤销操作
 *
 * 为什么使用命令模式
 * 在面向对象的程序设计中，一个对象调用另一个对象，整个调用过程比较繁杂，或者存在多处这种调用。这时，使用命令模式对该调用加以封装，
 * 便于功能的再利用。命令模式可将特定的方法调用转化为独立对象。这一改变也带来了许多有趣的应用：可以将命令作为方法的参数进行传递，
 * 将命令保存在其他对象中，或者在运行时切换已连接的命令等。同其他对象一样，命令也可以实现序列化(序列化就是转化为字符串)，从而能方便
 * 地写入文件或数据库中。一段时间后，该字符串可被恢复成最初的命令对象。因此，可以延迟或计划命令的执行。功能还有很多，
 * 比如说可以使用同样的方式，将命令放入队列、记录命令或者通过网络发送命令
 *
 * 命令模式实现步骤
 * 1.声明一个仅有一个执行方法的命令抽象类
 * 2.抽取请求并使之成为实现命令接口的具体命令类
 * 3.找到担任发送者职责的类
 * 4.修改发送者使其执行命令，而非直接将请求发送给接收者
 * 5.客户端必须按照以下顺序来初始化对象
 * 创建接收者
 * 创建命令，如有需要可将其关联至接收者
 * 创建发送者并将其与特定命令关联
 *
 * 命令模式优缺点
 * 优点
 * 单一职责原则：可以解耦触发和执行操作的类
 * 开闭原则：可以在不修改已有客户端代码的情况下在程序中创建新的命令
 * 可以实现撤销和恢复功能
 * 可以实现操作的延迟执行
 * 可以将一组简单命令组合成一个复杂命令
 * 缺点
 * 代码可能会变得更加复杂，因为在发送者和接收者之间增加了一个全新的层次
 */
#include "iostream"
#include "vector"
#include "string"

using namespace std;

class Cook {
public:
    void MakeVegetable() { cout << "蔬菜沙拉" << endl; }

    void MakeSteak() { cout << "牛排" << endl; }

    void UnVegetable() { cout << "撤销蔬菜沙拉" << endl; }

    void UnSteak() { cout << "撤销牛排" << endl; }
};

//抽象命令类
class Command {
public:
    Command(Cook *pccok = nullptr) : pcook(pccok) {}

    virtual void ExecuteCommand() = 0;

    virtual void UnCommand() = 0;

protected:
    Cook *pcook;
};

//具体命令类
class MakeVegetableCommand : public Command {
public:
    MakeVegetableCommand(Cook *pcook) : Command(pcook) {}

    void ExecuteCommand() {
        pcook->MakeVegetable();
    }

    void UnCommand() {
        pcook->UnVegetable();
    }
};

class MakeSteakCommand : public Command {
public:
    MakeSteakCommand(Cook *pcook) : Command(pcook) {}

    void ExecuteCommand() {
        pcook->MakeSteak();
    }

    void UnCommand() {
        pcook->UnSteak();
    }
};

//客户点菜
class Order {
public:
    void setOrder(Command *command) {
        commandQueue.push_back(command);
        cout << "点了一道菜:" << endl;
    }

    void UnOrder() {
        commandQueue.pop_back();
        cout << "退了一道菜" << endl;
    }

    void Notify(){
        for(auto&v:commandQueue){
            v->ExecuteCommand();
        }
    }

private:
    vector<Command *> commandQueue;
};

void test01() {
    //生成厨师，点菜，订单对象
    Cook*pcook=new Cook;
    Command*pvegettable=new MakeVegetableCommand(pcook);
    Command*psteak=new MakeSteakCommand(pcook);
    Order*porder=new Order;

    //订单推送到命令队列
    porder->setOrder(pvegettable);
    porder->setOrder(psteak);

    porder->Notify();
    porder->UnOrder();
    porder->Notify();
    porder->setOrder(psteak);
    porder->Notify();
}

int main() {
    test01();
}