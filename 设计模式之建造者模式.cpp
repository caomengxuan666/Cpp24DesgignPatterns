//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是建造者模式
 * 建造者模式是一种对象的创建型模式之一，用来隐藏复合对象的创建过程，它把复合对象的创建过程加以抽象，通过子类继承和重载的方式，
 * 动态的创建具有复合属性的对象。官方说法就是将一个复杂对象的构造与它的表示分离，使同样的构建过程可以创建不同的表示
 *
 * 为什么使用建造者模式
 * 主要解决在软件系统中，有时候面临着一个复杂对象的创建工作，其通常由各个部分的子对象用一定的算法构成。由于需求的变化，
 * 这个复杂对象的各个部分经常面临着剧烈的变化，但是将它们组合在一起的算法却相对稳定
 *
 * 建造者模式实现步骤
 * 1.提供抽象建造者类 为创建产品各个部分，统一抽象接口
 * 2.提供具体建造者类 具体实现抽象建造者各个部件的接口
 * 3.提供多个具体产品类 具体的创建产品的各个部分
 * 4.提供一个指挥类 负责安排和调度复杂对象的各个建造过程
 *
 * 建造者模式优缺点
 * 优点
 * 封装性好，构建和表示分离
 * 扩展性好，各个具体的建造者相互独立，有利于系统的解耦
 * 控制细节风险，客户端无需详细细节，建造者细化创建过程
 *
 * 缺点
 * 产品的组成成分必须相同，这限制了使用范围
 * 产品内部发生变化，建造者需要同步修改，后期维护成本较大
 */
#include "iostream"
#include "string"
#include "vector"

using namespace std;

//抽象产品类
class AbstractProduct {
public:
    virtual void SetDisplay(string &displayer) = 0;

    virtual void SetMouse(string &mouse) = 0;

    virtual void SetKeyBoard(string &keyBoard) = 0;

    virtual void SetHost(string &host) = 0;

    virtual void show() = 0;
};

//具体产品类
class Computer : public AbstractProduct {
public:
    void SetDisplay(string &displayer) {
        m_list.push_back(displayer);
    }

    void SetMouse(string &mouse) {
        m_list.push_back(mouse);
    }

    void SetKeyBoard(string &keyBoard) {
        m_list.push_back(keyBoard);
    }

    void SetHost(string &host) {
        m_list.push_back(host);
    }

    void show() {
        cout << "当前电脑配置" << endl;
        for (auto &i: m_list) {
            cout << i << endl;
        }
    }

    vector<string> m_list;
};

//建造者类
class AbstractBuild {
public:
    AbstractBuild() : product(new Computer) {}

    //抽象建造过程
    virtual void BuildDisplay(string &displayer) = 0;

    virtual void BuildMouse(string &mouse) = 0;

    virtual void BuildKeyBoard(string &keyBoard) = 0;

    virtual void BuildHost(string &host) = 0;

    AbstractProduct *GetProduct() {
        return product;
    }

protected:
    AbstractProduct *product;
};

//具体的建造者
class ConcreteBuilder:public AbstractBuild{
public:
    void BuildDisplay(string &displayer){
        product->SetDisplay(displayer);
    }
    void BuildMouse(string &mouse){
        product->SetMouse(mouse);
    }
    void BuildKeyBoard(string &keyBoard){
        product->SetKeyBoard(keyBoard);
    }
    void BuildHost(string &host) {
        product->SetHost(host);
    }
};

//指挥者类
class Director{
public:
    Director(AbstractBuild*build):pBuilder(build){}

    AbstractProduct*create(string displayer,string mouse,string keyboard,string host){
        pBuilder->BuildDisplay(displayer);
        pBuilder->BuildMouse(mouse);
        pBuilder->BuildKeyBoard(keyboard);
        pBuilder->BuildHost(host);
        return pBuilder->GetProduct();
    }
private:
    AbstractBuild*pBuilder;
};

void test01() {
    AbstractBuild*pb=new ConcreteBuilder;
    Director*pd=new Director(pb);
    AbstractProduct*computer=pd->create("三星显示器","罗技鼠标","艾石头键盘","拯救者主机");
    computer->show();
}

int main() {
    test01();
}