/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

inline std::string take(TokenScanner &scanner);//切一片

/* Main program */

int main() {
    EvalState state;//存取变量
    Program program;//具体执行 存具体内容
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() <<std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.
 * 处理单行输入
 * In this version of implementation, the program reads a line,
 * parses it as an expression,
 * and then prints the result.
 * In your implementation, you will
 * need to replace this method with one that can
 * respond correctly when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 * 处理输入的带行号命令或者BASIC commands
 */

//指令合法性检查 throw
void processLine(std::string line, Program &program, EvalState &state) {
    //scanner初始设置
    TokenScanner scanner;
    scanner.ignoreWhitespace();//跳过空白
    scanner.scanNumbers();//遇数字 读数
    scanner.setInput(line);//将line加入输入流
    //读入指令并切片 string->token
    std::string token;
    token = take(scanner);
    TokenType type = scanner.getTokenType(token);//获取token类型
    Statement *current = nullptr;
//无行号的
//1控制 直接执行
        if (token == "QUIT") {
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            exit(0);
        }
        if (token == "RUN") {
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            current = new RUNsta();
        }
        if (token == "LIST") {
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            current = new LISTsta();
        }
        if (token == "CLEAR") {
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            current = new CLEARsta();
        }
        if (token == "HELP") {
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            current = new HELPsta();
        }
//2statement 直接执行
        if (token == "PRINT") {
            current = new PRINTsta(scanner,0);
        }
        if (token == "INPUT") {
            current = new INPUTsta(scanner,0);
        }
        if (token == "LET") {
            current = new LETsta(scanner, 0);
        }
    if (current != nullptr) {
        current->execute(state, program);
        delete current;
    }
//带行号的 run执行 存到program
if(type==NUMBER){
        int num= stringToInteger(token);
        std::string tokenNum=token;
        //REM no error exits
        if(!scanner.hasMoreTokens()){
            if(num==-1)error("SYNTAX ERROR");
            program.removeSourceLine(num);
        }else{
            program.addSourceLine(num, line);
            token=take(scanner);
            if (token == "REM"){
                current = new REMsta();
            }
            if (token == "END") {
                if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
                current = new ENDsta(num);
            }
            if (token == "PRINT") {
                current = new PRINTsta(scanner, num);
            }
            if (token == "INPUT") {
                current = new INPUTsta(scanner, num);
            }
            if (token == "GOTO") {
                current = new GOTOsta(scanner, num);
            }
            if (token == "LET") {
                current = new LETsta(scanner, num);
            }
            if(token=="IF"){
                current = new IFsta(line ,num);
            }
            //存入program
            program.setParsedStatement(num,current);
        }
    }
}

inline std::string take(TokenScanner &scanner) {
    std::string token;
    if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    } else {
        token = scanner.nextToken();//切片得一个token 词法检查
    }
    return token;
}
