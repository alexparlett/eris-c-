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

#include "Memory/Pointers.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace Eris
{
    class JsonFile;
    class JsonElementIterator;

    enum class JsonElementType : glm::i8
    {
        OBJECT,
        ARRAY,
        VALUE
    };

    class JsonElement
    {
    public:
        using iterator = JsonElementIterator;
        using const_iterator = const JsonElementIterator;

        JsonElement();
        JsonElement(JsonFile* file, rapidjson::Value* value);
        JsonElement(const JsonElement& rhs);

        JsonElement& operator = (const JsonElement& rhs);

        operator bool() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        /// Add Child to Array
        JsonElement addChild(JsonElementType type);
        /// Add Child to Object
        JsonElement addChild(const std::string& name, JsonElementType type);
        bool removeChild(JsonElement& ele);
        bool removeChild(const std::string& name);
        bool removeChild(glm::i32 index);
        bool removeChildren();

        bool empty() const;
        bool hasChild(const std::string& name) const;
        bool hasChildren() const;

        JsonElement getChild(const std::string& name);
        JsonElement getChild(glm::i32 index);

        bool getBool(bool default) const;
        glm::i32 getI32(glm::i32 default) const;
        glm::u32 getU32(glm::u32 default) const;
        glm::f32 getF32(glm::f32 default) const;
        glm::i64 getI64(glm::i64 default) const;
        glm::u64 getU64(glm::u64 default) const;
        glm::f64 getF64(glm::f64 default) const;
        glm::fvec2 getFVec2(glm::fvec2 default) const;
        glm::fvec3 getFVec3(glm::fvec3 default) const;
        glm::fvec4 getFVec4(glm::fvec4 default) const;
        glm::ivec2 getIVec2(glm::ivec2 default) const;
        glm::ivec3 getIVec3(glm::ivec3 default) const;
        glm::ivec4 getIVec4(glm::ivec4 default) const;
        glm::bvec2 getBVec2(glm::bvec2 default) const;
        glm::bvec3 getBVec3(glm::bvec3 default) const;
        glm::bvec4 getBVec4(glm::bvec4 default) const;
        glm::mat2 getMat2(glm::mat2 default) const;
        glm::mat3 getMat3(glm::mat3 default) const;
        glm::mat4 getMat4(glm::mat4 default) const;
        std::string getString(std::string default) const;

        bool setValue(const std::string& value);

        JsonFile* getFile() const { return m_file; }
        rapidjson::Value* getElement() const { return m_value; }

        static const JsonElement EMPTY;
    private:
        rapidjson::Value* m_value;
        WeakPtr<JsonFile> m_file;
    };

    class JsonElementIterator
    {
    public:
        JsonElementIterator(JsonFile* file, rapidjson::Value::MemberIterator iter, rapidjson::Value::MemberIterator end, JsonElementType type);
        JsonElementIterator(JsonFile* file, rapidjson::Value::ValueIterator iter, rapidjson::Value::ValueIterator end, JsonElementType type);

        JsonElementIterator operator++ ();
        JsonElementIterator operator++ (glm::i32 junk);

        JsonElement& operator* ();
        JsonElement* operator-> ();

        bool operator == (const JsonElementIterator& rhs);
        bool operator != (const JsonElementIterator& rhs);
    private:
        WeakPtr<JsonFile> m_file;
        JsonElement m_element;
        JsonElementType m_type;
        rapidjson::Value::MemberIterator m_object_iter;
        rapidjson::Value::MemberIterator m_object_iter_end;
        rapidjson::Value::ValueIterator m_array_iter;
        rapidjson::Value::ValueIterator m_array_iter_end;
    };
}