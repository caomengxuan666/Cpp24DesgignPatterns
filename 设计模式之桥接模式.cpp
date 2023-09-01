//
// Created by Administrator on 2023/8/28.
//
/*
 * 什么是桥接模式
 * 桥接模式是一种结构型设计模式，可将业务逻辑或一个大类拆分为不同的层次结构，从而能独立地进行开发。桥接模式通过将继承改为组合
 * 的方式来解决这个问题。具体来说，就是抽取其中一个维度并使值成为独立的类层次，这样就可以在初始类中引用这个新层次的对象，
 * 从而使得一个类不必拥有所有的状态和行为。层次结构中第一层（通常称为抽象部分）将包含对第二层（实现部分）对象的引用
 * 抽象部分将能将一些（有时是绝大部分）对自己的调用委派给实现部分的对象。所有的实现部分都有一个通用接口，因此它们能在抽象的部分内部相互替换
 *
 * 为什么使用桥接模式
 * 桥接模式在处理跨平台应用、支持多种类型的数据库服务器或与多个特定种类（例如云平台和社交网络）的API供应商协作时会特别有用
 *
 * 桥接模式实现步骤
 * 1.提供一个抽象类：提供高层控制逻辑，依赖实际底层对象
 * 2.提供一个精准抽象类：拓展抽象类，更精确的抽象
 * 3.提供具体实现抽象类：具体实现类的抽象通用接口
 * 4.提供具体实现：针对不同底层的通用接口实现
 *
 * 桥接模式优缺点
 * 可以创建与平台无关的类和程序
 * 客户端代码仅与高层抽象部分互动，不接触到平台详细信息
 * 开闭原则，可以新增抽象与实现，且它们之间互不影响
 * 单一职责原则，抽象专注高层逻辑，实现部分处理平台细节
 * 缺点
 * 对高内聚的类使用该模式可能会让代码更加复杂
 */
#include "iostream"

using namespace std;

class Color {
public:
    virtual void FillColor() = 0;
};

class Red : public Color {
public:
    Red() : colorType("red") {}

    void FillColor() {
        cout << "填充颜色:" << colorType << endl;
    }

private:
    string colorType;
};

class Blue : public Color {
public:
    Blue() : colorType("blue") {}

    void FillColor() {
        cout << "填充颜色:" << colorType << endl;
    }

private:
    string colorType;
};

class Shape {
public:
    virtual void ShowShape() = 0;

    virtual void SetColor(Color *color) = 0;

protected:
    virtual void DrawShape() = 0;

    virtual void DrawColor() = 0;

    Color *color;
};

class Circle : public Shape {
public:
    Circle() : shapeType("Circle") {}

    void ShowShape() {
        DrawShape();
        DrawColor();
    }

    void SetColor(Color *color) {
        this->color = color;
    }

private:
    void DrawShape() {
        cout << "绘制" << shapeType << endl;
    }

    void DrawColor() {
        this->color->FillColor();
    }

    string shapeType;
};

class Rect : public Shape {
public:
    Rect() : shapeType("Rect") {}

    void ShowShape() {
        DrawShape();
        DrawColor();
    }

    void SetColor(Color *color) {
        this->color = color;
    }

private:
    void DrawShape() {
        cout << "绘制" << shapeType << endl;
    }

    void DrawColor() {
        this->color->FillColor();
    }

    string shapeType;
};

void test01() {
    cout<<"绘制圆"<<endl;
    Shape*shape=new Circle;
    shape->SetColor(new Red);
    shape->ShowShape();
    shape->SetColor(new Blue);
    shape->ShowShape();
    delete shape;
    cout<<"绘制矩形"<<endl;
    shape=new Rect;
    shape->SetColor(new Red);
    shape->ShowShape();
    shape->SetColor(new Blue);
    shape->ShowShape();
}

int main() {
    test01();
}