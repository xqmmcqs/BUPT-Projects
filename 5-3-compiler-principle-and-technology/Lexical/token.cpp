/**
 * @file      token.cpp
 * @brief     记号数据结构
 * @author    Ziheng Mao
 * @date      2021/9/24
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "token.h"

using namespace std;

namespace Lexical
{
    const char * token_type_literal[] = {"Unknown", "Keyword", "Identifier", "Integer Constant", "Float Constant",
                                         "Character Constant", "String Literal", "Punctuator", "Preprocessing",
                                         "End Of File"};
    
    const char * keyword_literal[] = {"", "auto", "break", "case", "char", "const", "continue", "default", "do",
                                      "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int",
                                      "long", "register", "restrict", "return", "short", "signed", "sizeof", "static",
                                      "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
                                      "_Bool", "_Complex", "_Imaginary"};
    
    const char * punctuator_literal[] = {"", "[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*", "+", "-",
                                         "~", "!", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|",
                                         "&&", "||", "?", ":", ";", "...", "=", "*=", "/=", "%=", "+=", "-=", "<<=",
                                         ">>=", "&=", "^=", "|=", ","};
    
    ostream &operator<<(ostream &os, const TokenLocation &location)
    {
        os << location.file_name + ":" + to_string(location.row) + ":" + to_string(location.column);
        return os;
    }
    
    ostream &operator<<(ostream &os, const Token &token)
    {
        os << "[Location] " << token.location << '\n';
        os << "[Type] " << token_type_literal[static_cast<size_t>(token.type)] << '\n';
        if (token.type == TokenType::UNKNOWN)
        {
            os << "[Value] ";
            if (token.unknown == Unknown::NONE)
                os << "None";
            else if (token.unknown == Unknown::UNEXPECTED_HASH)
                os << "Unexpected Hash";
            else if (token.unknown == Unknown::UNKNOWN_CHARACTER)
                os << "Unknown Character " << token.literal;
            else if (token.unknown == Unknown::UNCLOSED_COMMENT)
                os << "Unclosed Comment";
            else if (token.unknown == Unknown::ILLEGAL_NUMBER)
                os << "Illegal Number " << token.literal;
            else if (token.unknown == Unknown::UNCLOSED_CHAR)
                os << "Unclosed Character Constant " << token.literal;
            else if (token.unknown == Unknown::UNCLOSED_STRING)
                os << "Unclosed String Literal " << token.literal;
            else if (token.unknown == Unknown::SLASH_NEWLINE_AT_EOF)
                os << "Slash Newline at EOF";
        }
        else if (token.type == TokenType::KEYWORD)
            os << "[Value] " << keyword_literal[token.keyword];
        else if (token.type == TokenType::IDENTIFIER || token.type == TokenType::INTEGER_CONSTANT ||
                 token.type == TokenType::FLOAT_CONSTANT || token.type == TokenType::CHAR_CONSTANT ||
                 token.type == TokenType::STRING_LITERAL)
            os << "[Value] " << token.literal;
        else if (token.type == TokenType::PUNCTUATOR)
            os << "[Value] " << punctuator_literal[static_cast<size_t>(token.punctuator)];
        os << '\n';
        return os;
    }
}
