//
// Copyright (c) 2008-2014 the Eris project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <functional>
#include <stdarg.h>
#include <memory>
#include <stdlib.h>

namespace Eris
{
    template<class Int>
    inline bool isPowerOfTwo(Int x)
    {
        while (((x % 2) == 0) && x > 1)
        {
            x /= 2;
        }
        return (x == 1);
    }

    template<class Type>
    inline Type getAlignedRoundUp(std::size_t alignment, Type value)
    {
        ERIS_ASSERT(isPowerOfTwo(alignment));
        std::size_t v = (std::size_t) value;
        v = (v + alignment - 1) & ~(alignment - 1);
        return (Type) v;
    }

    template<class Type>
    inline void alignRoundUp(std::size_t alignment, Type& value)
    {
        value = getAlignedRoundUp(alignment, value);
    }

    template<class Type>
    inline Type getAlignedRoundDown(std::size_t alignment, Type value)
    {
        ERIS_ASSERT(isPowerOfTwo(alignment));
        std::size_t v = (std::size_t) value;
        v &= ~(alignment - 1);
        return (Type) v;
    }

    template<class Type>
    inline void alignRoundDown(std::size_t alignment, Type& value)
    {
        value = getAlignedRoundDown(alignment, value);
    }

    template<class Type>
    inline bool isAligned(std::size_t alignment, Type value)
    {
        return ((std::size_t) value % alignment) == 0;
    }

    template<class T, class... Args>
    void construct(T* ptr, Args&&... args)
    {
        ::new (reinterpret_cast<void*>(ptr)) T(forward<Args>(args)...);
    }

    template<class T>
    void destruct(T* ptr)
    {
        ptr->~T();
    }
}

namespace std
{
    static string string_format(const string fmt_str, ...)
    {
        int final_n, n = ((int) fmt_str.size()) * 2;
        string str;
        unique_ptr<char[]> formatted;
        va_list ap;
        while (1)
        {
            formatted.reset(new char[n]);
            strcpy_s(&formatted[0], n, fmt_str.c_str());
            va_start(ap, fmt_str);
            final_n = vsnprintf_s(&formatted[0], n, _TRUNCATE, fmt_str.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }
        return string(formatted.get());
    }

    static string string_replace(string str, const string& from, const string& to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
}