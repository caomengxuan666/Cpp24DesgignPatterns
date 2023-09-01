//
// Created by Administrator on 2023/8/30.
//
/*
 * 什么是组合模式
 * 组合模式是一种结构型设计模式，可以使用它将对象组合成树状结构，并且能像使用独立对象一样使用他们
 *
 * 为什么使用组合模式
 * 组合模式提供了两种共享公共接口的基本元素类型：简单叶节点和复杂容器，容器中可以包含叶节点和其他容器，这使得可以构建
 * 树状嵌套递归对象结构。如果想要客户端代码以相同方式处理简单和复杂元素，可以使用该模式。组合模式中定义的所有元素共用一个接口。
 * 在这一个接口的帮助下，客户端不必在意其所使用的对象的具体类
 *
 * 组合模式实现步骤
 * 1.提供一个组件类:描述复杂元素的通用接口和方法
 * 2.创建一个叶子节点，描述简单元素
 * 3.提供一个容器类：表示复杂元素
 * 4.容器中定义添加和删除子元素的方法
 *
 * 组合模式优缺点
 * 优点
 * 可以利用多态和递归机制更方便地使用复杂树结构
 * 开闭原则。无需更改现有代码，就可以在应用中添加新元素，使其成为对象树的一部分
 * 缺点
 * 对于功能差异较大的类，提供公共接口可能会有困难
 * 在一些特定情况下，可以存在过度抽象化组件接口，使其变得令人难以理解
 */
#include <iostream>
#include <list>
#include <string>

using namespace std;

class Root {
public:
    virtual void Display(int depth) = 0;
    virtual void Add(Root *file) = 0;
    virtual void Remove(Root *file) = 0;
    virtual list<Root *> GetChild() = 0;
};

class File : public Root {
public:
    File(string name) : name(name) {}

    void Display(int depth) {
        for (int i = 0; i < depth; i++) {
            cout << "----";
        }
        cout << name << endl;
    }

    void Add(Root *file) {}

    void Remove(Root *file) {}

    list<Root *> GetChild() { return m_list; }

private:
    string name;
    list<Root *> m_list;
};

class Folder : public Root {
public:
    Folder(string name) : name(name) {}

    void Display(int depth=0) {
        for (int i = 0; i < depth; i++) {
            cout << "----";
        }
        cout << name << endl;

        for (auto &v : m_list) {
            v->Display(depth + 1);
        }
    }

    void Add(Root *file) {
        m_list.push_back(file);
    }

    void Remove(Root *file) {
        m_list.remove(file);
    }

    list<Root *> GetChild() { return m_list; }

private:
    string name;
    list<Root *> m_list;
};

void Display(Root*root,int depth=0){
    root->Display(depth);
}

int main() {
    Folder *root = new Folder("C:");
    Folder *d1 = new Folder("dir1");
    File *t1 = new File("t1.txt");

    Folder *d2 = new Folder("dir2");
    File *t2 = new File("t2.txt");

    root->Add(d1);
    root->Add(d2);

    d1->Add(t1);
    d2->Add(t2);
    d2->Add(t1);

    Display(root,0);

    delete root;
    delete d1;
    delete d2;
    delete t1;
    delete t2;

    return 0;
}
