//
// Created by Administrator on 2023/9/1.
//
/*
 * 什么是解释器模式
 * 解释器模式属于行为型模式，它提供了评估语言的语法或表达式的方式。这种模式实现了一个表达式接口，该接口解释一个特定的上下文
 * 这种模式被用在SQL解析，符号处理引擎等
 *
 * 为什么使用解释器模式
 * 在软件开发中，会遇到有些问题多次重复出现，而且有一定的相似性和规律性，如果将它们归纳成一种简单的语言，那么只写问题实例将是
 * 该语言的一些句子。这样就可以用“编译原理”中的解释器模式来实现了。虽然使用解释器模式的实例并不是很多，但对于满足以上特点，且对运行效率不是特别高的应用
 * 实例，如果用解释器模式来实现，效果会是非常好的
 *
 * 解释器模式实现步骤
 * 1.提供一个解释器上下文环境类：用来存储解释器的上下文环境，比如需要解析的文法，一般用来传递所有被解释器共享的数据
 * 2.提供一个解释器抽象类：定义解释器的接口，约定解释器的解释操作
 * 3.提供一个终结符解释器的具体实现类：用来实现语法规则中和终结符相关的操作
 * 4.提供一个非终结符解释器：用来实现语法规则中非终结符的相关操作。通常一个解释器对应一个语法规则，可以包含其他解释器
 *
 * 解释器模式优缺点
 * 优点
 * 扩展性好。由于在解释器模式中使用类来表示语言的文法规则，因此可以通过继承等机制来改变或扩展文法
 * 容易实现。在语法树中的每个表达式节点类都是相似的，所以实现文法较为容易
 * 缺点
 * 执行效率较低：解释器模式中通常使用大量的循环和递归调用，句子较复杂时，运行速度很慢，且代码的调试过程也比较麻烦
 * 会引起类膨胀：解释器模式中的每条规则至少需要定义一个类，文法规则很多时，类的个数将急剧增加，导致系统难以管理和维护
 */
#include "iostream"
#include "string"
#include "map"
#include "stack"

using namespace std;

//抽象表达式
class Expression {
public:
    virtual int Interpreter(map<string, int> &var) = 0;
};

//变量解析器
class VarExpression : public Expression {
public:
    VarExpression(string key) : key(key) {}

    int Interpreter(map<string, int> &var) {
        return var[key];
    };
private:
    string key;
};

//运算符解析器
//抽象父类
class SymbolExpression : public Expression {
public:
    SymbolExpression(Expression *left, Expression *right) : left(left), right(right) {}

    Expression *GetLeft() { return left; }

    Expression *GetRight() { return right; }

protected:
    Expression *left;
    Expression *right;
};

//具体子类
class AddExpression : public SymbolExpression {
public:
    AddExpression(Expression *left, Expression *right) : SymbolExpression(left, right) {}

    int Interpreter(map<string, int> &var) {
        return left->Interpreter(var) + right->Interpreter(var);
    };

};

class SubExpression : public SymbolExpression {
public:
    SubExpression(Expression *left, Expression *right) : SymbolExpression(left, right) {}

    int Interpreter(map<string, int> &var) {
        return left->Interpreter(var) - right->Interpreter(var);
    };

};

//封装调用接口->解析器的封装类
class Caculator {

public:
    Caculator(string expstr) {  //a+b-c
        expression = nullptr;
        stack<Expression *> stkexp;
        Expression *left = nullptr;
        Expression *right = nullptr;
        for (int i = 0; i < expstr.length(); i++) {
            switch (expstr[i]) {
                case '+':
                    //从栈中取出左操作数
                    left = stkexp.top();
                    stkexp.pop();
                    //从文法字符串取出+后面的右操作数
                    right = new VarExpression(expstr.substr(++i, 1));
                    //将新的结果存入栈
                    stkexp.push(new AddExpression(left,right));
                    break;
                case '-':
                    //从栈中取出左操作数
                    left = stkexp.top();
                    stkexp.pop();
                    //从文法字符串取出+后面的右操作数
                    right = new VarExpression(expstr.substr(++i, 1));
                    //将新的结果存入栈
                    stkexp.push(new SubExpression(left,right));
                    break;
                default:
                    stkexp.push(new VarExpression(expstr.substr(i, 1)));
                    break;
            }
        }
        //栈中保存的就是最终语法树的根
        if(!stkexp.empty()){
            expression=stkexp.top();
            stkexp.pop();
        }
    }

    int Run(map<string,int>&var){
        return (expression== nullptr)?0:expression->Interpreter(var);
    }

private:
    Expression *expression;
};

void test01() {
    string expstr = "a+b-c";
    map<string, int> var;
    var["a"] = 10;
    var["b"] = 100;
    var["c"] = 30;

    Caculator cal(expstr);
    cout<<cal.Run(var)<<endl;
}

int main() {
    test01();
}