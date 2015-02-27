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

#include "JsonElement.h"
#include "JsonFile.h"

#include "Collections/Functions.h"

namespace Eris
{
    const JsonElement JsonElement::EMPTY;

    JsonElement::JsonElement() :
        m_file(nullptr),
        m_value(nullptr)
    {
    }

    JsonElement::JsonElement(JsonFile* file, rapidjson::Value* value) :
        m_file(file),
        m_value(value)
    {
    }

    JsonElement::JsonElement(const JsonElement& rhs)
    {
        *this = rhs;
    }

    JsonElement& JsonElement::operator=(const JsonElement& rhs)
    {
        m_file = rhs.m_file;
        m_value = rhs.m_value;
        return *this;
    }

    JsonElement::operator bool() const 
    { 
        return m_file && m_value ? !m_value->IsNull() : false; 
    }


    JsonElement JsonElement::operator[] (glm::i32 index) const
    {
        if (!m_file || !m_value || !m_value->IsArray() || index < 0 || index > m_value->Size())
            return EMPTY;

        return JsonElement(m_file, &m_value[index]);
    }

    JsonElement JsonElement::operator[] (const std::string& name) const
    {
        return (*this)[name.c_str()];
    }

    JsonElement JsonElement::operator[] (const char* name) const
    {
        if (!m_file || !m_value || !m_value->IsObject() || !name)
            return EMPTY;

        auto it = m_value->FindMember(name);
        if (it != m_value->MemberEnd())
            return JsonElement(m_file, &it->value);

        return EMPTY;
    }

    JsonElement::iterator JsonElement::begin()
    {
        if (m_value->IsArray())
        {
            return JsonElementIterator(m_file, m_value->Begin(), m_value->End(), JsonElementType::ARRAY);
        }
        else
        {
            return JsonElementIterator(m_file, m_value->MemberBegin(), m_value->MemberEnd(), JsonElementType::OBJECT);
        }
    }

    JsonElement::const_iterator JsonElement::begin() const
    {
        if (m_value->IsArray())
        {
            return JsonElementIterator(m_file, m_value->Begin(), m_value->End(), JsonElementType::ARRAY);
        }
        else
        {
            return JsonElementIterator(m_file, m_value->MemberBegin(), m_value->MemberEnd(), JsonElementType::OBJECT);
        }
    }

    JsonElement::iterator JsonElement::end()
    {
        if (m_value->IsArray())
        {
            return JsonElementIterator(m_file, m_value->End(), m_value->End(), JsonElementType::ARRAY);
        }
        else
        {
            return JsonElementIterator(m_file, m_value->MemberEnd(), m_value->MemberEnd(), JsonElementType::OBJECT);
        }
    }

    JsonElement::const_iterator JsonElement::end() const
    {
        if (m_value->IsArray())
        {
            return JsonElementIterator(m_file, m_value->End(), m_value->End(), JsonElementType::ARRAY);
        }
        else
        {
            return JsonElementIterator(m_file, m_value->MemberEnd(), m_value->MemberEnd(), JsonElementType::OBJECT);
        }
    }

    JsonElement JsonElement::addChild(JsonElementType type)
    {
        if (!m_file || !m_value || !m_value->IsArray())
            return EMPTY;

        rapidjson::Value& new_value = m_value->PushBack(rapidjson::Value(), m_file->getDocument()->GetAllocator());
        switch (type)
        {
        case JsonElementType::ARRAY:
            new_value.SetArray();
            break;
        case JsonElementType::OBJECT:
            new_value.SetObject();
            break;
        }
        return JsonElement(m_file, &new_value);
    }

    JsonElement JsonElement::addChild(const std::string& name, JsonElementType type)
    {
        if (!m_file || !m_value || !m_value->IsObject() || name.empty())
            return EMPTY;

        rapidjson::Value& new_value = m_value->AddMember(rapidjson::StringRef(name), rapidjson::Value(), m_file->getDocument()->GetAllocator());
        switch (type)
        {
        case JsonElementType::ARRAY:
            new_value.SetArray();
            break;
        case JsonElementType::OBJECT:
            new_value.SetObject();
            break;
        }
        return JsonElement(m_file, &new_value);
    }

