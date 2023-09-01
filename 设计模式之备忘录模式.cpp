//
// Created by Administrator on 2023/9/1.
//
/*
 * 什么是备忘录模式
 * 备忘录模式是一种行为设计模式，允许在不暴露对象实现细节的情况下保存和恢复对象之前的状态
 * 如浏览器回退，编辑器撤销和重做，虚拟机生成快照与恢复，游戏悔棋等
 *
 * 为什么使用备忘录模式
 * 备忘录模式允许赋值对象中的全部状态(包括私有成员变量)，并将其独立于对象进行保存。备忘录模式不仅可以用于“撤销”，
 * 也在处理事务(比如需要在出现错误时回滚一个操作)的过程中必不可少。备忘录模式让对象自行负责创建其状态的快照
 * 任何其他对象都不能读取该快照，这有效地保障了数据的安全性
 *
 * 备忘录模式实现步骤
 * 1.提供一个原发器类：真正要被保存或者恢复的对象，其负责创建一个备忘录，可以存储，恢复需要状态信息
 * 2.提供一个备忘录类：用于存储原生器对象的内部状态，防止外部直接访问原生器对象
 * 3.提供一个管理者类：负责存储备忘录，但不能对备忘录内容进行操作和访问，只能将备忘录传递给其他对象
 *
 * 备忘录模式优缺点
 * 优点
 * 可以在不破坏对象封装情况的前提下，创建对象状态快照
 * 可以通过让负责任维护原发器状态历史记录来简化原发器代码
 *
 * 缺点
 * 如果客户端过于频繁地创建备忘录，程序将消耗大量内存
 * 负责人必须完整跟踪原发器的生命周期，这样才能销毁弃用的备忘录
 */
#include "iostream"
#include "vector"

using namespace std;

class Memento {
public:

    Memento(string name, pair<int, int> pos) : name(name), pos(pos) {}

    string GetName() {
        return name;
    }

    auto GetPos() {
        return pos;
    }

    void show() {
        cout << name << ":(" << pos.first << "," << pos.second << ")" << endl;
    }

private:
    string name;
    pair<int, int> pos;
};

class Chess {
public:
    Chess(string name, pair<int, int> pos) : name(name), pos(pos) {}

    //走一下
    void SetChess(string name, pair<int, int> pos) {
        this->name = name;
        this->pos = pos;
    }

    //保存
    Memento *saveState() {
        return new Memento(name, pos);
    }

    //回退
    void Restore(Memento *memento) {
        name = memento->GetName();
        pos = memento->GetPos();
    }

protected:
    string name;
    pair<int, int> pos;
};

//管理类
class Caretaker {
public:
    void AddMemento(Memento *memento) {
        if (step < m.size()) {
            m[step] = memento;
        }
        else
        m.push_back(memento);
        step++;
    }

    Memento *GetMemento(int index) {
        //步数满足条件
        step = index;
        return m[step];
    }

    void show() {
        for (int i = 0; i < step; i++) {
            cout << "step:" << i + 1 << "手" << endl;
            m[i]->show();
        }
    }

protected:
    vector<Memento *> m;
    static int step;
};

int Caretaker::step = 0;

void test01() {
    Chess *pc = new Chess("车", {4, 3});
    Caretaker *pcaretaker = new Caretaker;
    pcaretaker->AddMemento(pc->saveState());
    pcaretaker->show();
    cout << endl;
    pc->SetChess("马", {1, 2});
    pcaretaker->AddMemento(pc->saveState());
    pc->SetChess("兵", {5, 5});
    pcaretaker->AddMemento(pc->saveState());
    pc->SetChess("炮", {3, 3});
    pcaretaker->AddMemento(pc->saveState());
    pcaretaker->show();
    cout << endl;
    pc->Restore(pcaretaker->GetMemento(3));
    pcaretaker->show();
}

int main() {
    test01();
}