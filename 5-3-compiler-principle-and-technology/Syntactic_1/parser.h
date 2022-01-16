/**
 * @file      parser.h
 * @brief     预测分析
 * @author    Ziheng Mao
 * @date      2021/10/23
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef SYNTACTIC_PARSER_H
#define SYNTACTIC_PARSER_H

#include "grammar.h"

namespace Syntactic
{
    using Production = std::pair<Symbol, ProductionRight>;
    
    class Parser
    {
    public:
        Parser() = default;
        
        /**
         * @brief 从LL(1)文法构造预测分析表
         * @param grammar LL(1)文法
         */
        explicit Parser(const Grammar &grammar);
        
        /**
         * @brief 利用预测分析表分析符号串
         * @param str 符号串
         * @return 如果分析成功，返回true；否则返回false
         */
        bool ParseString(const ProductionRight &str);
    
    private:
        using Status = std::pair<std::vector<Symbol>, ProductionRight>;
        Symbol start; ///< 起始符号
        SymbolSet nonterminal; ///< 非终结符号集
        std::vector<Symbol> terminal; ///< 终结符号集
        std::unordered_map<Symbol, std::unordered_map<Symbol, ProductionRight>> parsing_table; ///< 预测分析表
        
        /**
         * @brief 进行一步预测分析
         * @param status 当前分析状态
         * @param production 返回使用的文法产生式
         * @return 如果分析成功，返回true；否则返回false
         */
        bool NextStep(Status &status, Production &production);
        
        /**
         * @brief 输出预测分析表
         * @param os 输出流
         * @param rhs 预测分析表
         * @return 输出流
         */
        friend std::ostream &operator<<(std::ostream &os, const Parser &rhs);
    };
    
    std::ostream &operator<<(std::ostream &os, const std::vector<Symbol> &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Production &rhs);
    
    std::ostream &operator<<(std::ostream &os, const Parser &rhs);
}

#endif //SYNTACTIC_PARSER_H
