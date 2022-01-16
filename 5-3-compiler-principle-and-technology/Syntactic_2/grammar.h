/**
 * @file      grammar.h
 * @brief     LR(1)文法及项目集规范族
 * @author    Ziheng Mao
 * @date      2021/11/20
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef SYNTACTIC_GRAMMAR_H
#define SYNTACTIC_GRAMMAR_H

#include <vector>
#include <string>
#include <deque>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

namespace Syntactic
{
    using Symbol = std::string;
    using SymbolSet = std::unordered_set<Symbol>;
    using ProductionRight = std::deque<std::string>;
    using Production = std::pair<Symbol, ProductionRight>;
    using Productions = std::unordered_map<Symbol, std::vector<Production>>;
    using Item = std::pair<const Production *, ProductionRight::const_iterator>;
    
    extern const std::string EMPTY;
    
    /// LR(1)有效项目集
    struct ItemSet
    {
        std::map<Item, SymbolSet> shift_item; ///< 移进项目或待约项目
        std::map<Item, SymbolSet> reduce_item; ///< 归约项目
        
        bool operator==(const ItemSet &rhs);
    };
    
    /// 识别所有活前缀的DFA
    struct DFA
    {
        std::vector<ItemSet *> states; ///< 状态集合
        std::vector<std::unordered_map<Symbol, int>> next; ///< 状态转移
    };
    
    class Grammar
    {
    public:
        Grammar() = default;
    
        /**
         * @brief 从文件中读取文法
         * @param fs 文件输入流
         */
        void LoadFromFile(std::ifstream &fs);
    
        /**
         * @brief 构建LR(1)项目集规范族及识别所有活前缀的DFA
         * @return 如果是LR(1)文法，返回true；否则返回false
         */
        bool ConstructLR1StateMachine();
        
        const Symbol &GetStart() const
        { return start; }
        
        const SymbolSet &GetTerminal() const
        { return terminal; }
        
        const DFA &GetDfa() const
        { return dfa; }
    
    private:
        SymbolSet nonterminal; ///< 非终结符号集
        SymbolSet terminal; ///< 终结符号集
        Productions productions; ///< 产生式
        Symbol start; ///< 起始符号
        std::unordered_map<Symbol, SymbolSet> first; ///< 非终结符号的FIRST集
        std::map<Item, SymbolSet> item_first; ///< 待约项目的FIRST集
        DFA dfa; ///< 识别所有活前缀的DFA
        
        /**
         * @brief 求LR(1)项目集合的闭包
         * @param item_set LR(1)项目集合
         */
        void Closure(ItemSet &item_set);
    
        /**
         * @brief 构建非终结符号left的FIRST集
         * @param left 一个非终结符号
         */
        void ConstructFirst(const Symbol &left);
    
        /**
         * @brief 构建所有待约项目的FIRST集
         */
        void ConstructItemFirst();
        
        /**
         * @brief 拓广文法
         */
        void ExtendGrammar();
        
        /**
         * @brief 构造识别所有活前缀的DFA
         */
        void ConstructDFA();
        
        /**
         * @brief 判断该文法是否是LR(1)文法
         * @return 如果是，返回true；否则返回false
         */
        bool IsLR1Grammar() const;
    };
    
    template<typename T>
    bool InSet(const std::unordered_set<T> &a, const T &b)
    {
        return a.find(b) != a.end();
    }
    
    template<typename T1, typename T2>
    bool InSet(const std::unordered_map<T1, T2> &a, const T1 &b)
    {
        return a.find(b) != a.end();
    }
    
    std::ostream &operator<<(std::ostream &os, const SymbolSet &rhs);
    
    std::ostream &operator<<(std::ostream &os, const ProductionRight &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Production &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Productions &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Item &rhs);
    
    std::ostream &operator<<(std::ostream &os, const ItemSet &rhs);
    
    std::ostream &operator<<(std::ostream &os, const DFA &rhs);
}

#endif //SYNTACTIC_GRAMMAR_H
