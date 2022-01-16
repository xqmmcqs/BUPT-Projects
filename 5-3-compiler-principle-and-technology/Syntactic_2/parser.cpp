/**
 * @file      parser.cpp
 * @brief     LR预测分析
 * @author    Ziheng Mao
 * @date      2021/11/20
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "parser.h"
#include <iostream>

using namespace std;

namespace Syntactic
{
    Parser::Parser(const Grammar &grammar) : terminal(grammar.GetTerminal())
    {
        const DFA &dfa = grammar.GetDfa();
        const Symbol &start = grammar.GetStart();
        for (int i = 0; i < dfa.states.size(); ++i)
        {
            action_table.emplace_back();
            goto_table.emplace_back();
            for (const auto &j: dfa.next[i])
                if (InSet(terminal, j.first))
                    action_table[i][j.first] = {.action_class = SHIFT, .index = j.second};
                else
                    goto_table[i][j.first] = j.second;
            for (const auto &j: dfa.states[i]->reduce_item)
                for (const auto &k: j.second)
                {
                    if (j.first.first->first == start)
                        action_table[i][k] = {.action_class = ACC};
                    else
                        action_table[i][k] = {.action_class = REDUCE, .production = j.first.first};
                }
        }
    }
    
    int Parser::NextStep(Parser::Status &status) const
    {
        int S = status.first.first.back();
        Symbol a = status.second.front();
        if (!InSet(action_table[S], a))
        {
            cout << "Error!" << endl;
            return -1;
        }
        auto action = action_table[S].at(a);
        if (action.action_class == SHIFT) // 移进
        {
            status.first.first.push_back(action.index);
            status.first.second.push_back(a);
            status.second.pop_front();
            cout << "Shift " << action.index << endl;
        }
        else if (action.action_class == REDUCE) // 归约
        {
            for (int i = 0; i < action.production->second.size(); ++i)
            {
                status.first.first.pop_back();
                status.first.second.pop_back();
            }
            status.first.first.push_back(goto_table[status.first.first.back()].at(action.production->first));
            status.first.second.push_back(action.production->first);
            cout << "Reduce by " << *action.production << endl;
        }
        else
        {
            cout << "Accepted!" << endl;
            return 1;
        }
        return 0;
    }
    
    bool Parser::ParseString(const ProductionRight &str)
    {
        Status status;
        status.first.first.emplace_back(0);
        status.first.second.emplace_back("$");
        status.second = str;
        if (str.back() != "$")
            status.second.emplace_back("$");
        cout << left;
        do
        {
            cout << "Stack\n" << status.first << endl;
            cout << setw(15) << "Input" << status.second << endl;
            cout << setw(15) << "Output";
            int result = NextStep(status);
            cout << endl;
            if (result == -1)
                return false;
            if (result == 1)
            {
                cout << right;
                return true;
            }
        } while (true);
    }
    
    std::ostream &operator<<(ostream &os, const Parser::ActionItem &action_item)
    {
        switch (action_item.action_class)
        {
            case Parser::SHIFT:
                os << "S " << action_item.index;
                break;
            case Parser::REDUCE:
                os << "R " << *action_item.production;
                break;
            case Parser::ACC:
                os << "ACC";
                break;
        }
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Parser &rhs)
    {
        os << left;
        os << "[Parsing Table]" << endl;
        for (int i = 0; i < rhs.action_table.size(); ++i)
        {
            os << "State " << i << endl;
            for (const auto &j: rhs.action_table[i])
            {
                os << '\t' << setw(15) << "Nonterminal" << j.first << endl;
                os << '\t' << setw(15) << "Action" << j.second << endl;
            }
            for (const auto &j: rhs.goto_table[i])
            {
                os << '\t' << setw(15) << "Terminal" << j.first << endl;
                os << '\t' << setw(15) << "Goto" << j.second << endl;
            }
        }
        os << right;
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const pair<std::vector<int>, std::vector<Symbol>> &rhs)
    {
        for (const auto &i: rhs.first)
            os << setw(4) << i;
        os << endl;
        for (const auto &i: rhs.second)
            os << setw(4) << i;
        return os;
    }
}