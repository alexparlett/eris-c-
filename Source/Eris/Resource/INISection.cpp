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

#include "Collections/Functions.h"

namespace Eris
{
    const INISection INISection::EMPTY;

    void INISection::setValue(const std::string& key, const std::string& value)
    {
        if (!key.empty() && !value.empty())
        {
            m_key_values[key] = value;
        }
    }

    std::string INISection::getValue(const std::string& key) const
    {
        auto find = m_key_values.find(key);
        if (find != m_key_values.end())
            return find->second;

        return StringEmpty;
    }

    bool INISection::getBool(const std::string& key, bool default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::string_lower(value) == "true";
    }

    glm::i32 INISection::getI32(const std::string& key, glm::i32 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stoi(value);
    }

    glm::u32 INISection::getU32(const std::string& key, glm::u32 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stoul(value);
    }

    glm::f32 INISection::getF32(const std::string& key, glm::f32 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stof(value);
    }

    glm::i64 INISection::getI64(const std::string& key, glm::i64 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stoll(value);
    }

    glm::u64 INISection::getU64(const std::string& key, glm::u64 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stoull(value);
    }

    glm::f64 INISection::getF64(const std::string& key, glm::f64 default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return std::stod(value);
    }

    std::string INISection::getString(const std::string& key, std::string default) const
    {
        std::string value = getValue(key);
        if (value == StringEmpty)
            return default;

        return value;
    }

    void INISection::removeValue(const std::string& key)
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

    INISection::const_iterator INISection::begin() const
    {
        return m_key_values.cbegin();
    }

    INISection::const_iterator INISection::end() const
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