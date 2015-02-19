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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/RefCounted.h"
#include "Resource/JsonElement.h"

namespace Eris
{
    class Page : public RefCounted
    {
    public:
        Page();
        Page(const JsonElement& element);

        std::string getLine(glm::i32 line) const;

    private:
        std::unordered_map<glm::i32, std::string> m_lines;
    };

    class Locale : public Object
    {
    public:
        Locale(Context* context);

        void load(const Path& fileName);

        std::string localize(glm::i32 page, glm::i32 line) const;
        void replace(std::string& line, glm::i32 token, const std::string& value) const;
        void replace(std::string& line, const std::vector<std::string>& values) const;

    private:
        std::unordered_map<glm::i32, SharedPtr<Page>> m_pages;
    };

    template<> inline void Context::registerModule(Locale* module)
    {
        m_locale = SharedPtr<Locale>(module);
    }

    template<> inline Locale* Context::getModule()
    {
        ERIS_ASSERT(m_locale);
        return m_locale.get();
    }
}