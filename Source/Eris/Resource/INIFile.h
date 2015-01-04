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

#include "Resource.h"

#include "Memory/RefCounted.h"

namespace Eris
{
    class INISection : public RefCounted
    {
    public:
        using iterator = std::unordered_map<std::string, std::string>::iterator;
        using const_iterator = std::unordered_map<std::string, std::string>::const_iterator;

        void setKeyValue(const std::string& key, const std::string& value);
        std::string getKeyValue(const std::string& key) const;
        void removeKeyValue(const std::string& key);

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;

        bool empty() const;
        bool exists(const std::string& key) const;

    private:
        std::unordered_map<std::string, std::string> m_key_values;
    };

    class INIFile : public Resource
    {
    public:
        using iterator = std::unordered_map<std::string, SharedPtr<INISection>>::iterator;
        using const_iterator = std::unordered_map<std::string, SharedPtr<INISection>>::const_iterator;

        INIFile(Context* context);
        virtual ~INIFile();

        virtual bool load(Deserializer& deserializer);
        virtual bool save(Serializer& serializer);

        void addSection(const std::string& section);
        INISection* getSection(const std::string& section) const;
        void removeSection(const std::string& section);

        void setKeyValue(const std::string& section, const std::string& key, const std::string& value);
        std::string getKeyValue(const std::string& section, const std::string& key) const;
        void removeKeyValue(const std::string& section, const std::string& key);

        void clear();

        bool empty() const;
        bool exists(const std::string& section) const;

        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;

    private:
        std::unordered_map<std::string, SharedPtr<INISection>> m_sections;
    };
}