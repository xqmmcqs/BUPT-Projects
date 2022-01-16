/**
 * @file      grammar.cpp
 * @brief     LR(1)文法及项目集规范族
 * @author    Ziheng Mao
 * @date      2021/11/20
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "grammar.h"

#include <sstream>
#include <iostream>
#include <queue>

using namespace std;

namespace Syntactic
{
    const std::string EMPTY = "[empty]";
    
    bool ItemSet::operator==(const ItemSet &rhs)
    {
        if (shift_item.size() != rhs.shift_item.size() || reduce_item.size() != rhs.reduce_item.size())
            return false;
        // 判断本集合中的LR(1)项目是否都在另一个集合中出现
        for (const auto &i: shift_item)
        {
            auto j = rhs.shift_item.find(i.first);
            if (j == rhs.shift_item.end() || i.second != j->second)
                return false;
        }
        for (const auto &i: reduce_item)
        {
            auto j = rhs.reduce_item.find(i.first);
            if (j == rhs.reduce_item.end() || i.second != j->second)
                return false;
        }
        return true;
    }
    
    void Grammar::LoadFromFile(ifstream &fs)
    {
        string str;
        getline(fs, str);
        if (str == "$ Nonterminal symbols")
        {
            while (true)
            {
                fs >> str;
                if (str == "$")
                {
                    fs.putback('$');
                    break;
                }
                nonterminal.insert(str);
            }
        }
        getline(fs, str);
        if (str == "$ Terminal symbols")
        {
            while (true)
            {
                fs >> str;
                if (str == "$")
                {
                    fs.putback('$');
                    break;
                }
                if (nonterminal.find(str) != nonterminal.end())
                {
                    cout << "Illegal Grammar." << endl;
                    exit(1);
                }
                terminal.insert(str);
            }
        }
        getline(fs, str);
        if (str == "$ Start symbol")
        {
            fs >> str;
            start = str;
            if (nonterminal.find(str) == nonterminal.end())
            {
                cout << "Illegal Grammar." << endl;
                exit(1);
            }
            getline(fs, str);
        }
        getline(fs, str);
        if (str == "$ Productions")
        {
            while (!fs.eof())
            {
                getline(fs, str);
                istringstream ss(str);
                ss >> str;
                if (nonterminal.find(str) == nonterminal.end())
                {
                    cout << "Illegal Grammar." << endl;
                    exit(1);
                }
                if (!InSet(productions, str))
                    productions[str] = {};
                string left = str;
                ss >> str;
                if (str != "->")
                    continue;
                
                while (!ss.eof())
                {
                    productions[left].emplace_back();
                    auto q = productions[left].end() - 1;
                    q->first = left;
                    while (!ss.eof())
                    {
                        ss >> str;
                        if (str == "$")
                            break;
                        if (str != EMPTY && nonterminal.find(str) == nonterminal.end() &&
                            terminal.find(str) == terminal.end())
                        {
                            cout << "Illegal Grammar." << endl;
                            exit(1);
                        }
                        if (str == EMPTY)continue;
                            q->second.push_back(str);
                    }
                }
            }
        }
    }
    
    void Grammar::ConstructFirst(const Symbol &left)
    {
        first[left] = {};
        for (const auto &i: productions[left])
        {
            auto right = i.second;
            if (right.empty())
            {
                first[left].insert(EMPTY);
                continue;
            }
            SymbolSet cur_first;
            auto iter = right.begin();
            for (; iter != right.end(); ++iter)
            {
                if (InSet(terminal, *iter)) // *iter是终结符
                {
                    cur_first.insert(*iter);
                    break;
                }
                if (!InSet(first, *iter)) // *iter是非终结符，其FIRST集还未计算
                    ConstructFirst(*iter);
                cur_first.insert(first[*iter].begin(), first[*iter].end()); // 当前候选式的FIRST集并*iter的FIRST集
                if (InSet(first[*iter], EMPTY))
                    break;
            }
            if (iter == right.end())
                cur_first.insert(EMPTY);
            first[left].insert(cur_first.begin(), cur_first.end()); // 非终结符的FIRST集并当前候选式的FIRST集
        }
    }
    
    void Grammar::ConstructItemFirst()
    {
        for (const auto &i: nonterminal)
            for (auto &j: productions[i])
            {
                auto right = &j.second;
                for (auto iter1 = right->cbegin(); iter1 != right->cend(); ++iter1)
                    if (InSet(nonterminal, *iter1)) // 找到候选式中的非终结符
                    {
                        auto cur_item_first = &item_first[make_pair(&j, iter1)]; // 记录项目对应的地址，避免重复寻址
                        for (auto iter2 = iter1 + 1; iter2 != right->cend(); ++iter2) // 从其后的第一个字符开始求FIRST
                        {
                            if (InSet(terminal, *iter2))
                            {
                                cur_item_first->insert(*iter2);
                                break;
                            }
                            if (!InSet(first, *iter2))
                                ConstructFirst(*iter2);
                            cur_item_first->insert(first[*iter2].begin(), first[*iter2].end());
                            if (InSet(first[*iter2], EMPTY))
                                break;
                        }
                    }
            }
    }
    
    void Grammar::Closure(ItemSet &item_set)
    {
        do
        {
            auto new_item_set(item_set);
            for (const auto &i: item_set.shift_item)
            {
                if (InSet(nonterminal, *i.first.second)) // 是待约项目
                {
                    for (auto &j: productions[*i.first.second])
                    {
                        auto new_item_lookahead = !j.second.empty() ?
                                                  &new_item_set.shift_item[make_pair(&j, j.second.cbegin())] :
                                                  &new_item_set.reduce_item[make_pair(&j, j.second.cbegin())];
                        auto following_first = &item_first.at(i.first);
                        if (following_first->empty() ||
                            InSet(*following_first, EMPTY)) // 如果非终结符后面子串的FIRST集包含空，则将向前看符号也加入到新项目的向前看符号集合中
                            new_item_lookahead->insert(i.second.begin(), i.second.end());
                        for (const auto &k: *following_first)
                            if (k != EMPTY)
                                new_item_lookahead->insert(k);
                    }
                }
            }
            if (new_item_set == item_set)break;
            item_set = std::move(new_item_set);
        } while (true);
    }
    
    void Grammar::ExtendGrammar()
    {
        Symbol new_nonterminal = start; // 新的起始符号
        while (nonterminal.find(new_nonterminal) != nonterminal.end())
            new_nonterminal += "'";
        Production new_production;
        new_production.first = new_nonterminal;
        new_production.second.push_back(start);
        productions[new_nonterminal].push_back(std::move(new_production));
        nonterminal.insert(new_nonterminal);
        start = std::move(new_nonterminal);
    }
    
    void Grammar::ConstructDFA()
    {
        dfa.states.push_back(new ItemSet);
        dfa.states[0]->shift_item[make_pair(&productions[start][0], productions[start][0].second.cbegin())] = {"$"};
        dfa.next.emplace_back();
        Closure(*dfa.states[0]);
        queue<int> q;
        q.push(0);
        while (!q.empty())
        {
            auto index = q.front();
            auto state = dfa.states[index];
            q.pop();
            vector<ItemSet> new_states;
            for (const auto &i: state->shift_item)
            {
                if (InSet(dfa.next[index], *i.first.second))continue; // 相同的转移已经走过
                auto go_symbol = *i.first.second; // 转移的符号
                auto new_state = new ItemSet;
                if (i.first.second + 1 == i.first.first->second.cend()) // 圆点往后移动之后是归约项目
                    new_state->reduce_item[make_pair(i.first.first, i.first.second + 1)] = i.second;
                else
                    new_state->shift_item[make_pair(i.first.first, i.first.second + 1)] = i.second;
                for (const auto &j: state->shift_item)
                {
                    if (i == j || go_symbol != *j.first.second) // 圆点后符号不相同
                        continue;
                    if (j.first.second + 1 == j.first.first->second.cend())
                        new_state->reduce_item[make_pair(j.first.first, j.first.second + 1)] = j.second;
                    else
                        new_state->shift_item[make_pair(j.first.first, j.first.second + 1)] = j.second;
                }
                
                Closure(*new_state);
                for (int j = 0; j < dfa.states.size(); ++j)
                    if (*new_state == *dfa.states[j]) // 新状态是已有状态
                        dfa.next[index][go_symbol] = j;
                if (!InSet(dfa.next[index], go_symbol)) // 创建新状态
                {
                    dfa.states.push_back(new_state);
                    dfa.next.emplace_back();
                    dfa.next[index][go_symbol] = dfa.states.size() - 1;
                    q.push(dfa.states.size() - 1);
                }
            }
        }
    }
    
    bool Grammar::IsLR1Grammar() const
    {
        for (const auto &i: dfa.states)
        {
            SymbolSet shift;
            for (const auto &j: i->shift_item)
                shift.insert(*j.first.second);
            SymbolSet reduce;
            for (const auto &j: i->reduce_item)
                for (const auto &k: j.second)
                {
                    if (!InSet(shift, k) && !InSet(reduce, k))
                        reduce.insert(k); // 归约项目的向前看符号，不和移进项目的移进符号、以及其他归约项目的向前看符号冲突
                    else return false;
                }
        }
        return true;
    }
    
    bool Grammar::ConstructLR1StateMachine()
    {
        cout << "[Nonterminal Symbols] " << nonterminal << endl;
        cout << "[Terminal Symbols] " << terminal << endl;
        cout << "[Start Symbol] " << start << endl;
        cout << "[Productions]\n" << productions << endl;
        ExtendGrammar();
        cout << "[Nonterminal Symbols] " << nonterminal << endl;
        cout << "[Terminal Symbols] " << terminal << endl;
        cout << "[Start Symbol] " << start << endl;
        cout << "[Productions]\n" << productions << endl;
        ConstructItemFirst();
        ConstructDFA();
        cout << "[DFA]\n" << dfa << endl;
        return IsLR1Grammar();
    }
    
    std::ostream &operator<<(ostream &os, const SymbolSet &rhs)
    {
        for (const auto &i: rhs)
            os << i << ' ';
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const ProductionRight &rhs)
    {
        for (const auto &i: rhs)
            os << i << ' ';
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Production &rhs)
    {
        os << rhs.first << " -> " << rhs.second;
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Productions &rhs)
    {
        for (const auto &i: rhs)
            for (const auto &j: i.second)
                os << j << endl;
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const Item &rhs)
    {
        os << rhs.first->first << " ->";
        for (auto i = rhs.first->second.cbegin(); i != rhs.second; ++i)
            os << " " << *i;
        os << " · " << ProductionRight(rhs.second, rhs.first->second.cend());
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const ItemSet &rhs)
    {
        for (const auto &i: rhs.shift_item)
            os << i.first << "    " << i.second << endl;
        for (const auto &i: rhs.reduce_item)
            os << i.first << "    " << i.second << endl;
        return os;
    }
    
    std::ostream &operator<<(ostream &os, const DFA &rhs)
    {
        for (int i = 0; i < rhs.states.size(); ++i)
        {
            os << "I" << i << ":\n";
            os << *rhs.states[i];
            for (const auto &j: rhs.next[i])
                os << "--- " << j.first << " --> I" << j.second << endl;
            os << endl;
        }
        return os;
    }
}