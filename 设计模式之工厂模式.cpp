//
// Created by Administrator on 2023/8/24.
//
/*
 * 工厂模式
 * 什么是工厂模式
 * 工厂方法模式同样属于类的创建型模式，又被称为多态工厂模式，工厂方法模式的意义是 定义一个创建产品对象的工厂接口，
 * 将实际创建工作推迟到子类中。核心工厂类不再负责产品的创建，这样核心类成为一个抽象工厂角色，仅负责具体工厂子类必须实现的接口，
 * 这样进一步抽象化的好处是使得工厂方法模式可以使系统在不在修改具体工厂角色的情况下引进新的产品
 *
 * 为什么使用工厂模式
 * 工厂方法模式与简单工厂模式在结构上的不同不是很明显，工厂方法类的核心是一个抽象工厂类，而简单工厂模式把核心放在一个具体类上。
 * 工厂方法模式之所以有一个别名叫多态性工厂模式是因为具体工厂类都有共同的接口，或者有共同的抽象父类。当系统扩展需要添加新的产品对象时，
 * 仅仅需要添加一个具体对象以及一个具体工厂对象，原有工厂对象不需要进行任何修改，也不需要修改客户端，很好的符合了“开放封闭”原则
 * 而简单工厂模式在添加新产品对象后不再修改工厂方法，扩展性不好，工厂方法模式退化后可以演变为简单工厂模式
 *
 * 工厂模式实现步骤
 * 1.提供一个抽象工厂类 ：所有具体工厂类的父类
 * 2.提供与产品对应的工厂类：负责实例化产品对象
 * 3.提供一个抽象产品类：所有产品的父类
 * 4.提供一个或多个产品类:工厂方法模式所创建的具体实例化对象
 *
 * 工厂模式优缺点
 * 优点
 * 需求改变时改动最小
 * 具体的创建实例过程与客户端分离
 * 缺点
 * 新增功能时，工程量稍大
 */
#include "iostream"

using namespace std;

//抽象产品类
class AbstractProduct {
public:
    virtual void makeProduct() = 0;
    virtual ~AbstractProduct(){};
};

//抽象工厂
class AbstractFactory {
public:
    virtual ~AbstractFactory(){};
    virtual AbstractProduct *createProduct() = 0;
};

//具体产品
class PlaneProduct : public AbstractProduct {
public:
    void makeProduct() {
        cout << "Here the plane come" << endl;
    }
};

class RocketProduct : public AbstractProduct {
    void makeProduct() {
        cout << "Here the rocket come" << endl;
    }
};

//具体工厂
class planeFactory : public AbstractFactory {
public:
    AbstractProduct *createProduct() {
        return new PlaneProduct;
    }
};

class RocketFactory : public AbstractFactory {
public:
    AbstractProduct *createProduct() {
        return new RocketProduct;
    }
};

void test(AbstractFactory*factory,AbstractProduct*product){
    factory->createProduct();
    product->makeProduct();
}

void test01() {
    AbstractFactory*factory=new planeFactory;
    AbstractProduct*product=factory->createProduct();
    product->makeProduct();

    factory=new RocketFactory;
    product=factory->createProduct();
    product->makeProduct();
}

int main() {
    test01();
}