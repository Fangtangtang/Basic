/*
 * File: exp.cpp
 * -------------
 * This file implements the Expression class and its subclasses.
 */

#include "exp.hpp"


/*
 * Implementation notes: the Expression class
 * ------------------------------------------
 * The Expression class declares no instance variables and needs no code.
 */

Expression::Expression() = default;

Expression::~Expression() = default;

/*
 * Implementation notes: the ConstantExp subclass
 * ----------------------------------------------
 * The ConstantExp subclass declares a single instance variable that
 * stores the value of the constant.
 * 储存常数的一个临时变量
 * The eval method doesn't use the
 * value of state but needs it to match the general prototype for eval.
 * eval不使用state的值 但要求原型匹配
 */

//存值
//Expression *exp = new ConstantExp(value);
ConstantExp::ConstantExp(int value) {
    this->value = value;
}

//取值
int ConstantExp::eval(EvalState &state) {
    return value;
}

//转string
std::string ConstantExp::toString() {
    return integerToString(value);
}

//显示类型
ExpressionType ConstantExp::getType() {
    return CONSTANT;
}

//不调用eval
//int value = ((ConstantExp *) exp)->getValue(); exp：指针
//仅用于已知为ConstantExp的对象
int ConstantExp::getValue() {
    return value;
}

/*
 * Implementation notes: the IdentifierExp subclass
 * ------------------------------------------------
 * The IdentifierExp subclass declares a single instance variable that
 * stores the name of the variable.  The implementation of eval must
 * look this variable up in the evaluation state.
 */
//存变量名
IdentifierExp::IdentifierExp(std::string name) {
    this->name = name;
}
//取变量值 （需先判断该变量名是否存在）
int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) error("VARIABLE NOT DEFINED");
    return state.getValue(name);
}
//转string（name sting型）
std::string IdentifierExp::toString() {
    return name;
}

ExpressionType IdentifierExp::getType() {
    return IDENTIFIER;
}
//已知该类型对象 直接取name
//string name = ((IdentifierExp *) exp)->getName();
std::string IdentifierExp::getName() {
    return name;
}

/*
 * Implementation notes: the CompoundExp subclass
 * ----------------------------------------------
 * The CompoundExp subclass declares instance variables for the operator
 * and the left and right subexpressions.  The implementation of eval 
 * evaluates the subexpressions recursively and then applies the operator.
 */

//Expression *exp = new CompoundExp(op, lhs, rhs);运算符 右边式 左边式
//Expression派生 内含Expression
CompoundExp::CompoundExp(std::string op, Expression *lhs, Expression *rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

//删除左右子式
CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

/*
 * Implementation notes: eval
 * --------------------------
 * The eval method for the compound expression case must check for the
 * assignment operator as a special case.  Unlike the arithmetic operators
 * the assignment operator does not evaluate its left operand.
 */
//先特判运算符
//throw
//返回运算结果
int CompoundExp::eval(EvalState &state) {
    if (op == "=") {        if (lhs->getType() != IDENTIFIER) {
            error("Illegal variable in assignment");
        }
        if (lhs->getType() == IDENTIFIER && lhs->toString() == "LET")
            error("SYNTAX ERROR");
        int val = rhs->eval(state);
        state.setValue(((IdentifierExp *) lhs)->getName(), val);
        return val;
    }
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("DIVIDE BY ZERO");
        return left / right;
    }
    return 0;
}
//转表达式（string）
std::string CompoundExp::toString() {
    return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
    return COMPOUND;
}
//获运算符
//string op = ((CompoundExp *) exp)->getOp();
std::string CompoundExp::getOp() {
    return op;
}
//Expression *lhs = ((CompoundExp *) exp)->getLHS();
Expression *CompoundExp::getLHS() {
    return lhs;
}
//Expression *rhs = ((CompoundExp *) exp)->getRHS();
Expression *CompoundExp::getRHS() {
    return rhs;
}
