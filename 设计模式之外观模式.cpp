//
// Created by Administrator on 2023/8/29.
//
/*
 * 什么是外观模式
 * 外观模式是一种结构型设计模式，能为程序库、框架或者其他复杂类提供
 *
 *
 * 一个简单的接口
 * 外观模式实现步骤
 * 1.提供一个外观类，为调用方，定义简单的调用接口
 * 2.提供一个调用者类，通过外观接口提供某功能的内部类群
 * 3.多个功能提供者类，提供功能的类群（模块或子系统）
 *
 * 外观模式优缺点
 * 优点
 * 可以让自己的代码独立于复杂子系统
 * 缺点
 * 外观可能成为与程序中所有类都耦合的上帝对象（了解过多或者是负责过多的对象）
 */
#include "iostream"

using namespace std;

//功能提供者类
class CSyntaxParser {
public:
    void SyntaxParser() {
        cout << "语法分析中" << endl;
    }
};

class CMidCode {
public:
    void MidCode() {
        cout << "生成中间代码" << endl;
    }
};

class CAssemblyCode {
public:
    void AssemblyCode() {
        cout << "生成汇编代码" << endl;
    }
};

class Clink {
public:
    void LinkSystem() {
        cout << "链接成可执行程序" << endl;
    }
};

class Fade {
public:
public:
    void Build() {
        CSyntaxParser().SyntaxParser();
        CMidCode().MidCode();
        CAssemblyCode().AssemblyCode();
        Clink().LinkSystem();
        cout << "程序运行中" << endl;
    }
};

void test01() {
    Fade f;
    f.Build();
}

int main() {
    test01();
}