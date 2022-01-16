/**
 * @file      dict.h
 * @brief     词典
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef LEXICAL_DICT_H
#define LEXICAL_DICT_H

#include "token.h"

namespace Lexical
{
    /// 字典树节点
    struct TrieNode
    {
        Keyword keyword;
        TrieNode * child[63]{};
    };
    
    /// 字典树
    class Dict
    {
    public:
        
        Dict() = default;
        
        /**
         * @brief 在字典树中查找关键字
         *
         * @param str 字符串
         * @return 查找结果，如果查找不到该字符串，返回Keyword::NONE
         */
        Keyword find(const std::string &str) const;
        
        /**
         * @brief 在字典树中插入关键字
         *
         * @param keyword 关键字类型
         * @param str 关键字字符串
         */
        void insert(Keyword keyword, const std::string &str);
    
    private:
        TrieNode root; ///< 字典树根
        
        /// 将小写字母、数字和下划线映射到0-62的区间内
        static size_t char2Index(char c);
    };
}

#endif //LEXICAL_DICT_H
