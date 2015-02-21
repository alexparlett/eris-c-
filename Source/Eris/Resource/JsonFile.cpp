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

#include "JsonFile.h"

#include "Core/Log.h"
#include "Core/Profiler.h"
#include "Collections/Functions.h"
#include "Memory/ArrayPointers.h"

#include <boost/lexical_cast.hpp>

#include <rapidjson/allocators.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>

namespace Eris
{
    rapidjson::Value* JsonPath::operator()()
    {
        if (!root || root->IsNull() || path.empty())
            return nullptr;

        std::vector<std::string> tokens = std::string_split(path, "/");
        rapidjson::Value* current = root;
        for (auto token : tokens)
        {
            if (std::is_numeric<glm::i32>(token) && current->IsArray())
            {
                glm::u32 index = boost::lexical_cast<glm::i32>(token);
                if (index >= 0U && index < current->Size())
                    current = &current[index];
                else
                    return nullptr;
            }
            else
            {
                auto it = current->FindMember(token.c_str());
                if (it != current->MemberEnd())
                    current = &it->value;
                else
                    return nullptr;
            }
        }
        return current;
    }

    JsonFile::JsonFile(Context* context) :
        Resource(context),
        m_doc(new rapidjson::Document())
    {
    }

    JsonFile::~JsonFile()
    {
        if (m_doc)
        {
            delete m_doc;
            m_doc = nullptr;
        }
    }

    bool JsonFile::load(Deserializer& deserializer)
    {
        PROFILE(LoadJsonFile);

        std::size_t ds_size = deserializer.getSize();
        SharedArrayPtr<char> buffer(ds_size);
        std::size_t read = deserializer.read(buffer, ds_size);
        if (read != ds_size)
        {
            return false;
        }

        m_doc->Parse<rapidjson::kParseStopWhenDoneFlag>(buffer);

        if (m_doc->HasParseError())
        {
            Log::errorf("Failed loading JsonFile: %s (%u)", rapidjson::GetParseError_En(m_doc->GetParseError()), m_doc->GetErrorOffset());
            m_doc->SetNull();
            return false;
        }

        return true;
    }

    bool JsonFile::save(Serializer& serializer)
    {
        PROFILE(SaveJsonFile);

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        m_doc->Accept(writer);

        std::size_t out_size = serializer.write(buffer.GetString(), buffer.GetSize());
        return out_size == buffer.GetSize();
    }

    JsonElement JsonFile::createRoot(JsonElementType type)
    {
        m_doc->SetNull();
        switch (type)
        {
        case JsonElementType::ARRAY:
            m_doc->SetArray();
            break;
        case JsonElementType::OBJECT:
        default:
            m_doc->SetObject();
            break;
        }

        return JsonElement(this, m_doc);
    }

    void JsonFile::patch(const JsonElement& element)
    {
        rapidjson::Value* root = element.getElement();
        if (!root->IsArray())
        {
            Log::error("Failed Patching Json File: Root is not array.");
            return;
        }

        for (auto patch = root->Begin(); patch != root->End(); patch++)
        {
            auto op = patch->FindMember("op");
            if (op == patch->MemberEnd())
            {
                Log::error("Json Patch failed: Does not have a op object.");
                continue;
            }

            auto path = patch->FindMember("path");
            if (path == patch->MemberEnd())
            {
                Log::error("Json Patch failed: Does not have a path object.");
                continue;
            }

            if (strcmp(op->value.GetString(), "add") == 0)
                patchAdd(patch, path->value);
            else if (strcmp(op->value.GetString(), "replace") == 0)
                patchReplace(patch, path->value);
            else if (strcmp(op->value.GetString(), "remove") == 0)
                patchRemove(patch, path->value);
            else
                Log::error("Json Patch failed: should only use 'add', 'replace' or 'remove' elements.");
        }
    }

    Eris::JsonElement JsonFile::getRoot()
    {
        if (m_doc->IsNull())
            return JsonElement::EMPTY;

        return JsonElement(this, m_doc);
    }

    void JsonFile::patchAdd(rapidjson::Value* patch, const rapidjson::Value& path)
    {
        auto value = patch->FindMember("value");
        if (value == patch->MemberEnd())
        {
            Log::error("Json Patch failed: Does not have a value object.");
            return;
        }

        std::string path_string = path.GetString();
        std::size_t last_index = path_string.find_last_of('/');
        path_string = path_string.substr(0, last_index);
        std::string element_name = path_string.substr(last_index + 1);

        JsonPath json_path;
        json_path.root = m_doc;
        json_path.path = path_string;

        rapidjson::Value* origin = json_path();
        if (!origin)
        {
            Log::errorf("Json patch failed: invalid path (%s)", path_string);
            return;
        }

        if (std::is_numeric<glm::i32>(element_name) && origin->IsArray())
        {
            glm::i32 index = boost::lexical_cast<glm::i32>(element_name);
            origin->PushBack(value->value, m_doc->GetAllocator());
            for (auto i = origin->Size() - 1; i > index && i < origin->Size(); i--)
                origin[i - 1].Swap(origin[i]);
        }
        else
            origin->AddMember(rapidjson::StringRef(element_name), value->value, m_doc->GetAllocator());
    }

    void JsonFile::patchReplace(rapidjson::Value* patch, const rapidjson::Value& path)
    {
        auto value = patch->FindMember("value");
        if (value == patch->MemberEnd())
        {
            Log::error("Json Patch failed: Does not have a value object.");
            return;
        }

        JsonPath json_path;
        json_path.root = m_doc;
        json_path.path = path.GetString();

        rapidjson::Value* origin = json_path();
        if (!origin)
        {
            Log::errorf("Json patch failed: invalid path (%s)", json_path.path);
            return;
        }

        origin->CopyFrom(value->value, m_doc->GetAllocator());
    }

    void JsonFile::patchRemove(rapidjson::Value* patch, const rapidjson::Value& path)
    {
        std::string path_string = path.GetString();
        std::size_t last_index = path_string.find_last_of('/');
        path_string = path_string.substr(0, last_index);
        std::string element_name = path_string.substr(last_index + 1);

        JsonPath json_path;
        json_path.root = m_doc;
        json_path.path = path_string;

        rapidjson::Value* origin = json_path();
        if (!origin)
        {
            Log::errorf("Json patch failed: invalid path (%s)", path_string);
            return;
        }

        if (std::is_numeric<glm::i32>(element_name) && origin->IsArray())
        {
            glm::i32 index = boost::lexical_cast<glm::i32>(element_name);
            if (index < origin->Size() && index >= 0)
                origin->Erase(&origin[index]);
        }
        else
        {
            auto it = origin->FindMember(element_name.c_str());
            if (it != origin->MemberEnd())
                origin->EraseMember(it);
        }
    }
}