    bool JsonElement::removeChild(JsonElement& ele)
    {
        if (!m_file || !m_value)
            return false;

        if (m_value->IsArray())
            return m_value->Erase(ele.m_value);
        else if (m_value->IsObject())
        {
            auto it = m_value->FindMember(*ele.m_value);
            if (it != m_value->MemberEnd())
            {
                m_value->EraseMember(it);
                return true;
            }
        }

        return false;
    }

    bool JsonElement::removeChild(const std::string& name)
    {
        if (!m_file || !m_value || !m_value->IsObject())
            return false;

        auto it = m_value->FindMember(name.c_str());
        if (it != m_value->MemberEnd())
        {
            m_value->EraseMember(it);
            return true;
        }

        return true;
    }

    bool JsonElement::removeChild(glm::i32 index)
    {
        if (!m_file || !m_value || !m_value->IsArray())
            return false;

        if (index >= 0 && index < m_value->Size())
            return m_value->Erase(&m_value[index]);

        return false;
    }

    bool JsonElement::removeChildren()
    {
        if (!m_file || !m_value)
            return false;

        if (m_value->IsArray())
        {
            m_value->Clear();
            return true;
        }
        else if (m_value->IsObject())
        {
            m_value->RemoveAllMembers();
            return true;
        }

        return false;
    }

    bool JsonElement::null() const
    {
        return this;
    }

    std::size_t JsonElement::childCount() const
    {
        if (!m_file || !m_value)
            return 0;

        if (m_value->IsArray())
            return m_value->Size();
        else if (m_value->IsObject())
            return m_value->MemberCount();

        return 0;
    }

    bool JsonElement::hasChild(const std::string& name) const
    {
        if (!m_file || !m_value || !m_value->IsObject())
            return false;

        return m_value->FindMember(name.c_str()) != m_value->MemberEnd();
    }

    bool JsonElement::hasChildren() const
    {
        if (!m_file || !m_value)
            return false;

        if (m_value->IsArray())
            return m_value->Empty();
        else if (m_value->IsObject())
            return m_value->ObjectEmpty();

        return false;
    }

    JsonElement JsonElement::getChild(const std::string& name) const
    {
        if (name.empty() || !m_value->IsObject())
            return EMPTY;

        auto it = m_value->FindMember(name.c_str());
        if (it != m_value->MemberEnd())
            return JsonElement(m_file, &it->value);

        return EMPTY;
    }

    JsonElement JsonElement::getChild(glm::i32 index) const
    {
        if (index < 0 || index > m_value->Size() || !m_value->IsArray())
            return EMPTY;

        return JsonElement(m_file, &m_value[index]);
    }

    bool JsonElement::getBool(bool default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::string_lower(value) == "true";
    }

    glm::i32 JsonElement::getI32(glm::i32 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stoi(value);
    }

    glm::u32 JsonElement::getU32(glm::u32 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stoul(value);
    }

    glm::f32 JsonElement::getF32(glm::f32 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stof(value);
    }

    glm::i64 JsonElement::getI64(glm::i64 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stoll(value);
    }

    glm::u64 JsonElement::getU64(glm::u64 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stoull(value);
    }

    glm::f64 JsonElement::getF64(glm::f64 default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return std::stod(value);
    }

    glm::fvec2 JsonElement::getFVec2(glm::fvec2 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::fvec2(stof(parts[0]), stof(parts[1]));
    }

    glm::fvec3 JsonElement::getFVec3(glm::fvec3 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::fvec3(stof(parts[0]), stof(parts[1]), stof(parts[2]));
    }

    glm::fvec4 JsonElement::getFVec4(glm::fvec4 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::fvec4(stof(parts[0]), stof(parts[1]), stof(parts[2]), stof(parts[3]));
    }

    glm::ivec2 JsonElement::getIVec2(glm::ivec2 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::ivec2(stoi(parts[0]), stoi(parts[1]));
    }

