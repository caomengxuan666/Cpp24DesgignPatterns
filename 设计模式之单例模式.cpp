//
// Created by Administrator on 2023/8/23.
//
/*
 * 构造函数私有化
 * 提供一个全局的静态方法，访问唯一对象
 * 类中定义一个静态指针，指向唯一对象
 *
 * 为什么使用单例模式
 * 在应用系统开发中，我们经常有以下需求
 * 1.需要生成唯一序列的环境
 * 2.需要频繁实例化然后销毁的对象
 * 3.创建对象耗时过多或者耗费资源过多，但又要经常用到的对象
 * 4.方便资源相互通信的方式
 *
 * 实际案例
 * 多线程中网络资源初始化
 * 回收站机制
 * 任务管理器
 * 应用程序日志管理
 *
 * 单例模式之懒汉式
 * 单例模式之饿汉式
 * 不在类的静态方法中创建对象，而是利用对象指针直接在类外new一个
 *
 * 单例模式优缺点
 * 优点
 * 在内存中只有一个对象，节省内存空间
 * 避免频繁的创建销毁对象，可以提高性能
 * 避免对共享资源的多重占用，简化访问
 * 为整个系统提供一个全局访问点
 * 缺点
 * 不适用于变化频繁的对象
 * 如果实例化的对象长时间未被使用，系统会认为该对象是垃圾而被回收，这可能会导致对象状态的丢失，不过这是JAVA的垃圾回收机制不管C++什么事
 */
#include "iostream"

using namespace std;

class SingleTon {
public:
    static SingleTon *m_singleTon;  //静态指针

    static SingleTon *GetInstance() {   //提供静态方法
        if (m_singleTon == nullptr) {
            m_singleTon = new SingleTon;
            return m_singleTon;
        }
    }

    //  static SingleTon *GetInstance() {   //如果是饿汉模式就直接返回
//            return m_singleTon;
//    }

    void testPrint() {
        cout << "测试调用" << endl;
    }

private:
    SingleTon() {   //构造函数私有化
        cout << "构造对象" << endl;
        m_singleTon = nullptr;
    }
};

//SingleTon*singleTon::m_singleTon=nullptr  //不在类外new，懒汉模式
SingleTon *SingleTon::m_singleTon = new SingleTon;  //饿汉模式


void test01() {
    SingleTon *p1 = SingleTon::GetInstance();
    SingleTon *p2 = SingleTon::GetInstance();
    cout << "p1:" << hex << p1 << endl;
    cout << "p2:" << hex << p2 << endl;
    p1->testPrint();
    p2->testPrint();
}


int main() {
    test01();
}