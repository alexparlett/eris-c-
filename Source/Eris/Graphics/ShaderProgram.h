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
#include "IO/File.h"
#include "Resource/Resource.h"
#include "Memory/ArrayPointers.h"

namespace Eris
{
    struct ShaderUniform
    {
        Variant data;
        glm::u32 type;
        glm::i32 location;
    };

    class ShaderPreprocessor
    {
    public:
        std::stringstream process( const File* in );
        void reset();

    private:
        bool isIncluded( const std::string& file );

        std::vector<std::string> m_included_files;
    };

    class ShaderProgram : public Resource
    {
    public:
        ShaderProgram(Context* context);

        virtual bool load(Deserializer& deserializer) override;
        virtual bool save(Serializer& serializer) override;

        void use() const;

        void setUniform(const std::string& uniform, const Variant& data);
        ShaderUniform* getUniform(std::string uniform);
        void removeUniform(std::string uniform);

        glm::u32 getHandle() const { return m_handle; }

    private:
        bool compile(const char* vert_source, const char* frag_source);

        glm::u32 m_handle;
        std::map<std::string, ShaderUniform> m_parameters;
    };
}