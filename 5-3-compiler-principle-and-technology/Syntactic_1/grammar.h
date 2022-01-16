/**
 * @file      grammar.h
 * @brief     LL(1)文法
 * @author    Ziheng Mao
 * @date      2021/10/21
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef SYNTACTIC_GRAMMAR_H
#define SYNTACTIC_GRAMMAR_H

#include <vector>
#include <string>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

namespace Syntactic
{
    using Symbol = std::string;
    using SymbolSet = std::unordered_set<Symbol>;
    using ProductionRight = std::deque<std::string>;
    using Productions = std::unordered_map<Symbol, std::vector<ProductionRight>>;
    
    extern const std::string EMPTY;
    
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
         * @brief 将文法转换为LL(1)文法
         * @return 如果是LL(1)文法，返回true；否则返回false
         */
        bool ConvertToLL1();
        
        const SymbolSet &GetNonterminal() const
        { return nonterminal; }
        
        const SymbolSet &GetTerminal() const
        { return terminal; }
        
        const Productions &GetProduction() const
        { return production; }
        
        const Symbol &GetStart() const
        { return start; }
        
        const std::unordered_map<std::string, std::vector<SymbolSet>> &GetCandidateFirst() const
        { return candidate_first; }
        
        const std::unordered_map<std::string, SymbolSet> &GetFollow() const
        { return follow; }
    
    private:
        SymbolSet nonterminal; ///< 非终结符号集
        SymbolSet terminal; ///< 终结符号集
        Productions production; ///< 产生式
        Symbol start; ///< 起始符号
        std::unordered_map<std::string, std::vector<SymbolSet>> candidate_first; ///< 各个候选式的FIRST集
        std::unordered_map<std::string, SymbolSet> first; ///< 非终结符号的FIRST集
        std::unordered_map<std::string, SymbolSet> follow; ///< 非终结符号的FOLLOW集
        
        /**
         * @brief 消除左递归
         */
        void EliminateLeftRecursion();
        
        /**
         * @brief 消除左公因子
         */
        void EliminateLeftFactoring();
        
        /**
         * @brief 构建非终结符号left的FIRST集
         * @param left 一个非终结符号
         */
        void ConstructFirst(const Symbol &left);
        
        /**
         * @brief 构建FIRST集
         */
        void ConstructFirstSet();
        
        /**
         * @brief 构建非终结符号left的FIRST集
         * @param left 一个非终结符号
         * @param include_follow 非终结符号的FOLLOW集的包含关系
         */
        void ConstructFollow(const Symbol &left,
                             std::unordered_map<Symbol, std::unordered_map<Symbol, bool>> &include_follow);
        
        /**
         * @brief 构建FOLLOW集
         */
        void ConstructFollowSet();
        
        /**
         * @brief 判断文法是否是LL(1)文法
         * @return 如果是LL(1)文法，返回true；否则返回false
         */
        bool IsLL1Grammar() const;
    };
    
    std::ostream &operator<<(std::ostream &os, const SymbolSet &rhs);
    
    std::ostream &operator<<(std::ostream &os, const ProductionRight &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Productions &rhs);
}

#endif //SYNTACTIC_GRAMMAR_H
