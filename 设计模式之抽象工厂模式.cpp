//
// Created by Administrator on 2023/8/25.
//
/*
 * 什么是抽象工厂模式
 * 抽象工厂模式是所有形态的工厂模式中最为抽象和最一般性的，抽象工厂模式可以向客户端提供一个接口，使得客户端在不必指定产品的具体类型
 * 的情况下，能够创建多个产品族的对象。
 *
 * 为什么使用抽象工厂模式
 * 抽象工厂方法是针对与一个产品族，使得易于交换产品系列，只需改变具体的工厂就可以使用不同的产品配置，当一个族中的产品对象被设计成一起工作
 * 且一个应用只是用同一族的对象。例如设计系统生成不同风格的UI界面，按钮，边框等UI元素在一起使用，并且只能同属一种风格，这很容易用抽象工厂实现
 *
 * 抽象工厂模式实现步骤
 * 1.提供一个抽象工厂类：声明一组创建一族产品的工厂方法
 * 2.提供一个具体工厂类：实现了在抽象工厂创建产品的工厂方法
 * 3.提供一个抽象产品类：抽象产品中声明了产品具有的业务方法
 * 4.提供一个具体产品类：实现抽象产品接口中声明的业务方法
 *
 * 抽象工厂模式优缺点
 * 优点
 * 抽象工厂封装了变化，封装了对象创建的具体细节
 * 增加新的产品族很方便，无需修改已有系统
 * 针对接口进行编程而不是针对具体进行编程
 * 缺点
 * 增加新的产品等级结构需对原系统做较大修改(违背开放封闭的设计原则)
 */
#include "iostream"

using namespace std;

//抽象产品
class Product {
public:
    virtual void show() = 0;
};

//抽象产品族1
class keyboard : public Product {
};

//具体产品
class LogiKeyboard : public keyboard {
    void show() {
        cout << "罗技键盘" << endl;
    }
};

class RazerKeyboard : public keyboard {
    void show() {
        cout << "雷蛇键盘" << endl;
    }
};

//抽象产品组2
class Mouse : public Product {
};

class LogiMouse : public Mouse {
    void show() {
        cout << "罗技鼠标" << endl;
    }
};


class RazerMouse : public Mouse {
    void show() {
        cout << "雷蛇鼠标" << endl;
    }
};

//抽象工厂
class Factory {
public:
    virtual keyboard *CreateKeyBoard() = 0;

    virtual Mouse *CreateMouse() = 0;
};

//具体工厂
class LogiFactory : public Factory {
    keyboard *CreateKeyBoard() {
        return new LogiKeyboard;
    }

    Mouse *CreateMouse() {
        return new LogiMouse;
    }
};

class RazerFactory : public Factory {
    keyboard *CreateKeyBoard() {
        return new RazerKeyboard;
    }

    Mouse *CreateMouse() {
        return new RazerMouse;
    }
};

void test01() {
    Factory *factory = new LogiFactory;
    keyboard *keyboard = factory->CreateKeyBoard();
    Mouse *mouse = factory->CreateMouse();
    keyboard->show();
    mouse->show();
    delete factory;
    delete keyboard;
    delete mouse;
    factory = new RazerFactory;
    keyboard = factory->CreateKeyBoard();
    mouse = factory->CreateMouse();
    keyboard->show();
    mouse->show();
    delete factory;
    delete keyboard;
    delete mouse;
    factory = nullptr;
    keyboard = nullptr;
    mouse = nullptr;
}

int main() {
    test01();
}