/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.hpp"



/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 * 读表达式 检查是否有多余token
 * 解析表达式
 */

// Expression *exp = parseExp(scanner);
//从scanner读取token解析
//scanner设为忽略空白 读取数字
Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

//Expression *exp = readE(scanner, prec);
//prec默认0 表示读整个表达式
//获得一个表达式（一个独立的组：单个表达式 复合表达式）
Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);//左边表达式
    std::string token;//运算符 or not
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);//token（运算符）代表的优先级
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);//读取运算符右边表达式
        exp = new CompoundExp(token, exp, rhs);//新建复合表达式
    }
    scanner.saveToken(token);//若退出 把末个读取的token暂存在saveToken
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

//Expression *exp = readT(scanner);
//返回下一个
Expression *readT(TokenScanner &scanner) {//基类指针
    std::string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);//token类型
    if (type == WORD) return new IdentifierExp(token);//标识符
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));//常量
    if (token != "(") error("Illegal term in expression");
    //token==“（”
    Expression *exp = readE(scanner);//读表达式（独立组）
    if (scanner.nextToken() != ")") {//表达式后没有）
        error("Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

//int prec = precedence(token);
//运算符优先级
//无运算符 0
int precedence(std::string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}
