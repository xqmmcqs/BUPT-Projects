/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/9/24
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "lexer.h"

using namespace Lexical;
using namespace std;

int main(int argc, char * argv[])
{
    auto dict = Dict();
    for (int i = 0; i < 38; ++i) // 将关键字插入字典
        dict.insert(i, keyword_literal[i]);
    auto lexer = Lexer(argv[1], dict);
    cout << "===========Tokens============" << endl;
    Token token = lexer.GetNextToken();
    while (token.GetType() != TokenType::END_OF_FILE)
    {
        cout << token << endl;
        token = lexer.GetNextToken();
    }
    cout << "===========Analysis==========" << endl;
    cout << lexer << endl;
    return 0;
}