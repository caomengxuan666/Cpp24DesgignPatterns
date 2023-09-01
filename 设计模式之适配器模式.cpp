//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是适配器模式
 * 适配器模式是将一个类的接口转换成客户端希望的另一个接口，适配器模式使得原本由于接口不兼容而不能一起工作的那些类可以一起工作
 * 好比各个国家标准家庭工作电压不一样，适配器就是在不兼容的东西之间搭建起一座桥梁，让而这能很好的兼容在一起工作
 *
 * 为什么使用适配器模式
 * 在软件开发中，有的时候系统的数据和行为都正确，但是接口不符合，我们应该考虑使用适配器模式，目的是使控制范围之外的一个原有
 * 对象与某个接口匹配。在开发一个模块的时候，有一个功能点实现起来比较费劲，但是，之前有一个项目实现了一样的功能点，但是
 * 现在这个模块的接口和之前那个模块的接口是不一样的。所以需要用到适配器模式将之前实现的功能点匹配进新的项目
 *
 * 适配器模式实现步骤
 * 1.适配器实现与其中一个先有对象兼容的接口
 * 2.现有对象可以使用该接口安全地调用适配器方法
 * 3.适配器方法被调用后将另一个对象兼容的格式和顺序将请求传递给对象
 *
 * 一般适配器模式分为以下两类
 * 对象适配器：适配器实现了其中一个对象的接口，并对另一个对象进行封装
 * 类适配器：适配器同时继承两个对象的接口
 *
 * 适配器模式的优缺点
 * 优点
 * 降低了去实现一个功能点的难度，可以对现有的类进行包装，就可以进行使用了
 * 提高了项目质量，现有的类一般都是经过测试的，使用了适配器模式之后，不需要对旧的类进行全面的覆盖测试
 * 总的来说，提升了效率，降低了成本
 * 缺点
 * 类适配器模式，由于多继承，可能会出现二义性
 * 对象适配器模式，如果过多使用适配器模式，会导致代码阅读难度增大
 */
#include "iostream"

using namespace std;

//电源插座、三插头、二插头的适配
class ThreeOutlet {
public:
    void ThreeOutletCharge() {
        cout << "开始三插头充电" << endl;
    }
};

class AbstractTwoOutlet {
public:
    virtual void TwoOutletCharge() = 0;
};

class TwoOutlet : public AbstractTwoOutlet {
public:
    void TwoOutletCharge() {
        cout << "开始两插头充电" << endl;
    }
};

//对于对象做适配
class OutletAdaptObject : public AbstractTwoOutlet {
public:
    OutletAdaptObject(ThreeOutlet *pOut) {
        this->pOut = pOut;
    }

    void TwoOutletCharge() {
        Convertor();
        pOut->ThreeOutletCharge();
    }

    void Convertor() {
        cout << "三插头转换为两插头" << endl;
    }

protected:
    ThreeOutlet *pOut;
};

//对于类做适配
class OutletAdaptClass : public AbstractTwoOutlet, public ThreeOutlet {
public:
    void TwoOutletCharge(){
        Convertor();
        ThreeOutletCharge();
    }
    void Convertor(){
        cout<<"三插头转换为两插头"<<endl;
    }
};

void test01() {
    ThreeOutlet *pThree = new ThreeOutlet;
    pThree->ThreeOutletCharge();
    AbstractTwoOutlet *pTwo = new OutletAdaptObject(pThree);
    pTwo->TwoOutletCharge();
}

void test02() {
    ThreeOutlet*pThree=new ThreeOutlet;
    pThree->ThreeOutletCharge();
    AbstractTwoOutlet*pTwo=new OutletAdaptClass;
    pTwo->TwoOutletCharge();
}

int main() {
    test01();
    test02();
}