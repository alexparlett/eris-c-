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

#include "Memory/RefCounted.h"

namespace Eris
{
    class INISection : public RefCounted
    {
    public:
        using iterator = std::map<std::string, std::string>::iterator;
        using const_iterator = std::map<std::string, std::string>::const_iterator;

        void setValue(const std::string& key, const std::string& value);
        void removeValue(const std::string& key);
        std::string getValue(const std::string& key) const;

        bool getBool(const std::string& key, bool default) const;
        glm::i32 getI32(const std::string& key, glm::i32 default) const;
        glm::u32 getU32(const std::string& key, glm::u32 default) const;
        glm::f32 getF32(const std::string& key, glm::f32 default) const;
        glm::i64 getI64(const std::string& key, glm::i64 default) const;
        glm::u64 getU64(const std::string& key, glm::u64 default) const;
        glm::f64 getF64(const std::string& key, glm::f64 default) const;
        std::string getString(const std::string& key, std::string default) const;

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;

        bool empty() const;
        bool exists(const std::string& key) const;

        static const INISection EMPTY;

    private:
        std::map<std::string, std::string> m_key_values;
    };
}
