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

#include "ShaderProgram.h"
#include "Texture.h"

#include "Core/Context.h"
#include "Memory/Pointers.h"
#include "Resource/Resource.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace Eris
{
    struct TextureUnit
    {
        glm::i32 unit;
        std::string uniform;
        SharedPtr<Texture> texture;
    };

    enum class CullMode : glm::u32
    {
        BACK = GL_BACK,
        FRONT = GL_FRONT,
        FRONT_BACK = GL_FRONT_AND_BACK
    };

    class Material : public Resource
    {
    public:
        Material(Context* context);

        virtual bool load(Deserializer& deserializer) override;
        virtual bool save(Serializer& serializer) override;

        void use() const;

        ShaderProgram* getProgram() const { return m_program; }
        Texture* getTexture(glm::i32 index) const { ERIS_ASSERT(index >= 0 && index < 32); return m_textures[index].texture; }
        CullMode getCullMode() const { return m_cull_mode; }
        glm::u64 getRenderKey() const { return m_render_key; }

        void setCullMode(CullMode mode);

        void setUniform(const std::string& uniform, const Variant& data);
        ShaderUniform* getUniform(std::string uniform);
        void removeUniform(std::string uniform);

    private:
        glm::u64 m_render_key;
        CullMode m_cull_mode;
        SharedPtr<ShaderProgram> m_program;
        TextureUnit m_textures[32];
        std::map<std::string, ShaderUniform> m_parameters;
        std::vector<StringHash> m_tags;

        static boost::uuids::random_generator s_uuid_generator;
    };
}
