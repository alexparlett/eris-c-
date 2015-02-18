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

#include "JsonElement.h"
#include "Resource.h"

namespace Eris
{
    struct JsonPath
    {
        rapidjson::Value* operator()();

        std::string path;
        rapidjson::Value* root;
    };

    class JsonFile : public Resource
    {
    public:
        JsonFile(Context* context);
        virtual ~JsonFile();

        virtual bool load(Deserializer& deserializer) override;
        virtual bool save(Serializer& serializer) override;

        JsonElement createRoot(JsonElementType = JsonElementType::OBJECT);

        void patch(const JsonElement& element);
        void clear() { m_doc->Clear(); }

        bool empty() { return m_doc->IsArray() ? m_doc->Empty() : m_doc->ObjectEmpty(); }

        JsonElement getRoot();
        rapidjson::Document* getDocument() const { return m_doc; }

    private:
        void patchAdd(rapidjson::Value* patch, const rapidjson::Value& path);
        void patchReplace(rapidjson::Value* patch, const rapidjson::Value& path);
        void patchRemove(rapidjson::Value* patch, const rapidjson::Value& path);

        rapidjson::Document* m_doc;
    };
}
