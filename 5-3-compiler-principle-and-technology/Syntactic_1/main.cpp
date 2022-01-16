/**
 * @file      main.cpp
 * @author    Ziheng Mao
 * @date      2021/10/21
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include <iostream>

#include "parser.h"

using namespace Syntactic;
using namespace std;

int main(int argc, char * argv[])
{
    Grammar grammar;
    ifstream grammar_fs(argv[1]);
    grammar.LoadFromFile(grammar_fs);
    if (!grammar.ConvertToLL1())
    {
        cout << "Not LL(1) Grammar.";
        return 1;
    }
    Parser parser(grammar);
    cout << parser << endl;
    ifstream str_fs(argv[2]);
    ProductionRight str;
    while (!str_fs.eof())
    {
        Symbol symbol;
        str_fs >> symbol;
        str.push_back(symbol);
    }
    if (!parser.ParseString(str))
    {
        cout << "Parsing failed!" << endl;
        return 1;
    }
    cout << "Parsing successful!" << endl;
    return 0;
}
