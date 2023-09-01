//
// Created by Administrator on 2023/8/24.
//
/*
 * 简单工厂设计模式
 * 简单工厂模式属于类的创建型模式，又叫做静态工厂方法模式
 * 通过专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类
 * 为什么用简单工厂
 * 减少客户程序对类创建过程的依赖
 * 简单工厂实现步骤
 * 1.提供一个工厂类：负责实现创建所有实例的内部变量，可以被外界直接调用，创建所需的产品对象
 * 2.提供一个抽象产品类：简单工厂模式所创建的所有对象的父类，它负责描述所有实例锁共有的公共接口
 * 3.提供一个具体产品类：简单工厂模式所创建的具体实例对象
 *
 * 简单工厂优缺点
 * 优点
 * 帮助封装，实现组件封装，面向接口编程
 * 解耦合，客户端和具体实现类的耦合
 * 缺点
 * 可能增加客户端的复杂度
 * 不方便扩展子工厂
 */
#include "iostream"

using namespace std;

//抽象产品类
class Operation {
public:
    double m_leftValue;
    double m_rightValue;

    virtual double GetResult() = 0;
};

class AddOperation : public Operation {
    double GetResult() {
        return m_leftValue + m_rightValue;
    }
};

class SubOperation : public Operation {
    double GetResult() {
        return m_leftValue - m_rightValue;
    }
};

class MulOperation : public Operation {
    double GetResult() {
        return m_leftValue * m_rightValue;
    }
};

class DivOperation : public Operation {
    double GetResult() {
        return m_leftValue / m_rightValue;
    }
};

//工厂类
class OperatorFactory {
public:
    static Operation *creatOperation(char c) {
        switch (c) {
            case '+':
                return new AddOperation;
                break;
            case '-':
                return new SubOperation;
                break;
            case '*':
                return new MulOperation;
                break;
            case '/':
                return new DivOperation;
                break;
        }
    }
};

void test01() {
    Operation *add = OperatorFactory::creatOperation('+');
    add->m_leftValue = 1;
    add->m_rightValue = 2;
    cout << add->GetResult() << endl;
}

int main() {
    test01();
}