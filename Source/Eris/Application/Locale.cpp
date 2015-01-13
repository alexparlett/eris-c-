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

#include "Locale.h"

#include "Resource/XMLFile.h"
#include "Resource/ResourceCache.h"

namespace Eris
{
    Page::Page() :
        RefCounted()
    {
    }

    Page::Page(const XMLElement& element) :
        RefCounted()
    {
        for (auto line : element)
        {
            glm::i32 id = line.getI32("id", -1);
            std::string value = line.getString(StringEmpty);

            if (id >= 0 && !value.empty())
                m_lines[id] = value;
        }
    }

    std::string Page::getLine(glm::i32 line) const
    {
        auto find = m_lines.find(line);
        return find != m_lines.end() ? find->second : StringEmpty;
    }


    Locale::Locale(Context* context) :
        Object(context)
    {
    }

    void Locale::load(const Path& file_name)
    {
        ERIS_ASSERT(!file_name.empty());

        m_pages.clear();

        Path full_path = "Locales/";
        full_path /= file_name;
        full_path.replace_extension(".xml");

        XMLFile* file = m_context->getModule<ResourceCache>()->getResource<XMLFile>(full_path);
        if (file)
        {
            for (auto page : file->getRoot())
            {
                glm::i32 id = page.getI32("id", -1);
                if (id >= 0)
                    m_pages[id] = new Page(page);
            }
        }
    }

    std::string Locale::localize(glm::i32 page, glm::i32 line) const
    {
        auto find = m_pages.find(page);
        return find != m_pages.end() ? find->second->getLine(line) : StringEmpty;
    }

    void Locale::replace(std::string& line, glm::i32 token, const std::string& value) const
    {
        std::string_replace(line, std::string_format("{%d}", token), value);
    }

    void Locale::replace(std::string& line, const std::vector<std::string>& values) const
    {
        for (glm::i32 i = 0; i < values.size(); i++)
            std::string_replace(line, std::string_format("{%d}", i), values[i]);
    }

}