    glm::ivec3 JsonElement::getIVec3(glm::ivec3 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::ivec3(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]));
    }

    glm::ivec4 JsonElement::getIVec4(glm::ivec4 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::ivec4(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]), stoi(parts[3]));
    }

    glm::bvec2 JsonElement::getBVec2(glm::bvec2 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::bvec2(stoi(parts[0]), stoi(parts[1]));
    }

    glm::bvec3 JsonElement::getBVec3(glm::bvec3 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::bvec3(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]));
    }

    glm::bvec4 JsonElement::getBVec4(glm::bvec4 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::bvec4(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]), stoi(parts[3]));
    }

    glm::mat2 JsonElement::getMat2(glm::mat2 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::mat2(stof(parts[0]), stof(parts[1]),
                         stof(parts[2]), stof(parts[3]));
    }

    glm::mat3 JsonElement::getMat3(glm::mat3 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::mat3(stof(parts[0]), stof(parts[1]), stof(parts[2]),
                         stof(parts[3]), stof(parts[4]), stof(parts[5]),
                         stof(parts[6]), stof(parts[7]), stof(parts[8]));
    }

    glm::mat4 JsonElement::getMat4(glm::mat4 default) const
    {
        std::string value = m_value->GetString();
        if (value.empty())
            return default;

        std::vector<std::string> parts = std::string_split(value, " ");
        return glm::mat4(stof(parts[0]), stof(parts[1]), stof(parts[2]), stof(parts[3]),
                         stof(parts[4]), stof(parts[5]), stof(parts[6]), stof(parts[7]),
                         stof(parts[8]), stof(parts[9]), stof(parts[10]), stof(parts[11]),
                         stof(parts[12]), stof(parts[13]), stof(parts[14]), stof(parts[15]));
    }

    std::string JsonElement::getString(std::string default) const
    {
        std::string value = m_value->GetString();
        if (value == StringEmpty)
            return default;

        return value;
    }

    StringHash JsonElement::getHash(StringHash default) const
    {
        StringHash value(m_value->GetString());
        if (value == StringEmpty)
            return default;

        return value;
    }

    bool JsonElement::setValue(const std::string& value)
    {
        if (!m_file || !m_value)
            return false;

        m_value->SetString(value.c_str(), m_file->getDocument()->GetAllocator());
        return true;
    }


    JsonElementIterator::JsonElementIterator(JsonFile* file, rapidjson::Value::MemberIterator iter, rapidjson::Value::MemberIterator end, JsonElementType type) :
        m_file(file),
        m_type(type),
        m_object_iter(iter),
        m_object_iter_end(end),
        m_element(file, &iter->value)
    {

    }

    JsonElementIterator::JsonElementIterator(JsonFile* file, rapidjson::Value::ValueIterator iter, rapidjson::Value::ValueIterator end, JsonElementType type) :
        m_file(file),
        m_type(type),
        m_array_iter(iter),
        m_array_iter_end(end),
        m_element(m_file, iter)
    {

    }

    JsonElementIterator JsonElementIterator::operator++()
    {
        JsonElementIterator i = *this;


        switch (m_type)
        {
        case JsonElementType::ARRAY:
            if (m_array_iter != m_array_iter_end)
                m_element = JsonElement(m_file, m_array_iter);
            m_array_iter++;
            break;
        case JsonElementType::OBJECT:
            if (m_object_iter != m_object_iter_end)
                m_element = JsonElement(m_file, &m_object_iter->value);
            m_object_iter++;
            break;
        }

        return i;
    }

    JsonElementIterator JsonElementIterator::operator++(glm::i32 junk)
    {
        switch (m_type)
        {
        case JsonElementType::ARRAY:
            m_array_iter++;
            if (m_array_iter != m_array_iter_end)
                m_element = JsonElement(m_file, m_array_iter);
            break;
        case JsonElementType::OBJECT:
            m_object_iter++;
            if (m_object_iter != m_object_iter_end)
                m_element = JsonElement(m_file, &m_object_iter->value);
            break;
        }
        
        return *this;
    }

    JsonElement& JsonElementIterator::operator*()
    {
        return m_element;
    }

    JsonElement* JsonElementIterator::operator->()
    {
        return &m_element;
    }

    bool JsonElementIterator::operator==(const JsonElementIterator& rhs)
    {
        if (m_type != rhs.m_type)
            return false;

        switch (m_type)
        {
        case JsonElementType::ARRAY:
            return m_array_iter == rhs.m_array_iter;
            break;
        case JsonElementType::OBJECT:
            return m_object_iter == rhs.m_object_iter;
            break;
        }

        return false;
    }

    bool JsonElementIterator::operator!=(const JsonElementIterator& rhs)
    {
        return !(*this == rhs);
    }

}