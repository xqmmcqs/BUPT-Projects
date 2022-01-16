/**
 * @file      parser.h
 * @brief     LR预测分析
 * @author    Ziheng Mao
 * @date      2021/11/20
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef SYNTACTIC_PARSER_H
#define SYNTACTIC_PARSER_H

#include "grammar.h"

namespace Syntactic
{
    class Parser
    {
    public:
        Parser() = default;
    
        /**
         * @brief 从LR(1)项目集规范族构造预测分析表
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
        using Status = std::pair<std::pair<std::vector<int>, std::vector<Symbol>>, ProductionRight>;
        enum ActionClass
        {
            SHIFT,
            REDUCE,
            ACC
        };
        
        /// Action表的项目
        struct ActionItem
        {
            ActionClass action_class;
            union
            {
                int index; ///< Shift的编号
                const Production * production; ///< Reduce的产生式
            };
        };
        
        SymbolSet terminal; ///< 终结符号集
        std::vector<std::unordered_map<Symbol, ActionItem>> action_table; ///< 预测分析表
        std::vector<std::unordered_map<Symbol, int>> goto_table; ///< 预测分析表
    
        /**
         * @brief 进行一步预测分析
         * @param status 当前分析状态
         * @param production 返回使用的文法产生式
         * @return 如果分析成功完成，返回1；分析失败，返回-1；仍未分析完成，返回0
         */
        int NextStep(Status &status) const;
        
        friend std::ostream &operator<<(std::ostream &os, const Parser &rhs);
        
        friend std::ostream &operator<<(std::ostream &os, const ActionItem &action_item);
    };
    
    std::ostream &operator<<(std::ostream &os, const std::pair<std::vector<int>, std::vector<Symbol>> &rhs);
}

#endif //SYNTACTIC_PARSER_H
