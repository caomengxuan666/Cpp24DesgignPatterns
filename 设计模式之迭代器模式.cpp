//
// Created by Administrator on 2023/9/1.
//
/*
 * 什么是迭代器模式
 * 迭代器模式是一种行为设计模式，能使得在不暴露集合底层表现形式的情况下(如STL迭代器)遍历集合中的所有元素
 *
 * 为什么使用迭代器模式
 * 迭代器封装了与复杂数据结构进行交互的细节，为客户端提供多个访问集合元素的简单方法。这种方法不仅对客户端来说非常方便，而且能避免客户端
 * 在直接与集合交互时执行错误或有害的操作，从而起到保护集合的作用
 * 重要迭代算法的代码往往体积非常庞大，当这些代码被放置在程序业务逻辑中，它会让原始代码的指责模糊不清，降低其维护性
 * 因此，将遍历代码移到特定的迭代器中可以使程序代码更加精炼和简洁
 *
 * 迭代器模式实现步骤
 * 1.提供一个迭代器抽象类:抽象实现迭代功能的最小定义方法集
 * 2.提供一个迭代器类:定义实现迭代功能的最小定义方法集
 * 3.提供一个容器抽象类：抽象基本功能以及提供类似迭代器类的方法
 * 4.提供一个容器具体类：定义基本功能以及提供类似迭代器的方法
 *
 * 迭代器模式优缺点
 * 优点
 * 单一职责原则，通过将体积庞大的遍历算法代码抽取为独立的类，可以对客户端代码和集合进行整理
 * 开闭原则：可以实现新型的集合和迭代器并将其传递给现有代码，无需修改现有代码
 * 可以并行遍历同一集合，因为每个迭代器对象都包含自身的遍历状态
 * 缺点
 * 如果程序只与简单的集合进行交互，应用该模式可能会矫枉过正
 * 对于某些特殊集合，使用迭代器可能比直接遍历的效率低
 */
#include "iostream"
#include "vector"


using namespace std;

template<class T, class U>
class Iterator {
public:
    typedef typename vector<T>::iterator iter_type;

    Iterator(U *data, bool reverse = false) : m_data(data), iter(data->m_data.begin()) {}

    void Begin() {
        iter = m_data->m_data.begin();
    }

    void Next() {
        iter++;
    }

    bool End() {
        return (iter == m_data->m_data.end());
    }

    iter_type Current() {
        return iter;
    }

protected:
    U *m_data;
    iter_type iter;
};

template<class T>
class Container {
    friend class Iterator<T, Container>;

public:
    void Add(T data) {
        m_data.push_back(data);
    }

    Iterator<T, Container> *CreateIterator() {
        return new Iterator<T, Container>(this);
    }

protected:
    vector<T> m_data;
};

class Data {
public:
    Data(int data = 0) : m_data(data) {}

    void SetData(int data) {
        m_data = data;
    }

    int GetData() {
        return m_data;
    }

private:
    int m_data;
};

void test01() {
    Container<int> intData;
    for (int i = 0; i < 10; i++) {
        intData.Add(i);
    }
    Iterator<int, Container<int>> *iter = intData.CreateIterator();
    for (iter->Begin(); !iter->End(); iter->Next()) {
        cout << *iter->Current() << " ";
    }
    cout << endl;
}

void test02() {
    Container<Data> testData;
    testData.Add(Data(168));
    testData.Add(Data(328));
    testData.Add(Data(648));
    Iterator<Data,Container<Data>>*iter=testData.CreateIterator();
    for (iter->Begin(); !iter->End(); iter->Next()) {
        cout << iter->Current() ->GetData()<< " ";
    }
    cout<<endl;
}

int main() {
    test01();
    test02();
}