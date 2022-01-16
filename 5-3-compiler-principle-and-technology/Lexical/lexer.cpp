/**
 * @file      lexer.cpp
 * @brief     词法分析器
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "lexer.h"

using namespace std;

namespace Lexical
{
    Token Lexer::GetNextToken()
    {
        Token token = GetTokenHandler();
        ++lexeme_count[static_cast<size_t>(token.GetType())];
        return token;
    }
    
    Token Lexer::GetTokenHandler()
    {
        buffer.clear();
        if (scanner.Eof())
            return {TokenType::END_OF_FILE, scanner.GetLocation(), ""};
        char c;
        bool start_of_row;
        do
        {
            start_of_row = scanner.GetStartOfRow();
            if (scanner.Eof())
                return {TokenType::END_OF_FILE, scanner.GetLocation(), ""};
            c = scanner.GetChar();
        } while (isspace(c)); // 跳过空白字符
    
        token_location = scanner.GetLocation();
        switch (c)
        {
            case '[':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::LEFT_SQUARE};
            case ']':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::RIGHT_SQUARE};
            case '(':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::LEFT_PAREN};
            case ')':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::RIGHT_PAREN};
            case '{':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::LEFT_BRACE};
            case '}':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::RIGHT_BRACE};
            case '.':
                if (scanner.PeekChar(1) == '.' && scanner.PeekChar(2) == '.')
                {
                    scanner.SkipChar(2);
                    return {TokenType::PUNCTUATOR, token_location, Punctuator::DOTS};
                }
                else if (isdigit(scanner.PeekChar(1)))
                {
                    buffer.push_back(c);
                    return NumberTokenHandler(); // 小数点后接数字
                }
                else
                    return {TokenType::PUNCTUATOR, token_location, Punctuator::DOT};
            case '-':
                switch (scanner.PeekChar(1))
                {
                    case '>':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::ARROW};
                    case '-':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::MINUS_MINUS};
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::MINUS_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::MINUS};
                }
            case '+':
                switch (scanner.PeekChar(1))
                {
                    case '+':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PLUS_PLUS};
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PLUS_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PLUS};
                }
            case '&':
                switch (scanner.PeekChar(1))
                {
                    case '&':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::AMP_AMP};
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::AMP_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::AMP};
                }
            case '*':
                switch (scanner.PeekChar(1))
                {
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::STAR_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::STAR};
                }
            case '~':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::TILDE};
            case '!':
                switch (scanner.PeekChar(1))
                {
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::EXCLAIM_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::EXCLAIM};
                }
            case '/':
                switch (scanner.PeekChar(1))
                {
                    case '/':
                        scanner.SkipChar(1);
                        LineCommentHandler(); // 行内注释
                        return GetTokenHandler(); // 跳过注释后获取下一个记号
                    case '*':
                        scanner.SkipChar(1);
                        if (!BlockCommentHandler()) // 块注释
                            return {TokenType::UNKNOWN, token_location, Unknown::UNCLOSED_COMMENT};
                        return GetTokenHandler(); // 跳过注释后获取下一个记号
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::SLASH_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::SLASH};
                }
            case '%':
                switch (scanner.PeekChar(1))
                {
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PERCENT_EQUAL};
                    case '>': // }
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::RIGHT_BRACE};
                    case ':': // #
                        if (scanner.PeekChar(2) == '%' && scanner.PeekChar(3) == ':') // ##
                        {
                            scanner.SkipChar(3);
                            return {TokenType::UNKNOWN, token_location, Unknown::UNEXPECTED_HASH};
                        }
                        else if (start_of_row)
                        {
                            scanner.SkipChar(1); // 在行首，是合法预处理器
                            return PreprocessingHandler();
                        }
                        else
                        {
                            scanner.SkipChar(1);
                            return {TokenType::UNKNOWN, token_location, Unknown::UNEXPECTED_HASH};
                        }
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PERCENT};
                }
            case '<':
                switch (scanner.PeekChar(1))
                {
                    case '<':
                        if (scanner.PeekChar(2) == '=')
                        {
                            scanner.SkipChar(2);
                            return {TokenType::PUNCTUATOR, token_location, Punctuator::LESS_LESS_EQUAL};
                        }
                        else
                        {
                            scanner.SkipChar(1);
                            return {TokenType::PUNCTUATOR, token_location, Punctuator::LESS_LESS};
                        }
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::LESS_EQUAL};
                    case ':': // [
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::LEFT_SQUARE};
                    case '%': // {
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::LEFT_BRACE};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::LESS};
                }
            case '>':
                switch (scanner.PeekChar(1))
                {
                    case '>':
                        if (scanner.PeekChar(2) == '=')
                        {
                            scanner.SkipChar(2);
                            return {TokenType::PUNCTUATOR, token_location, Punctuator::GREATER_GREATER_EQUAL};
                        }
                        else
                        {
                            scanner.SkipChar(1);
                            return {TokenType::PUNCTUATOR, token_location, Punctuator::GREATER_GREATER};
                        }
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::GREATER_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::GREATER};
                }
            case '=':
                switch (scanner.PeekChar(1))
                {
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::EQUAL_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::EQUAL};
                }
            case '^':
                switch (scanner.PeekChar(1))
                {
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::CARET_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::CARET};
                }
            case '|':
                switch (scanner.PeekChar(1))
                {
                    case '|':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PIPE_PIPE};
                    case '=':
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PIPE_EQUAL};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::PIPE};
                }
            case '?':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::QUESTION};
            case ':':
                switch (scanner.PeekChar(1))
                {
                    case '>': // ]
                        scanner.SkipChar(1);
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::RIGHT_SQUARE};
                    default:
                        return {TokenType::PUNCTUATOR, token_location, Punctuator::COLON};
                }
            case ';':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::SEMICOLON};
            case ',':
                return {TokenType::PUNCTUATOR, token_location, Punctuator::COMMA};
            case '#':
                switch (scanner.PeekChar(1))
                {
                    case '#':
                        scanner.SkipChar(1);
                        return {TokenType::UNKNOWN, token_location, Unknown::UNEXPECTED_HASH};
                    default:
                        if (start_of_row) // 在行首，是合法预处理器
                            return PreprocessingHandler();
                        else
                            return {TokenType::UNKNOWN, token_location, Unknown::UNEXPECTED_HASH};
                }
            case '\'':
                return CharTokenHandler();
            case '\"':
                return StringTokenHandler();
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                buffer.push_back(c);
                return NumberTokenHandler();
            case 'L':
                switch (scanner.PeekChar(1))
                {
                    case '\'':
                        scanner.SkipChar(1);
                        return CharTokenHandler();
                    case '\"':
                        scanner.SkipChar(1);
                        return StringTokenHandler();
                    default:
                        buffer.push_back(c);
                        return IdentifierTokenHandler();
                }
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '_':
                buffer.push_back(c);
                return IdentifierTokenHandler();
            default:
                buffer.push_back(c);
                return {TokenType::UNKNOWN, token_location, Unknown::UNKNOWN_CHARACTER, buffer};
        }
    }
    
    void Lexer::LineCommentHandler()
    {
        while (!scanner.Eof() && scanner.GetChar() != '\n');
    }
    
    bool Lexer::BlockCommentHandler()
    {
        while (!scanner.Eof())
        {
            if (scanner.GetChar() == '*' && scanner.PeekChar(1) == '/')
            {
                scanner.SkipChar(1);
                return true;
            }
        }
        return false;
    }
    
    Token Lexer::PreprocessingHandler()
    {
        char c;
        while (!scanner.Eof())
        {
            c = scanner.GetChar();
            if (c == '\n')break;
            if (c == '\\')
            {
                do
                {
                    if (scanner.Eof())return {TokenType::UNKNOWN, token_location, Unknown::SLASH_NEWLINE_AT_EOF};
                    c = scanner.GetChar();
                } while (c != '\n' && isblank(c));
            }
        }
        return {TokenType::PREPROCESSING, token_location, ""};
    }
    
    Token Lexer::NumberTokenHandler()
    {
//        if (buffer[0] == '0' && (scanner.PeekChar(1) == 'x' || scanner.PeekChar(1) == 'X'))
//        {
//            while (!scanner.Eof() && (isNumberCharacter(scanner.PeekChar(1)) ||
//                                      (scanner.PeekChar(1) == '+' || scanner.PeekChar(1) == '-') &&
//                                      (buffer.back() == 'p' || buffer.back() == 'P')))
//                buffer.push_back(scanner.GetChar());
//        }
//        else
//        {
//            while (!scanner.Eof() && (isNumberCharacter(scanner.PeekChar(1)) ||
//                                      (scanner.PeekChar(1) == '+' || scanner.PeekChar(1) == '-') &&
//                                      (buffer.back() == 'e' || buffer.back() == 'E')))
//                buffer.push_back(scanner.GetChar());
//        }
        while (!scanner.Eof() && (isNumberCharacter(scanner.PeekChar(1)) ||
                                  (scanner.PeekChar(1) == '+' || scanner.PeekChar(1) == '-') &&
                                  (buffer.back() == 'e' || buffer.back() == 'E' || buffer.back() == 'p' ||
                                   buffer.back() == 'P')))
            buffer.push_back(scanner.GetChar());
        switch (buffer[0])
        {
            case '.':
                return DecimalFloatHandler(buffer.begin());
            case '0':
                if (buffer[1] == 'x' || buffer[1] == 'X')
                    return HexIntegerHandler(buffer.begin() + 2);
                else
                    return OctIntegerHandler(buffer.begin() + 1);
            default:
                return DecimalIntegerHandler(buffer.begin());
        }
    }
    
    bool Lexer::isNumberCharacter(char c)
    {
        return isalnum(c) || c == '.';
    }
    
    bool Lexer::isOctCharacter(char c)
    {
        return c >= '0' && c <= '7';
    }
    
    bool Lexer::isIntegerSuffix(const string &str)
    {
        return str.empty() || str == "U" || str == "UL" || str == "ULL" || str == "L" || str == "LU" || str == "LL" ||
               str == "LLU" || str == "u" || str == "ul" || str == "ull" || str == "l" || str == "lu" || str == "ll" ||
               str == "llu";
    }
    
    bool Lexer::isFloatSuffix(const string &str)
    {
        return str.empty() || str == "f" || str == "l" || str == "F" || str == "L";
    }
    
    Token Lexer::DecimalIntegerHandler(string::iterator iter)
    {
        while (iter != buffer.end() && isnumber(*iter))
            ++iter;
        if (iter == buffer.end())
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        if (*iter == '.' || *iter == 'e' || *iter == 'E')
            return DecimalFloatHandler(iter);
        if (isIntegerSuffix(string(iter, buffer.end())))
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
    }
    
    Token Lexer::OctIntegerHandler(std::string::iterator iter)
    {
        while (iter != buffer.end() && isOctCharacter(*iter))
            ++iter;
        if (iter == buffer.end())
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        if (*iter == '.' || *iter == 'e' || *iter == 'E')
            return DecimalFloatHandler(iter);
        if (*iter == '8' || *iter == '9') // 如果出现8和9，尝试作为十进制浮点数处理
        {
            while (isnumber(*iter))
                ++iter;
            if (*iter == '.' || *iter == 'e' || *iter == 'E')
                return DecimalFloatHandler(iter);
            return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
        }
        if (isIntegerSuffix(string(iter, buffer.end())))
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
    }
    
    Token Lexer::HexIntegerHandler(std::string::iterator iter)
    {
        if (*iter == '.') // 如果开头就是小数点，其后必须跟一个十六进制数字
        {
            if (isxdigit(*(iter + 1)))
                return HexFloatHandler(iter);
            return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
        }
        if (!isxdigit(*iter))
            return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
        while (iter != buffer.end() && isxdigit(*iter))
            ++iter;
        if (iter == buffer.end())
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        if (*iter == '.' || *iter == 'p' || *iter == 'P')
            return HexFloatHandler(iter);
        if (isIntegerSuffix(string(iter, buffer.end())))
            return {TokenType::INTEGER_CONSTANT, token_location, buffer};
        return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
    }
    
    Token Lexer::DecimalFloatHandler(std::string::iterator iter)
    {
        if (*iter == '.')
        {
            ++iter;
            while (isdigit(*iter))++iter;
        }
        if (*iter == 'e' || *iter == 'E')
        {
            ++iter;
            if (*iter == '+' || *iter == '-')++iter;
            if (!isdigit(*iter)) // 指数部分必须至少有一个数字
                return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
            while (isdigit(*iter))++iter;
        }
        if (isFloatSuffix(string(iter, buffer.end())))
            return {TokenType::FLOAT_CONSTANT, token_location, buffer};
        return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
    }
    
    Token Lexer::HexFloatHandler(std::string::iterator iter)
    {
        if (*iter == '.')
        {
            ++iter;
            while (isxdigit(*iter))++iter;
        }
        if (*iter == 'p' || *iter == 'P')
        {
            ++iter;
            if (*iter == '+' || *iter == '-')++iter;
            if (!isdigit(*iter)) // 指数部分必须至少有一个数字
                return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
            while (isdigit(*iter))++iter;
        }
        else // 十六进制浮点数必须有指数部分
            return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
        if (isFloatSuffix(string(iter, buffer.end())))
            return {TokenType::FLOAT_CONSTANT, token_location, buffer};
        return {TokenType::UNKNOWN, token_location, Unknown::ILLEGAL_NUMBER, buffer};
    }
    
    Token Lexer::CharTokenHandler()
    {
        char c;
        while (!scanner.Eof())
        {
            c = scanner.GetChar();
            if (c == '\'')
                return {TokenType::CHAR_CONSTANT, token_location, buffer};
            if (c == '\n')
                return {TokenType::UNKNOWN, token_location, Unknown::UNCLOSED_CHAR, buffer};
            if (c == '\\')
            {
                std::string tempString{c};
                do
                {
                    if (scanner.Eof())return {TokenType::UNKNOWN, token_location, Unknown::SLASH_NEWLINE_AT_EOF};
                    c = scanner.GetChar();
                    tempString.push_back(c);
                } while (c != '\n' && isblank(c));
                if (c == '\n')continue;
                buffer += tempString; // 如果反斜杠后跟的不是空格和换行，则反斜杠及其后的部分要加入缓冲区
            }
            else
                buffer.push_back(c);
        }
        return {TokenType::UNKNOWN, token_location, Unknown::UNCLOSED_CHAR, buffer};
    }
    
    Token Lexer::StringTokenHandler()
    {
        char c;
        while (!scanner.Eof())
        {
            c = scanner.GetChar();
            if (c == '\"')
                return {TokenType::STRING_LITERAL, token_location, buffer};
            if (c == '\n')
                return {TokenType::UNKNOWN, token_location, Unknown::UNCLOSED_STRING, buffer};
            if (c == '\\')
            {
                std::string tempString{c};
                do
                {
                    if (scanner.Eof())return {TokenType::UNKNOWN, token_location, Unknown::SLASH_NEWLINE_AT_EOF};
                    c = scanner.GetChar();
                    tempString.push_back(c);
                } while (c != '\n' && isblank(c));
                if (c == '\n')continue;
                buffer += tempString; // 如果反斜杠后跟的不是空格和换行，则反斜杠及其后的部分要加入缓冲区
            }
            else
                buffer.push_back(c);
        }
        return {TokenType::UNKNOWN, token_location, Unknown::UNCLOSED_STRING, buffer};
    }
    
    bool Lexer::isIdentifierCharacter(char c)
    {
        return c == '_' || isalnum(c);
    }
    
    Token Lexer::IdentifierTokenHandler()
    {
        while (!scanner.Eof() && isIdentifierCharacter(scanner.PeekChar(1)))
            buffer.push_back(scanner.GetChar());
        auto keyword = dict.find(buffer);
        if (keyword)
            return {TokenType::KEYWORD, token_location, keyword};
        return {TokenType::IDENTIFIER, token_location, buffer};
    }
    
    std::ostream &operator<<(ostream &os, const Lexer &lexer)
    {
        os << "[File Name] " << lexer.scanner.GetFileName() << endl;
        os << "[Rows] " << lexer.scanner.GetRow() << endl;
        os << "[Characters] " << lexer.scanner.GetCountChar() << endl;
        for (int i = 0; i < 10; ++i)
            os << "[" << token_type_literal[i] << "] " << lexer.lexeme_count[i] << endl;
        return os;
    }
}