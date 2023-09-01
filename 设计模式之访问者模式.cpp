//
// Created by Administrator on 2023/8/31.
//
/*
 * 什么是访问者模式
 * 访问者模式是行为模式之一，它分离对象的数据和行为，使用访问者模式，可以在不修改已有类的情况下，增加新的操作角色和职责
 *
 * 为什么使用访问者模式
 * 访问者模式通过在访问者对象中为多个目标类提供相同操作的变体，使我们能够在属于不同类的一组对象上执行同一操作
 * 访问者模式将所有非主要的行为抽取到一组访问类中，使得程序的主要类能更专注于主要的工作
 * 当某个行为仅在类层次结构中的一些类中有意义，而在其他类中没有意义时，可以将行为抽取到单独的访问者类中
 * 只需实现接收相关类的对象作为参数的访问者方法并将其他方法留空即可
 *
 * 访问者模式实现步骤
 * 1.提供一个抽象访问者类：声明了一个或者多个访问操作，形成所有的具体元素角色必须实现的抽象方法
 * 2.提供一个具体访问者类：实现抽象访问者角色所声明的抽象方法，也就是抽象访问者所声明的各个访问操作
 * 3.提供一个抽象被访问者类：声明一个接受操作，接受一个访问者对象作为一个变量的抽象方法
 * 4.提供一个或多个被访问者类：被访问者的抽象方法的具体实现
 * 5.提供一个结构对象类：可以遍历结构中的所有元素:如果有需要，可以提供元素接口，元素可设计存储到容器中
 *
 * 访问者模式优缺点
 * 优点
 * 开闭原则：可以引入在不同对象上执行的新行为，且无需对这些类做出修改
 * 单一职责原则：可以将同一行为的不同版本移到同一个类中
 * 访问者对象可以在与各种对象交互时收集一些有用的信息
 * 缺点
 * 每次在元素层次结构中添加或移除一个类时，你都要更新所有的访问者
 * 在访问者同某个元素进行交互时，它们可能没有访问元素私有成员变量和方法的必要权限
 */
#include "iostream"
#include "string"
#include "list"

using namespace std;

class ParkElement;

class Visitor {
public:
    virtual void Visit(ParkElement *park) = 0;

protected:
    string name;
};

//公园
class ParkElement {
public:
    virtual void Accept(Visitor *visitor) = 0;

    string GetAddr() { return addr; }

protected:
    string addr;
};

//树林
class Grove : public ParkElement {
public:
    Grove() { this->addr = "树林"; }

    void Accept(Visitor *visitor) {
        visitor->Visit(this);
    }
};

//湖
class Lake : public ParkElement {
public:
    Lake() { this->addr = "湖边"; }

    void Accept(Visitor *visitor) {
        visitor->Visit(this);
    }
};

class LWB : public Visitor {
public:
    LWB() { this->name = "lwb"; }

    void Visit(ParkElement *park) {
        cout << name << "在" << park->GetAddr() << "看美女" << endl;
    }
};

class Muscle : public Visitor {
public:
    Muscle() { this->name = "cmx"; }

    void Visit(ParkElement *park) {
        cout << name << "在" << park->GetAddr() << "欣赏风景" << endl;
    }
};

class Lovers : public Visitor {
public:
    Lovers() { this->name = "小情侣"; }

    void Visit(ParkElement *park) {
        cout << name << "在" << park->GetAddr() << "约会" << endl;
    }
};

//结构对象类:组合管理访问点
class Park:public ParkElement{
public:
    Park(){}
    void SetPark(ParkElement*park){
        parkList.push_back(park);
    }
    void Accept(Visitor*visitor){
        for(auto &i:parkList){
            i->Accept(visitor);
        }
    }
private:
    list<ParkElement*>parkList;
};

void test01() {
    cout << "直接访问" << endl;
    //访问者
    LWB *plwb = new LWB;
    Muscle *pmus = new Muscle;
    Lovers *plove = new Lovers;
    //访问对象
    Grove *pG = new Grove;
    Lake *pLake = new Lake;
    pLake->Accept(plwb);
    pG->Accept(plwb);

    pLake->Accept(pmus);
    pG->Accept(pmus);

    pLake->Accept(plove);
    pG->Accept(plove);
}
void test02(){
    cout<<"_____________"<<endl;
    cout<<"结构体类对象管理"<<endl;
    LWB *plwb = new LWB;
    Muscle *pmus = new Muscle;
    Lovers *plove = new Lovers;

    Grove *pG = new Grove;
    Lake *pLake = new Lake;

    Park*p=new Park;
    p->SetPark(pLake);
    p->SetPark(pG);
    p->Accept(plwb);
    p->Accept(pmus);
    p->Accept(plove);
}

int main() {
    test01();
    test02();
}