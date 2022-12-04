/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.
 * Statement类的构造和析构
 * Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 * 为BASIC statements 定义派生类
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo


//REM
REMsta::REMsta() = default;

void REMsta::execute(EvalState &state, Program &program) {

}

//LET
LETsta::LETsta(TokenScanner &scanner, int num = 0) {
    try {
        exp = parseExp(scanner);//读取表达式
        lineNumber = num;
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

LETsta::~LETsta() {
    delete exp;
}

void LETsta::execute(EvalState &state, Program &program) {
    try {
        exp->eval(state);//计算
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

//PRINT
PRINTsta::PRINTsta(TokenScanner &scanner, int num = 0) {
    exp = parseExp(scanner);//读取表达式 to be print
    lineNumber = num;
}

PRINTsta::~PRINTsta() {
    delete exp;
}

void PRINTsta::execute(EvalState &state, Program &program) {
    try {
        std::cout << exp->eval(state) << '\n';
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

//INPUT
INPUTsta::INPUTsta(TokenScanner &scanner, int num = 0) {
    if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    } else {
        name = scanner.nextToken();//切片得一个token 词法检查
    }
    if (scanner.hasMoreTokens())error("SYNTAX ERROR");
    lineNumber = num;
}

INPUTsta::~INPUTsta() = default;

void INPUTsta::execute(EvalState &state, Program &program) {
    std::string in, token, sign;
    int value;
    while (true) {
        try {
            std::cout << " ? ";
            getline(std::cin, in);
            TokenScanner scanner;
            scanner.setInput(in);
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            } else {
                token = scanner.nextToken();//切片得一个token 词法检查
            }
            if (token == "-") {
                sign = token;
                if (!scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR");
                } else {
                    token = scanner.nextToken();//切片得一个token 词法检查
                }
            }
            if (scanner.getTokenType(token) == NUMBER) {
                if (scanner.hasMoreTokens())error("INVALID NUMBER");
                value = stringToInteger(token);
                if (sign == "-") value = -value;
                state.setValue(name, value);
                break;
            } else {
                error("INVALID NUMBER");
            }
        } catch (ErrorException &ex) {
            std::cout << "INVALID NUMBER" << '\n';
        }
    }
}

//END
ENDsta::ENDsta(int num) {
    lineNumber=num;
}

void ENDsta::execute(EvalState &state, Program &program) {
    program.setGoTo(lineNumber, -1);
}


//GOTO
GOTOsta::GOTOsta(TokenScanner &scanner, int num) {
    exp = parseExp(scanner);//读取表达式
    lineNumber = num;
}

GOTOsta::~GOTOsta() {
    delete exp;
    delete newLineStatement;
}

void GOTOsta::execute(EvalState &state, Program &program) {
    newLineNumber = exp->eval(state);//去向行号
    program.setGoTo(lineNumber, newLineNumber);
}

//IF
//IFsta::IFsta(TokenScanner &scanner,EvalState &state,int num) {
IFsta::IFsta(std::string line, int num) {
    //10 IF 1 + 1 = 2 THEN 30
    lineNumber = num;
    this->Line = line;
}

IFsta::~IFsta()=default;

void IFsta::execute(EvalState &state, Program &program) {
    side = true;
    expressionL = expressionR = op = "";
    scan.ignoreWhitespace();//跳过空白
    scan.scanNumbers();//遇数字 读数
    scan.setInput(Line);//将line加入输入流
    token = scan.nextToken();
    token = scan.nextToken();
    while (true) {
        if (!scan.hasMoreTokens()) {
            error("SYNTAX ERROR");
        } else {
            token = scan.nextToken();//切片得一个token 词法检查
        }
        if (token == "THEN") break;
        if (token == "=" || token == ">" || token == "<") {
            side = false;
            op += token;
            continue;
        }
        if (side)expressionL = expressionL + token + " ";//左式
        else expressionR = expressionR + token + " ";//右式
    }
    //lineNumber
    if (!scan.hasMoreTokens()) {
        error("SYNTAX ERROR");
    } else {
        token = scan.nextToken();//切片得一个token 词法检查
    }
    newLineNumber = stringToInteger(token);
    //表达式求值
    scan.setInput(expressionL);
    expl = parseExp(scan);
    l = expl->eval(state);//左值
    scan.setInput(expressionR);
    expr = parseExp(scan);
    r = expr->eval(state);//右值//判断
    if (op == "="){
        judge = (l == r);
    }
    if (op == ">")judge = (l > r);
    if (op == "<")judge = (l < r);
    if (op == ">=")judge = (l >= r);
    if (op == "<=")judge = (l <= r);
    if (judge){
        program.setGoTo(lineNumber, newLineNumber);
    }
    else {
        program.setGoTo(lineNumber, 0);

    }delete expl ;
    delete expr;
}

RUNsta::RUNsta() = default;

RUNsta::~RUNsta() = default;

void RUNsta::execute(EvalState &state, Program &program) {
    int i = program.getFirstLineNumber();
    program.runFrom(i, state);
}

LISTsta::LISTsta() = default;

LISTsta::~LISTsta() {
    delete currentStatent;
}

void LISTsta::execute(EvalState &state, Program &program) {
    int i = program.getFirstLineNumber();
    while (i != -1) {
        std::cout << program.getSourceLine(i) << '\n';
        i = program.getNextLineNumber(i);
    }
}

CLEARsta::CLEARsta() = default;

void CLEARsta::execute(EvalState &state, Program &program) {
    program.clear();
    state.Clear();
}

HELPsta::HELPsta() = default;

void HELPsta::execute(EvalState &state, Program &program) {
    std::cout << "Yet another basic interpreter" << '\n';
}