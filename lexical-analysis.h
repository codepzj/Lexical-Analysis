#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <QString>

// 词法分析核心代码
enum TokenID
{
    IF, // if
    ELSE, // else
    FOR, // for
    DO, // do
    WHILE, // while
    SWITCH, // switch
    CASE, // case
    DEFAULT, // default
    BREAK, // break
    CONTINUE, // continue
    ASSIGN, // =
    EQ, // ==
    LT, // <
    GT, // >
    LTE, // <=
    GTE, // >=
    NOTEQ, // !=
    RightShift, // >>
    LeftShift, // <<
    DECIMAL, // 十进制
    OCTAL, // 八进制
    HEX, // 十六进制
    ID, // 标识符
    ADD, // +
    ADDEQ, // +=
    ADDPLUS, // ++
    SUB, // -
    SUBEQ, // -=
    SUBMINUS, // --
    PTR, // ->
    MUL, // *
    MULEQ, // *=
    MULSTAR, // **
    DIV, // /
    DIVEQ, // /=
    LINECOMMENT, // //
    BLOCKCOMMENT, // /* */
    LBRACKET, // (
    RBRACKET, // )
    SEMICOLON, // ;
    ENDINPUT, // 结束输入
};

struct TokenStru
{
    TokenID ID;
    double val;
    char op[3];
    char word[20];
};

extern TokenStru token;  // 声明全局变量 token
extern char buffer[1024];  // 声明字符数组 buffer
extern size_t pos;  // 声明位置变量 pos

// 解析正则表达式并判断类型
QString parseSentense(QString fileContent);

#endif // LEXICAL_ANALYSIS_H
