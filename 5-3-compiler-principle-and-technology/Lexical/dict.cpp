/**
 * @file      dict.cpp
 * @brief     词典
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "dict.h"

using namespace std;

namespace Lexical
{
    Keyword Dict::find(const std::string &str) const
    {
        auto node = &root;
        for (char iter: str)
        {
            if (!node->child[char2Index(iter)])return 0;
            node = node->child[char2Index(iter)];
        }
        return node->keyword;
    }
    
    void Dict::insert(Keyword keyword, const std::string &str)
    {
        auto node = &root;
        for (auto iter: str)
        {
            if (!node->child[char2Index(iter)])node->child[char2Index(iter)] = new TrieNode;
            node = node->child[char2Index(iter)];
        }
        node->keyword = keyword;
    }
    
    size_t Dict::char2Index(char c)
    {
        if (c >= 'a' && c <= 'z')return c - 'a';
        if (c >= 'A' && c <= 'Z')return c - 'A' + 26;
        if (c >= '0' && c <= '9')return c - '0' + 52;
        return 62;
    }
}