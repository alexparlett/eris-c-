//
// Copyright (c) 2013-2015 the Eris project.
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

#include <cstdarg>
#include <cstdlib>
#include <cctype>
#include <locale>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace std
{
    inline std::vector<std::string> string_split(const string& str, const string& delims)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, str, boost::is_any_of(delims));
        return tokens;
    }

    inline string& string_ltrim(string& str)
    {
        boost::trim_left(str);
        return str;
    }

    inline string& string_rtrim(string& str)
    {
        boost::trim_right(str);
        return str;
    }

    inline string string_format_recursive(boost::format& format)
    {
        return format.str();
    }

    template<typename T, typename... Args>
    inline string string_format_recursive(boost::format& format, T&& arg, Args&&... args)
    {
        return string_format_recursive(format % std::forward<T>(arg), std::forward<Args>(args)...);
    }

    template<typename... Arguments>
    inline string string_format(const string& fmt_str, Arguments&&... args)
    {
        boost::format format(fmt_str);
        return string_format_recursive(format, std::forward<Arguments>(args)...);
    }

    inline string& string_replace(string& str, const string& from, const string& to)
    {
        boost::replace_all(str, from, to);
        return str;
    }

    inline string& string_upper(string& str)
    {
        boost::to_upper(str);
        return str;
    }

    inline string& string_lower(string& str)
    {
        boost::to_lower(str);
        return str;
    }
}