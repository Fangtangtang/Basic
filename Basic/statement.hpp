/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.
 * 定义Statement基类
 * In the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 * !!!!!参考exp.h!!!!!
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include"parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"


class Program;//程序类

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * 代表状态/命令
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.
 * 执行一个状态（TM）
 * Each of the subclasses defines its own execute method
 * that implements the necessary operations.
 * As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 * 用到EvalState类 变量
 */

    virtual void execute(EvalState &state, Program &program) = 0;

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.
 * 派生类定义 各种状态
 * Each of those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.
 * 1.构造函数：从scanner解析状态
 * 2.execute：执行该状态
 * If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 * 注意内存泄漏
 */


class REMsta : public Statement {
public:
    REMsta();

    virtual void execute(EvalState &state, Program &program) override;
};

class LETsta : public Statement {
public:
    LETsta(TokenScanner &scanner, int num);

    ~LETsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    Expression *exp;
    int lineNumber;
};

class PRINTsta : public Statement {
public:
    PRINTsta(TokenScanner &scanner, int num);

    ~PRINTsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    Expression *exp=nullptr;
    int lineNumber;
};

class INPUTsta : public Statement {
public:
    INPUTsta(TokenScanner &scanner, int num);

    ~INPUTsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    TokenScanner scan;
    std::string name,line;
    int lineNumber;
};

class ENDsta : public Statement {
public:
    ENDsta(int num);

    virtual void execute(EvalState &state, Program &program) override;
private:
    int lineNumber;
};

class GOTOsta : public Statement {
public:
    GOTOsta(TokenScanner &scanner, int num);

    ~GOTOsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    Expression *exp = nullptr;
    Statement *newLineStatement = nullptr;
    int lineNumber, newLineNumber;

};

class IFsta : public Statement {
public:
//    IFsta(TokenScanner &scanner,EvalState &state,int num);
    IFsta(std::string line,int num);

    ~IFsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    TokenScanner scan;
    std::string Line,token,op="",expressionL="",expressionR="";
    int lineNumber, newLineNumber,l,r;
    bool judge=true,side=true;
    Expression *expl = nullptr,*expr = nullptr;
};

class RUNsta : public Statement {
public:
    RUNsta();

    ~RUNsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    Statement *currentStatent{};

};

class LISTsta : public Statement {
public:
    LISTsta();

    ~LISTsta();

    virtual void execute(EvalState &state, Program &program) override;

private:
    Statement *currentStatent = nullptr;
};

class CLEARsta : public Statement {
public:
    CLEARsta();

    virtual void execute(EvalState &state, Program &program) override;

};


class HELPsta : public Statement {
public:
    HELPsta();

    virtual void execute(EvalState &state, Program &program) override;

};

#endif
