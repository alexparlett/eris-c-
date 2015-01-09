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

#include "StringHash.h"

namespace Eris 
{
    const StringHash StringHash::ZERO;

    StringHash::StringHash() :
        m_value(0)
    {
    }

    StringHash::StringHash(const std::string& value)
    {
        std::hash<std::string> hasher;
        m_value = hasher(value);
    }

    StringHash::StringHash(const char* value)
    {
        std::hash<const char*> hasher;
        m_value = hasher(value);
    }

    StringHash::StringHash(const StringHash& rhs)
    {
        m_value = rhs.m_value;
    }

    void StringHash::operator=(const std::string& value)
    {
        std::hash<std::string> hasher;
        m_value = hasher(value);
    }

    void StringHash::operator=(const char* value)
    {
        std::hash<const char*> hasher;
        m_value = hasher(value);
    }

    void StringHash::operator=(const StringHash& rhs)
    {
        m_value = rhs.m_value;
    }

    bool StringHash::operator==(const StringHash& rhs) const
    {
        return m_value == rhs.m_value;
    }

    bool StringHash::operator != (const StringHash& rhs) const
    {
        return m_value != rhs.m_value;
    }

    bool StringHash::operator<(const StringHash& rhs) const
    {
        return m_value < rhs.m_value;
    }

    bool StringHash::operator>(const StringHash& rhs) const
    {
        return m_value > rhs.m_value;
    }

    bool StringHash::operator<=(const StringHash& rhs) const
    {
        return m_value <= rhs.m_value;
    }

    bool StringHash::operator>=(const StringHash& rhs) const
    {
        return m_value >= rhs.m_value;
    }

    std::size_t StringHash::operator()() const
    {
        return m_value;
    }
}