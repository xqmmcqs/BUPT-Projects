/**
 * @file      lexer.h
 * @brief     词法分析器
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef LEXICAL_LEXER_H
#define LEXICAL_LEXER_H

#include "scanner.h"
#include "dict.h"

namespace Lexical
{
    class Lexer
    {
    public:
        Lexer(std::string file_name_, Dict dict_) : scanner(std::move(file_name_)), dict(dict_), lexeme_count()
        {}
    
        Token GetNextToken();
    
        friend std::ostream &operator<<(std::ostream &os, const Lexer &lexer);

    private:
        Scanner scanner;
        Dict dict;
        std::string buffer;
        TokenLocation token_location; ///< 当前记号的位置
        unsigned int lexeme_count[10]; ///< 各种记号的数量
    
        Token GetTokenHandler();
    
        void LineCommentHandler();
    
        bool BlockCommentHandler();
    
        Token PreprocessingHandler();
    
        Token NumberTokenHandler();
        
        Token CharTokenHandler();
        
        Token StringTokenHandler();
        
        Token IdentifierTokenHandler();
        
        static bool isNumberCharacter(char c);
        
        static bool isOctCharacter(char c);
        
        static bool isIntegerSuffix(const std::string &str);
        
        static bool isFloatSuffix(const std::string &str);
        
        Token DecimalIntegerHandler(std::string::iterator iter);
        
        Token OctIntegerHandler(std::string::iterator iter);
        
        Token HexIntegerHandler(std::string::iterator iter);
        
        Token DecimalFloatHandler(std::string::iterator iter);
        
        Token HexFloatHandler(std::string::iterator iter);
        
        static bool isIdentifierCharacter(char c);
    };
}

#endif //LEXICAL_LEXER_H
