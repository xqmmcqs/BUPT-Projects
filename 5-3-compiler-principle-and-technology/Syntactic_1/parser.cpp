/**
 * @file      parser.cpp
 * @brief     预测分析
 * @author    Ziheng Mao
 * @date      2021/10/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "parser.h"
#include <sstream>
#include <iostream>

using namespace std;

namespace Syntactic
{
    Parser::Parser(const Syntactic::Grammar &grammar) : start(grammar.GetStart()), nonterminal(grammar.GetNonterminal())
    {
        for (const auto &i: grammar.GetTerminal())
            terminal.push_back(i);
        for (const auto &i: grammar.GetNonterminal()) // 非终结符号
        {
            auto iter1 = grammar.GetProduction().at(i).begin(); // 候选式
            auto iter2 = grammar.GetCandidateFirst().at(i).begin(); // 候选式的FIRST集
            for (long j = 0; j < grammar.GetProduction().at(i).size(); ++j)
            {
                for (const auto &k: *(iter2 + j)) // 候选式的FIRST集中的符号
                    if (grammar.GetTerminal().find(k) != grammar.GetTerminal().end())
                        parsing_table[i][k] = *(iter1 + j);
                if ((iter2 + j)->find(EMPTY) != (iter2 + j)->end())
                    for (const auto &k: grammar.GetFollow().at(i))
                        parsing_table[i][k] = *(iter1 + j);
            }
        }
    }
    
    bool Parser::NextStep(Status &status, Production &production)
    {
        Symbol X = status.first.back(), a = status.second[0];
        if (nonterminal.find(X) == nonterminal.end())
        {
            if (X == a)
            {
                status.first.pop_back();
                status.second.pop_front();
                return true;
            }
            else return false;
        }
        else
        {
            if (!parsing_table[X][a].empty())
            {
                status.first.pop_back();
                for (auto iter = parsing_table[X][a].rbegin(); iter != parsing_table[X][a].rend(); ++iter)
                    if (*iter != EMPTY)
                        status.first.push_back(*iter);
                production.first = X;
                production.second = parsing_table[X][a];
                return true;
            }
            else return false;
        }
    }
    
    bool Parser::ParseString(const ProductionRight &str)
    {
        Status status;
        status.first.emplace_back("$");
        status.first.push_back(start);
        status.second = str;
        if (str.back() != "$")
            status.second.emplace_back("$");
        cout << left;
        cout << setw(15) << "Stack" << status.first << endl;
        cout << setw(15) << "Input" << status.second << endl;
        cout << setw(15) << "Output" << ' ' << endl;
        cout << endl;
        do
        {
            Production step;
            if (!NextStep(status, step))
                return false;
            cout << setw(15) << "Stack" << status.first << endl;
            cout << setw(15) << "Input" << status.second << endl;
            cout << setw(15) << "Output" << step << endl;
            cout << endl;
        } while (status.first.back() != "$");
        cout << right;
        return true;
    }
    
    std::ostream &operator<<(ostream &os, const vector<Symbol> &rhs)
    {
        for (const auto &i: rhs)
            os << i << ' ';
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Production &rhs)
    {
        if (rhs.first.empty())
            return os;
        os << rhs.first << " -> ";
        for (const auto &i: rhs.second)
            os << i << " ";
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Parser &rhs)
    {
        os << "[Parsing Table]" << endl;
        os << left;
        for (const auto &i: rhs.nonterminal)
        {
            os << setw(15) << "Nonterminal" << i << endl;
            for (const auto &j: rhs.terminal)
            {
                if (rhs.parsing_table.at(i).find(j) != rhs.parsing_table.at(i).end())
                {
                    stringstream ss;
                    os << "\t" << setw(15) << "Terminal" << j << endl;
                    os << "\t" << setw(15) << "Production" << make_pair(i, rhs.parsing_table.at(i).at(j)) << endl;
                }
            }
            os << endl;
        }
        os << right;
        return os;
    }
}
