//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是原型模式
 * 用原型实例指定创建对象的种类，并通过拷贝这些原型创造新的对象，简单理解就是“克隆指定对象”
 *
 * 为什么使用原型模式
 * 某些结构复杂的对象的创建工作中由于需求的变化，这些对象经常面临着剧烈的变化，但是他们却有一些比较稳定一致的接口，此时便可以使用原型模式
 *
 * 原型模式实现步骤
 * 1.提供一个抽象原型类，规定了具体原型对象必须实现的接口
 * 2.提供多个具体原型类，实现抽象原型中的clone()方法，它是可被复制的对象
 * 3.提供访问类，使用具体原型类中的clone()方法来赋值新的对象
 *
 * 原型模式优缺点
 * 优点
 * 如果创建新的对象比较复杂，可以利用原型模式简化对象的创建过程，同时也能够提高效率
 * 简化对象的创建，无需理会创建过程
 * 可以在程序运行时(对象属性发生了变化)获得一份内容相同的实例，他们之间不会互相干扰
 * 缺点
 * 每一个类都必须配备一个克隆方法，对于已有的没有克隆方法的类来说是致命的
 *
 * 创建型模式总结
 * 工厂模式:单个类的对象创建工作
 * 抽象工厂模式:多个类的对象创建工作
 * 单例模式：类的全局对象创建工作
 * 建造者模式：复杂类的对象创建工作
 * 原型模式：自身类的克隆工作
 */
#include "iostream"
#include "string"

using namespace std;

//抽象原型类
class Monkey {
public:
    Monkey() = default;

    virtual ~Monkey() = default;

    virtual Monkey *Clone() = 0;

    virtual void Play() = 0;
};

//具体原型类
class WuKong : public Monkey {
public:
    WuKong(string name) : m_name(name) {}

    WuKong(const WuKong &other) {
        m_name = other.m_name;//一旦拷贝对象中有指针并且做了内存申请则必须深拷贝 这里是简单的浅拷贝
    }

    Monkey *Clone() {
        //调用自身拷贝构造
        return new WuKong(*this);
    }

    void Play() {
        cout << "name:" << m_name << "在玩原神" << endl;
    }

private:
    string m_name;
};

void test01() {
    Monkey *monkey = new WuKong("黑神话悟空");
    Monkey *monkey1 = monkey->Clone();

    monkey->Play();
    monkey1->Play();

    delete monkey;
    delete monkey1;
}

int main() {
    test01();
}