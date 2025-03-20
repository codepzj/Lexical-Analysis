#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include "lexical-analysis.h"
#include <cstring>
#include <QString>
using namespace std;

map<string, TokenID> keywords;

TokenStru token;
char buffer[1024]; // 缓冲区
size_t pos = 0;   // 指针(当前位置)

void GetToken()
{
    // 数字
    int s;

    while (buffer[pos] == 32 || buffer[pos] == 9 || buffer[pos] == '\n') // 空格或制表符
        pos++;

    if ((buffer[pos] >= '0') && (buffer[pos] <= '9')) // 数字
    {
        s = 0;
        int count = 0; // 计算数字位数
        char first = buffer[pos];
        char second = buffer[pos + 1];
        // 判断数字类型
        if (first == '0' && (second == 'x' || second == 'X'))
            token.ID = HEX;
        else if (first == '0' && second != 'x' && second != 'X')
            token.ID = OCTAL;
        else
            token.ID = DECIMAL;

        bool isHexPrefix = (first == '0') && ((second == 'x') || (second == 'X'));                            // 判断是否为十六进制前缀
        while ((((buffer[pos] >= '0') && (buffer[pos] <= '9')) || isHexPrefix) && pos < strlen(buffer) && buffer[pos] != ' ' && buffer[pos] != '\0') // 判断是否为数字或十六进制前缀，并且不能是空格或结束符
        {
            // 八进制
            if (token.ID == OCTAL)
            {
                s = s * 8 + buffer[pos] - '0';
            }
            // 十六进制
            else if (token.ID == HEX)
            {
                // 跳过 "0x" 或 "0X"
                if (count == 0) // 仅在第一次进入时跳过
                {
                    pos += 2;
                    count += 2;
                }

                if (buffer[pos] >= '0' && buffer[pos] <= '9')
                {
                    s = s * 16 + buffer[pos] - '0';
                }
                else if (buffer[pos] >= 'a' && buffer[pos] <= 'f')
                {
                    s = s * 16 + buffer[pos] - 'a' + 10;
                }
                else if (buffer[pos] >= 'A' && buffer[pos] <= 'F')
                {
                    s = s * 16 + buffer[pos] - 'A' + 10;
                }
            }
            else
                s = s * 10 + buffer[pos] - '0';
            pos++;
            count++;
        }

        token.val = s;
        if (buffer[pos] == '.')
        {
            int len = 0;
            s = 0;
            pos++;
            while (buffer[pos] >= '0' && buffer[pos] <= '9')
            {
                s = s * 10 + buffer[pos] - '0';
                len++;
                pos++;
            }

            token.val = token.val + s / pow(10, len);
        }

        // 科学计数法
        if (buffer[pos] == 'e' || buffer[pos] == 'E')
        {
            pos++;
            // 指数
            int exp = 0;
            // 符号（1为正，-1为负）
            int sign = 1;
            // 指数符号
            if (buffer[pos] == '+' || buffer[pos] == '-'){
                pos++;
                sign = buffer[pos] == '+' ? 1 : -1;
            }

            while (isdigit(buffer[pos]))
            {
                exp = exp * 10 + buffer[pos] - '0';
                pos++;
            }
            // 添加正负号科学计数法计算
            token.val = token.val * pow(10, sign * exp);
        }
    }
    else if (isalpha(buffer[pos]))
    {
        int len = 0;
        while (isalpha(buffer[pos]) || isdigit(buffer[pos]) || buffer[pos] == '_')
        {
            token.word[len] = buffer[pos];
            len++;
            pos++;
        }
        token.word[len] = '\0';
        map<string, TokenID>::iterator it;
        it = keywords.find(token.word); // 关键字
        if (it != keywords.end())
            token.ID = keywords[token.word];
        else
            token.ID = ID;
    }
    // 符号
    /* 若next匹配相应字符，pos++ */
    else if (buffer[pos] != '\0')
    {

        char next = buffer[pos + 1];
        switch (buffer[pos])
        {
        case '+':
            token.ID = ADD;
            token.op[0] = buffer[pos];
            if (next == '+')
            {
                token.ID = ADDPLUS;
                token.op[1] = '+';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '=')
            {
                token.ID = ADDEQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '-':
            token.ID = SUB;
            token.op[0] = buffer[pos];
            if (next == '-')
            {
                token.ID = SUBMINUS;
                token.op[1] = '-';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '=')
            {
                token.ID = SUBEQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '>')
            {
                token.ID = PTR;
                token.op[1] = '>';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '*':
            token.ID = MUL;
            token.op[0] = buffer[pos];
            if (next == '*')
            {
                token.ID = MULSTAR;
                token.op[1] = '*';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '=')
            {
                token.ID = MULEQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '/')
            {
                token.ID = BLOCKCOMMENT; // 块注释
                token.op[1] = '/';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '/':
            token.ID = DIV;
            token.op[0] = buffer[pos];

            if (next == '=')
            {
                token.ID = DIVEQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '/')
            {
                token.ID = LINECOMMENT; // 单行注释
                token.op[1] = '/';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '*')
            {
                token.ID = BLOCKCOMMENT; // 块注释
                token.op[1] = '*';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '(':
            token.ID = LBRACKET;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;
        case ')':
            token.ID = RBRACKET;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;
        case ';':
            token.ID = SEMICOLON;
            token.op[0] = buffer[pos];
            token.op[1] = '\0';
            break;
        case '>':
            token.ID = GT;
            token.op[0] = '>';
            if (next == '=')
            {
                token.ID = GTE;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '>')
            {
                token.ID = RightShift;
                token.op[1] = '>';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '<':
            token.ID = LT;
            token.op[0] = '<';
            if (next == '=')
            {
                token.ID = LTE;
                token.op[1] = '=';
                token.op[2] = '\0';
                pos++;
            }
            else if (next == '<')
            {
                token.ID = LeftShift;
                token.op[1] = '<';
                token.op[2] = '\0';
                pos++;
            }
            else
            {
                token.op[1] = '\0';
            }
            break;
        case '=':
            token.op[0] = '=';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = EQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            token.ID = ASSIGN;
            break;
        case '!':
            token.op[0] = '!';
            if (buffer[pos + 1] == '=')
            {
                pos++;
                token.ID = NOTEQ;
                token.op[1] = '=';
                token.op[2] = '\0';
                break;
            }
            token.op[1] = '\0';
            cout << " !  ERROR";
            break;
        default:
            cout << "unknown symbol:" << buffer[pos] << " Error Input at: " << pos + 1 << endl;
        }
        pos++;
    }
    else
        token.ID = ENDINPUT;
}

void init()
{
    keywords["if"] = IF;
    keywords["else"] = ELSE;
    keywords["for"] = FOR;
    keywords["do"] = DO;
    keywords["while"] = WHILE;
    keywords["switch"] = SWITCH;
    keywords["case"] = CASE;
    keywords["default"] = DEFAULT;
    keywords["break"] = BREAK;
    keywords["continue"] = CONTINUE;
}


// 解析正则表达式并判断类型
QString parseSentense(QString fileContent)
{

    init();
    QByteArray byteArray = fileContent.toUtf8();  // 将 QString 转换为 UTF-8 字节数组
    std::memcpy(buffer, byteArray.constData(), byteArray.size());  // 将 QString 转换为字符数组 buffer
    pos = 0;
    QString result;  // 用于拼接输出的结果
    GetToken(); // 获取token

    while (token.ID != ENDINPUT)
    {
        if (token.ID <= 9)
            result += "Keyword " + string(token.word) + "\n";
        else if (token.ID == DECIMAL || token.ID == OCTAL || token.ID == HEX)
            result += "Number " + QString::number(token.val) + "\n";
        else if (token.ID == ID)
            result += "Word " + string(token.word) + "\n";
        else
            result += "Operator " + string(token.op) + "\n";
        GetToken();
    }

    return result;
}
