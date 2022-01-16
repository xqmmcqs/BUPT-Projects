/**
 * @file      scanner.h
 * @brief     扫描器
 * @author    Ziheng Mao
 * @date      2021/9/25
 * @copyright GNU General Public License, version 3 (GPL-3.0)
*/

#ifndef LEXICAL_SCANNER_H
#define LEXICAL_SCANNER_H

#include <fstream>
#include "token.h"

namespace Lexical
{
    /// 扫描器
    class Scanner
    {
    public:
        explicit Scanner(std::string file_name_) :
                file_name(std::move(file_name_)), row(1), column(0), count_char(0), buffer(), start_of_row(true)
        {
            source_file.open(file_name);
            assert(!source_file.fail());
        }
        
        /**
         * @brief 判断是否扫描到文件末尾
         *
         * @return 如果文件流已经到文件末尾并且缓冲区为空，返回true
         */
        bool Eof();
        
        /**
         * @brief 读取文件流中的下一个字符
         *
         * @return 文件流中的下一个字符
         */
        char GetChar();
        
        /**
         * @brief 从当前位置开始向后读取数个字符
         *
         * @param offset 偏移量
         * @return 从当前字符起（包括当前字符）的第offset个字符
         */
        char PeekChar(size_t offset);
        
        /**
         * @brief 从文件流中跳过数个字符
         *
         * @param num 跳过的字符的个数
         */
        void SkipChar(int num);
        
        const std::string &GetFileName() const
        { return file_name; }
        
        unsigned int GetRow() const
        { return row; }
        
        unsigned int GetCountChar() const
        { return count_char; }
        
        bool GetStartOfRow() const
        { return start_of_row; }
        
        TokenLocation GetLocation() const
        { return {file_name, row, column}; }
    
    private:
        std::string file_name; ///< 文件名
        std::ifstream source_file; ///< 文件输入流
        unsigned int row; ///< 当前行号
        unsigned int column; ///< 当前列号
        unsigned int count_char; ///< 已经读取的字符数
        std::deque<char> buffer; ///< 缓冲区
        bool start_of_row; ///< 判断行首到当前位置是否都是空白字符
        
        /**
         * @brief 从文件流向缓冲区中读取一个字符
         */
        void Read();
    };
}

#endif //LEXICAL_SCANNER_H
