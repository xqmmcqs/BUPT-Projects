/**
 * @file      grammar.cpp
 * @brief     LL(1)文法
 * @author    Ziheng Mao
 * @date      2021/10/21
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "grammar.h"
#include <sstream>
#include <iostream>

using namespace std;

namespace Syntactic
{
    const std::string EMPTY = "[empty]";
    
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
                if (production.find(str) == production.end())
                    production[str] = {};
                string left = str;
                ss >> str;
                if (str != "->")
                    continue;
                
                while (!ss.eof())
                {
                    production[left].emplace_back();
                    auto q = production[left].end() - 1;
                    while (!ss.eof())
                    {
                        ss >> str;
                        if (str == "$")
                            break;
                        if (nonterminal.find(str) == nonterminal.end() && terminal.find(str) == terminal.end())
                        {
                            cout << "Illegal Grammar." << endl;
                            exit(1);
                        }
                        q->push_back(str);
                    }
                }
            }
        }
    }
    
    void Grammar::EliminateLeftRecursion()
    {
        Productions new_production;
        for (auto &i: production)
        {
            bool recursion = false;
            Symbol new_nonterminal = i.first; // 新的非终结符
            while (nonterminal.find(new_nonterminal) != nonterminal.end())
                new_nonterminal += "'";
            // 判断是否有左递归
            for (auto &right: i.second)
            {
                if (right[0] == i.first) // 有左递归
                {
                    recursion = true;
                    new_production[new_nonterminal] = {{EMPTY}};
                    nonterminal.insert(new_nonterminal);
                    break;
                }
            }
            if (!recursion)
            {
                new_production[i.first] = std::move(i.second);
                continue;
            }
            for (auto &right: i.second)
            {
                if (right[0] == i.first) // 有左递归
                {
                    right.pop_front();
                    right.push_back(new_nonterminal);
                    new_production[new_nonterminal].push_back(std::move(right));
                }
                else
                {
                    right.push_back(new_nonterminal);
                    new_production[i.first].push_back(std::move(right));
                }
            }
        }
        production = std::move(new_production);
    }
    
    void Grammar::EliminateLeftFactoring()
    {
        Productions final_production; // 消除完左公因子的产生式
        while (true)
        {
            Productions new_production; // 消左公因子的过程中产生的新的非终结符的产生式
            for (auto &i: production)
            {
                while (true) // 对于当前非终结符消左公因子
                {
                    bool left_factoring = false;
                    for (auto iter1 = i.second.begin(); iter1 != i.second.end(); ++iter1) // 枚举第一个产生式右部
                        for (auto iter2 = iter1 + 1; iter2 != i.second.end(); ++iter2) // 枚举第二个产生式右部
                        {
                            if ((*iter1)[0] != (*iter2)[0]) // 没有公共前缀
                                continue;
                            Symbol new_nonterminal = i.first; // 新的非终结符
                            while (nonterminal.find(new_nonterminal) != nonterminal.end())
                                new_nonterminal += "'";
                            nonterminal.insert(new_nonterminal);
                            if (!left_factoring)
                                left_factoring = true;
                            new_production[new_nonterminal] = {};
                            // 找最长公共前缀
                            size_t length = 0;
                            for (; length < min(iter1->size(), iter2->size()); ++length)
                                if ((*iter1)[length] != (*iter2)[length])
                                    break;
                            ProductionRight pre(iter1->begin(), iter1->begin() + length); // 公共前缀
                            // 在其他候选式中找这个前缀
                            for (auto iter3 = i.second.begin(); iter3 != i.second.end(); ++iter3)
                            {
                                if (iter3 == iter1 || iter3 == iter2)
                                    continue;
                                size_t j = 0;
                                for (; j < min(iter3->size(), length); ++j)
                                    if (pre[j] != (*iter3)[j])
                                        break;
                                if (j != length) // 不匹配公共前缀
                                    new_production[i.first].push_back(std::move(*iter3));
                                else // 匹配公共前缀
                                {
                                    while (j--) // 去掉前缀
                                        iter3->pop_front();
                                    if (iter3->empty())
                                        iter3->push_back(EMPTY);
                                    new_production[new_nonterminal].push_back(std::move(*iter3));
                                }
                            }
                            // 处理*iter1和*iter2
                            while (length--)
                            {
                                iter1->pop_front();
                                iter2->pop_front();
                            }
                            if (iter1->empty())
                                iter1->push_back(EMPTY);
                            if (iter2->empty())
                                iter2->push_back(EMPTY);
                            new_production[new_nonterminal].push_back(std::move(*iter1));
                            new_production[new_nonterminal].push_back(std::move(*iter2));
                            pre.push_back(new_nonterminal);
                            new_production[i.first].push_back(std::move(pre));
                            break;
                        }
                    if (!left_factoring)
                        break;
                    i.second = std::move(new_production[i.first]);
                    new_production.erase(i.first);
                }
                final_production[i.first] = std::move(i.second);
            }
            if (new_production.empty())
                break;
            production = std::move(new_production);
        }
        production = std::move(final_production);
    }
    
    void Grammar::ConstructFirst(const Symbol &left)
    {
        for (const auto &i: production[left])
        {
            if (i.size() == 1 && i[0] == EMPTY)
            {
                first[left].insert(EMPTY);
                candidate_first[left].push_back({EMPTY});
                continue;
            }
            SymbolSet cur_first;
            auto iter = i.begin();
            for (; iter != i.end(); ++iter)
            {
                if (terminal.find(*iter) != terminal.end()) // *iter是终结符
                {
                    cur_first.insert(*iter);
                    break;
                }
                if (first.find(*iter) == first.end()) // *iter是非终结符，其FIRST集还未计算
                    ConstructFirst(*iter);
                cur_first.insert(first[*iter].begin(), first[*iter].end()); // 当前候选式的FIRST集并*iter的FIRST集
                if (first[*iter].find(EMPTY) == first[*iter].end())
                    break;
            }
            if (iter == i.end())
                cur_first.insert(EMPTY);
            first[left].insert(cur_first.begin(), cur_first.end()); // 非终结符的FIRST集并当前候选式的FIRST集
            candidate_first[left].push_back(std::move(cur_first));
        }
    }
    
    void Grammar::ConstructFirstSet()
    {
        for (const auto &i: nonterminal)
            if (first.find(i) == first.end())
                ConstructFirst(i);
    }
    
    void
    Grammar::ConstructFollow(const Symbol &left, unordered_map<Symbol, unordered_map<Symbol, bool>> &include_follow)
    {
        static unordered_map<Symbol, bool> finished_construct_follow; // 非终结符已经创建完FOLLOW集
        for (const auto &i: nonterminal)
            for (const auto &j: production[i])
            {
                for (auto iter1 = j.begin(); iter1 != j.end(); ++iter1)
                {
                    if (*iter1 == left)
                    {
                        auto iter2 = iter1 + 1;
                        for (; iter2 != j.end(); ++iter2)
                        {
                            if (terminal.find(*iter2) != terminal.end()) // *iter是终结符
                            {
                                follow[left].insert(*iter2);
                                break;
                            }
                            follow[left].insert(first[*iter2].begin(), first[*iter2].end()); // 非终结符，并*iter2的FOLLOW集
                            if (first[*iter2].find(EMPTY) == first[*iter2].end()) // 如果*iter2无法导出EMPTY
                                break;
                            follow[left].erase(EMPTY);
                        }
                        if (iter2 == j.end() && i != left)
                        {
                            include_follow[left][i] = true; // left的FOLLOW集包含i的FOLLOW集
                            if (!include_follow[i][left] && !finished_construct_follow[i])
                                ConstructFollow(i, include_follow);
                            follow[left].insert(follow[i].begin(), follow[i].end());
                        }
                    }
                }
            }
        finished_construct_follow[left] = true;
    }
    
    void Grammar::ConstructFollowSet()
    {
        terminal.insert("$");
        follow[start].insert("$");
        unordered_map<Symbol, unordered_map<Symbol, bool>> include_follow;
        for (const auto &i: nonterminal)
            if (follow.find(i) == follow.end())
                ConstructFollow(i, include_follow);
        for (const auto &i: nonterminal)
            for (const auto &j: nonterminal)
                if (include_follow[i][j] && include_follow[j][i]) // 如果两个FOLLOW集相互包含
                {
                    follow[i].insert(follow[j].begin(), follow[j].end());
                    follow[j] = follow[i];
                }
    }
    
    bool Grammar::IsLL1Grammar() const
    {
        for (const auto &i: nonterminal)
        {
            for (auto iter1 = candidate_first.at(i).begin(); iter1 != candidate_first.at(i).end(); ++iter1)
            {
                for (auto iter2 = iter1 + 1; iter2 != candidate_first.at(i).end(); ++iter2)
                {
                    for (const auto &j: *(iter2))
                        if (iter1->find(j) != iter1->end())
                            return false;
                }
                if (first.at(i).find(EMPTY) != first.at(i).end())
                    for (const auto &j: follow.at(i))
                        if (iter1->find(j) != iter1->end())
                            return false;
            }
        }
        return true;
    }
    
    bool Grammar::ConvertToLL1()
    {
        cout << "[Nonterminal Symbols] " << nonterminal << endl;
        cout << "[Terminal Symbols] " << terminal << endl;
        cout << "[Start Symbol] " << start << endl;
        cout << "[Productions]\n" << production << endl;
        EliminateLeftRecursion();
        EliminateLeftFactoring();
        cout << "[Nonterminal symbols] " << nonterminal << endl;
        cout << "[Terminal symbols] " << terminal << endl;
        cout << "[Start symbol] " << start << endl;
        cout << "[Productions]\n" << production << endl;
        ConstructFirstSet();
        ConstructFollowSet();
        cout << left;
        cout << "[FIRST set of candidates]" << endl;
        for (const auto &i: nonterminal)
        {
            auto iter1 = production[i].begin(); // 候选式
            auto iter2 = candidate_first[i].begin(); // 候选式的FIRST集
            for (long j = 0; j < production[i].size(); ++j)
            {
                cout << setw(15) << "Candidate" << *(iter1 + j) << endl;
                cout << setw(15) << "FIRST" << *(iter2 + j) << endl;
            }
        }
        cout << endl;
        cout << "[FIRST and FOLLOW set of nonterminals]" << endl;
        for (const auto &i: nonterminal)
        {
            cout << setw(15) << "Nonterminals" << i << endl;
            cout << setw(15) << "FIRST" << first[i] << endl;
            cout << setw(15) << "FOLLOW" << follow[i] << endl;
        }
        cout << right << endl;
        return IsLL1Grammar();
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
    
    std::ostream &operator<<(ostream &os, const Productions &rhs)
    {
        for (const auto &i: rhs)
        {
            os << i.first << " -> ";
            for (const auto &j: i.second)
                os << j << "| ";
            os << endl;
        }
        return os;
    }
}
