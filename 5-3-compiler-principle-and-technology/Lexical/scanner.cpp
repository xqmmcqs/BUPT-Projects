/**
 * @file      scanner.cpp
 * @brief     扫描器
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#include "scanner.h"

namespace Lexical
{
    bool Scanner::Eof()
    {
        return source_file.eof() && buffer.empty();
    }
    
    char Scanner::GetChar()
    {
        if (Eof())
            return 0;
        if (buffer.empty())
            Read();
        char c = buffer.front();
        buffer.pop_front();
        
        if (c == '\n')
        {
            ++row;
            column = 0;
            start_of_row = true;
        }
        else
        {
            ++column;
            start_of_row &= isblank(c);
        }
        ++count_char;
        return c;
    }
    
    char Scanner::PeekChar(size_t offset)
    {
        while (!source_file.eof() && buffer.size() < offset)
            Read();
        return buffer.at(offset - 1);
    }
    
    void Scanner::SkipChar(int num)
    {
        while (num--)
            GetChar();
    }
    
    void Scanner::Read()
    {
        if (source_file.eof())return;
        char c = source_file.get();
        buffer.push_back(c);
        source_file.peek();
    }
}
