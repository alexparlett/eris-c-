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

#include "INISection.h"

namespace Eris
{
    void INISection::setKeyValue(const std::string& key, const std::string& value)
    {
        if (!key.empty() && !value.empty())
        {
            m_key_values[key] = value;
        }
    }

    std::string INISection::getKeyValue(const std::string& key) const
    {
        auto find = m_key_values.find(key);
        if (find != m_key_values.end())
            return find->second;

        return StringEmpty;
    }

    void INISection::removeKeyValue(const std::string& key)
    {
        m_key_values.erase(key);
    }

    INISection::iterator INISection::begin()
    {
        return m_key_values.begin();
    }

    INISection::iterator INISection::end()
    {
        return m_key_values.end();
    }

    INISection::const_iterator INISection::cbegin() const
    {
        return m_key_values.cbegin();
    }

    INISection::const_iterator INISection::cend() const
    {
        return m_key_values.cend();
    }

    bool INISection::empty() const
    {
        return m_key_values.empty();
    }

    bool INISection::exists(const std::string& key) const
    {
        return m_key_values.find(key) != m_key_values.end();
    }
}