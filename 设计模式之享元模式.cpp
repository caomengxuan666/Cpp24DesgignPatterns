//
// Created by Administrator on 2023/8/30.
//
/*
 * 什么是享元模式
 * 享元模式是一种结构型设计模式，它摒弃了在每个对象中保存所有数据的方式，通过共享多个对象所共有的相同状态，让你能在有限的内存容量中载入更多对象
 *
 * 为什么使用享元模式
 * 通过复用相同的对象来减少对象的创建数量，创建更小的对象组，并通过共享实现复用。通过归类，将对象的属性分为内蕴状态和外蕴状态
 * 外蕴状态是对象的外部描述，是每个对象的可变部分，比如对工具的使用地点、使用时间、使用人、工作内容的描述，这些属性不属于对象本身，
 * 而是根据每回使用情况进行变化的，这就需要制作成接口进行外部调用，而外蕴状态的维护是由调用者维护的，对象内不进行维护
 *
 * 享元模式实现步骤
 * 1.提供一个抽象享元角色类：具体享元类的父类，规定一些需要实现的公共接口
 * 2.提供一个或多个具体享元角色：实现了抽象享元角色规定的方法
 * 3.提供一个享元工厂角色类：负责创建和管理享元角色
 *
 * 享元模式优缺点
 * 优点
 * 如果程序中有很多相似对象，那么就可以节省大量内存
 * 缺点
 * 可能需要牺牲执行速度来换取内存，因为他人每次调用享元方法时都需要重新计算部分情景数据
 */
#include "iostream"
#include "map"

using namespace std;

//事物属性抽象 分为两部分
class SharedState {
public:
    SharedState(const string &brand, const string &model, const string &color) : brand(brand), model(model),
                                                                                 color(color) {}

    const string &GetBrand() const { return brand; }

    const string &GetModel() const { return model; }

    const string &GetColor() const { return color; }

    friend ostream &operator<<(ostream &out, const SharedState ss) {
        return out << "[" << ss.brand << "," << ss.model << "," << ss.color << "]" << endl;
    }

protected:
    string brand;
    string model;
    string color;
};

class UniqueState {
public:
    UniqueState(const string &owner, const string &plates) : owner(owner), plates(plates) {}

    const string &GetOwner() const { return owner; }

    const string &GetPlates() const { return plates; }

    friend ostream &operator<<(ostream &out, const UniqueState ss) {
        return out << "[" << ss.owner << "," << ss.plates << "]" << endl;
    }

private:
    string owner;
    string plates;
};

class FlyWeight {
public:
    FlyWeight(const SharedState *sharedState) : shared_state(new SharedState(*sharedState)) {}//内部使用的是拷贝构造
    FlyWeight(const FlyWeight &other) : shared_state(new SharedState(*other.shared_state)) {}

    SharedState *GetSharedState() const { return shared_state; }

    void Show(const UniqueState &uniqueState) {
        cout << "共享数据:" << *shared_state << endl;
        cout << "专有数据:" << uniqueState << endl;
    }

private:
    SharedState *shared_state;
};

class FlyWeightFactory {
public:
    FlyWeightFactory(initializer_list<SharedState> sharedstate) {
        for (auto &v: sharedstate) {
            this->flyweight.insert(make_pair(this->GetKey(v), FlyWeight(&v)));
        }
    }

    FlyWeight GetFlyweight(const SharedState &sharedState) {
        string key = this->GetKey(sharedState);
        if (this->flyweight.find(key) == this->flyweight.end()) {
            cout << "车库未找到该型号" << endl;
            this->flyweight.insert(make_pair(key, FlyWeight(&sharedState)));
            cout << "入库成功" << endl;
        } else cout << "车库找到这个型号..." << endl;
        return this->flyweight.at(key);
    }

    void ListFlyWeights() const {
        int count = this->flyweight.size();
        cout << "车库总信息:" << endl;
        for (auto &pair: this->flyweight) {
            cout << pair.first << "\n";
        }
    }

private:
    map<string, FlyWeight> flyweight;

    string GetKey(const SharedState &ss) const { return ss.GetBrand() + '_' + ss.GetModel() + '_' + ss.GetColor(); }
};

void AddCar(FlyWeightFactory &ff, const string &plates, const string &owner, const string &brand, const string &model,
            const string &color) {
    cout << "车型匹配结果" << endl;
    auto flyweight = ff.GetFlyweight({brand, model, color});
    flyweight.Show({owner, plates});
}

void test01() {
    FlyWeightFactory *f = new FlyWeightFactory(
            {
                    {"奥迪", "2023", "red"},
                    {"奔驰", "C43",  "black"},
                    {"丰田", "AE86", "white"}
            }
    );
    AddCar(*f, "2023", "cmx", "宝马", "M6", "blue");
    AddCar(*f, "2022", "cmx", "奔驰", "E", "blue");
    f->ListFlyWeights();
    delete f;
}

int main() {
    test01();
}