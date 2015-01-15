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

#include "Material.h"
#include "Texture2D.h"
#include "TextureCube.h"

#include "Core/Log.h"
#include "Resource/ResourceCache.h"
#include "Resource/XMLFile.h"

namespace Eris
{
        Material::Material(Context* context) :
            Resource(context),
            m_cull_mode(CullMode::BACK)
        {
        }

        bool Material::load(Deserializer& deserializer)
        {
            ResourceCache* rc = m_context->getModule<ResourceCache>();

            SharedPtr<XMLFile> file(new XMLFile(m_context));
            if (!file->load(deserializer))
                return false;

            XMLElement root = file->getRoot();
            XMLElement program = root.getChild("program");
            if (!program)
            {
                Log::errorf("Failed loading Material: missing <program> element.");
                return false;
            }

            m_program = rc->getResource<ShaderProgram>(program.getValue());
            if (!m_program)
                return false;

            XMLElement textures = root.getChild("textures");
            for (auto texture : textures)
            {
                TextureUnit texture_unit;
                texture_unit.unit = texture.getI32("unit", -1);
                texture_unit.uniform = texture.getAttribute("uniform");

                if (texture.getAttribute("type") == "2d")
                    texture_unit.texture = rc->getResource<Texture2D, Texture>(texture.getValue());
                else if (texture.getAttribute("type") == "cube")
                    texture_unit.texture = rc->getResource<TextureCube, Texture>(texture.getValue());

                if (!texture_unit.texture || texture_unit.uniform.empty() || texture_unit.unit < 0 || texture_unit.unit > 31)
                {
                    Log::errorf("Failed loading Material: <texture> element with missing or invalid unit, uniform, type or texture value");
                    return false;
                }

                m_textures[texture_unit.unit]  = texture_unit;
            }

            XMLElement cull = root.getChild("cull");
            if (cull)
            {
                std::string value = cull.getValue();
                if (value == "back")
                    m_cull_mode = CullMode::BACK;
                else if (value == "front")
                    m_cull_mode = CullMode::FRONT;
                else if (value == "frontback")
                    m_cull_mode = CullMode::FRONT_BACK;
            }

            return true;
        }

        bool Material::save(Serializer& serializer)
        {
            return true;
        }


        void Material::setCullMode(CullMode mode)
        {
            m_cull_mode = mode;
        }


        void Material::use() const
        {
            ERIS_ASSERT(m_program);

            glCullFace((glm::u32) m_cull_mode);

            m_program->use();
            
            for (auto texture_unit : m_textures)
            {
                if (texture_unit.texture)
                {
                    glActiveTexture(GL_TEXTURE0 + texture_unit.unit);
                    texture_unit.texture->use();
                    glUniform1i(glGetUniformLocation(m_program->getHandle(), texture_unit.uniform.c_str()), texture_unit.unit);
                }
            }
        }
}