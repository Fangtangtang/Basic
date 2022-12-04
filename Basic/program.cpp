/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.
 * 返回正确类型的值
 * Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"


Program::Program() = default;

Program::~Program() {
    clear();
}

//清空program中所有行
void Program::clear() {
    for (auto iter = lineNum.begin(); iter != lineNum.end(); ++iter) {
        delete aLine[*iter].statement;//回收空间
    }
    aLine.clear();
    lineNum.clear();
}

//加入特定行号的一行
//若行号已经存在 覆盖text 删除已解析的
//行号不存在 将program加到正确的行
//(statement haven't been set)
void Program::addSourceLine(int lineNumber, const std::string &line) {
    if (lineNum.count(lineNumber)) {//exist
        aLine[lineNumber].text = line;
        delete aLine[lineNumber].statement;
    } else {
        lineNum.insert(lineNumber);
        lineInformation newline{ line};
        aLine[lineNumber] = newline;
    }
}

//移去一行 释放空间
//若行不存在 直接返回
void Program::removeSourceLine(int lineNumber) {
    if (lineNum.count(lineNumber)) {
        delete aLine[lineNumber].statement;//free the memory
        aLine.erase(lineNumber); //erase from map
        lineNum.erase(lineNumber);//erase from set
    }
}

//返回program行和行号
//行不存在 返回empty string
std::string Program::getSourceLine(int lineNumber) {
    std::string str = "";
    if (lineNum.count(lineNumber)) {
        str =aLine[lineNumber].text;
    }
    return str;
}

//设置行状态
//向行的statement加状态的解析表示
//行不存在 抛出异常
//若存在之前状态解析表示 回收之前statement空间
void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (lineNum.count(lineNumber)) {
           aLine[lineNumber].statement = stmt;
    } else {
        error("SYNTAX ERROR");
    }
}

//获取行状态
//Statement *stmt = program.getParsedStatement(lineNumber);
//若未设置值 返回null
Statement *Program::getParsedStatement(int lineNumber) {
    if (lineNum.count(lineNumber)) {
        return aLine[lineNumber].statement;
    } else {
        return nullptr;
    }
}

//返回program第一个行号
//无行 return -1；
int Program::getFirstLineNumber() {
    auto iter = lineNum.begin();
    if (iter != lineNum.end()) {
        return *iter;
    } else {
        return -1;
    }
}

//返回下一个存在的行号
//无下一个 return -1；
int Program::getNextLineNumber(int lineNumber) {
    auto iter = lineNum.find(lineNumber);
    ++iter;
    if (iter != lineNum.end()) {
        return *iter;
    } else {
        return -1;
    }
}

//从某个行号开始执行（goto ifthen）
void Program::runFrom(int lineNumber,EvalState &state) {
    Statement *sta= nullptr;
    try{
        while(lineNumber!=-1){
            if(!find(lineNumber)) {
                error("LINE NUMBER ERROR");
            }
            sta=getParsedStatement(lineNumber);
            sta->execute(state, *this);
            lineNumber =next(lineNumber);
        }
    }catch(ErrorException &ex){
        std::cout << ex.getMessage() <<std::endl;
    }
}

int Program::next(int lineNumber) {
    if(aLine[lineNumber].goTo ){
        return aLine[lineNumber].goTo;
    }else {
        return getNextLineNumber( lineNumber);
    }
}

void Program::setGoTo(int lineNumber,int go){
    aLine[lineNumber].goTo=go;
}

bool Program::find(int lineNumber){
    int i=lineNum.count(lineNumber);
    if(lineNum.count(lineNumber)) return true;
    else return false;
}