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

#include "INIFile.h"

#include "Core/Log.h"
#include "Memory/ArrayPointers.h"

#include <cstring>
#include <sstream>

namespace Eris
{
    INIFile::INIFile(Context* context) :
        Resource(context)
    {
    }

    INIFile::~INIFile()
    {
        clear();
    }

    bool INIFile::load(Deserializer& deserializer)
    {
        std::size_t ds_size = deserializer.getSize();

        SharedArrayPtr<char> buffer = SharedArrayPtr<char>(ds_size + 1);
        std::size_t in_size = deserializer.read(buffer.get(), ds_size);

        if (in_size != ds_size)
            return false;
        
        char* line = nullptr;
        char* next_line = nullptr;
        char* section = nullptr;
        char* next_section = nullptr;
            
        line = strtok_s(buffer.get(), "\r\n\0", &next_line);
        while (line)
        {
            if (line[0] == '[')
            {
                section = strtok_s(&line[1], "[]\r\n\0", &next_section);
                addSection(section);
            }
            else
            {
                char* key = nullptr;
                char* value = nullptr;

                key = strtok_s(line, "=\r\n\0", &value);
                setKeyValue(section, key, value);
            }

            line = strtok_s(nullptr, "\r\n\0", &next_line);
        }

        return true;
    }

    bool INIFile::save(Serializer& serializer)
    {
        for (auto section : m_sections)
        {
            serializer << "[" << section.first << "]" << SerializerTraits::ENDL;
            for (auto kv : *section.second)
            {
                serializer << kv.first << "=" << kv.second << SerializerTraits::ENDL;
            }
            serializer << SerializerTraits::ENDL;
        }

        return true;
    }

    void INIFile::addSection(const std::string& section)
    {
        auto find = m_sections.find(section);
        if (find == m_sections.end())
            m_sections[section] = new INISection();
    }

    INISection* INIFile::getSection(const std::string& section) const
    {
        auto find = m_sections.find(section);
        if (find != m_sections.end())
            return find->second;

        return nullptr;
    }

    void INIFile::removeSection(const std::string& section)
    {
        m_sections.erase(section);
    }

    void INIFile::setKeyValue(const std::string& section, const std::string& key, const std::string& value)
    {
        m_sections[section]->setKeyValue(key, value);
    }

    std::string INIFile::getKeyValue(const std::string& section, const std::string& key) const
    {
        auto find = m_sections.find(section);
        if (find != m_sections.end())
            return find->second->getKeyValue(key);

        return StringEmpty;
    }

    void INIFile::removeKeyValue(const std::string& section, const std::string& key)
    {
        auto find = m_sections.find(section);
        if (find != m_sections.end())
            find->second->removeKeyValue(key);
    }

    void INIFile::clear()
    {
        m_sections.clear();
    }

    bool INIFile::empty() const
    {
        return m_sections.empty();
    }

    bool INIFile::exists(const std::string& section) const
    {
        return m_sections.find(section) != m_sections.end();
    }

    INIFile::iterator INIFile::begin()
    {
        return m_sections.begin();
    }

    INIFile::iterator INIFile::end()
    {
        return m_sections.end();
    }

    INIFile::const_iterator INIFile::cbegin() const
    {
        return m_sections.cbegin();
    }

    INIFile::const_iterator INIFile::cend() const
    {
        return m_sections.cend();
    }

}