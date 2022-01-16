/**
 * @file      token.h
 * @brief     记号数据结构
 * @author    Ziheng Mao
 * @date      2021/9/24
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef LEXICAL_TOKEN_H
#define LEXICAL_TOKEN_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>

namespace Lexical
{
    /// 记号类型名称
    extern const char * token_type_literal[];
    
    /// 记号类型
    enum class TokenType
    {
        UNKNOWN,
        KEYWORD,
        IDENTIFIER,
        INTEGER_CONSTANT,
        FLOAT_CONSTANT,
        CHAR_CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR,
        PREPROCESSING,
        END_OF_FILE,
    };
    
    /// 关键字名称
    extern const char * keyword_literal[];
    
    /// 关键字类型
    typedef int Keyword;
    
    /// 标点符号名称
    extern const char * punctuator_literal[];
    
    /// 标点符号类型
    enum class Punctuator
    {
        NONE,
        LEFT_SQUARE, // [
        RIGHT_SQUARE, // ]
        LEFT_PAREN, // (
        RIGHT_PAREN, // )
        LEFT_BRACE, // {
        RIGHT_BRACE, // }
        DOT, // .
        ARROW, // ->
        PLUS_PLUS, // ++
        MINUS_MINUS, // --
        AMP, // &
        STAR, // *
        PLUS, // +
        MINUS, // -
        TILDE, // ~
        EXCLAIM, // !
        SLASH, // /
        PERCENT, // %
        LESS_LESS, // <<
        GREATER_GREATER, // >>
        LESS, // <
        GREATER, // >
        LESS_EQUAL, // <=
        GREATER_EQUAL, // >=
        EQUAL_EQUAL, // ==
        EXCLAIM_EQUAL, // !=
        CARET, // ^
        PIPE, // |
        AMP_AMP, // &&
        PIPE_PIPE, // ||
        QUESTION, // ?
        COLON, // :
        SEMICOLON, // ;
        DOTS, // ...
        EQUAL, // =
        STAR_EQUAL, // *=
        SLASH_EQUAL, // /=
        PERCENT_EQUAL, // %=
        PLUS_EQUAL, // +=
        MINUS_EQUAL, // -=
        LESS_LESS_EQUAL, // <<=
        GREATER_GREATER_EQUAL, // >>=
        AMP_EQUAL, // &=
        CARET_EQUAL, // ^=
        PIPE_EQUAL, // |=
        COMMA, // ,
    };
    
    /// 未知类型
    enum class Unknown
    {
        NONE,
        UNEXPECTED_HASH,
        UNKNOWN_CHARACTER,
        UNCLOSED_COMMENT,
        ILLEGAL_NUMBER,
        UNCLOSED_CHAR,
        UNCLOSED_STRING,
        SLASH_NEWLINE_AT_EOF,
    };
    
    /// 记号位置类
    class TokenLocation
    {
    public:
        TokenLocation() = default;
        
        TokenLocation(std::string file_name_, unsigned int row_, unsigned int column_) :
                file_name(std::move(file_name_)), row(row_), column(column_)
        {}
        
        friend std::ostream &operator<<(std::ostream &os, const TokenLocation &location);
    
    private:
        std::string file_name;
        unsigned int row;
        unsigned int column;
    };
    
    /// 记号类
    class Token
    {
    public:
        Token() = default;
        
        Token(TokenType type_, TokenLocation location_, Keyword keyword_, Punctuator punctuator_, Unknown unknown_,
              std::string literal_) :
                type(type_), location(std::move(location_)), keyword(keyword_), punctuator(punctuator_),
                unknown(unknown_), literal(std::move(literal_))
        {}
        
        /// 构造关键字类型的记号
        Token(TokenType type_, TokenLocation location_, Keyword keyword_) :
                Token(type_, std::move(location_), keyword_, Punctuator::NONE, Unknown::NONE, "")
        {}
        
        /// 构造标点符号类型的记号
        Token(TokenType type_, TokenLocation location_, Punctuator punctuator_) :
                Token(type_, std::move(location_), 0, punctuator_, Unknown::NONE, "")
        {}
        
        /// 构造字面值类型的记号
        Token(TokenType type_, TokenLocation location_, std::string literal_) :
                Token(type_, std::move(location_), 0, Punctuator::NONE, Unknown::NONE, std::move(literal_))
        {}
        
        /// 构造未知类型的记号
        Token(TokenType type_, TokenLocation location_, Unknown unknown_) :
                Token(type_, std::move(location_), 0, Punctuator::NONE, unknown_, "")
        {}
        
        /// 构造带字面值的未知类型的记号
        Token(TokenType type_, TokenLocation location_, Unknown unknown_, std::string literal_) :
                Token(type_, std::move(location_), 0, Punctuator::NONE, unknown_, std::move(literal_))
        {}
        
        TokenType GetType() const
        { return type; }
        
        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    
    private:
        TokenType type; ///< 记号类型
        TokenLocation location; ///< 记号位置
        Keyword keyword; ///< 如果不为0，表示关键字类型
        Punctuator punctuator; ///< 如果不为0，表示标点符号类型
        Unknown unknown; ///< 如果不为0，表示未知类型
        std::string literal; ///< 字面值
    };
}

#endif //LEXICAL_TOKEN_H
