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

#include "Graphics.h"
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

            XMLElement uniforms = root.getChild("uniforms");
            for (auto uniform : uniforms)
            {
                std::string name = uniform.getAttribute("name");
                std::string type = uniform.getAttribute("type");

                Variant value;
                if (type == "f32")
                    value = uniform.getF32(0.f);
                else if (type == "fvec2")
                    value = uniform.getFVec2(glm::fvec2());
                else if (type == "fvec3")
                    value = uniform.getFVec3(glm::fvec3());
                else if (type == "fvec4")
                    value = uniform.getFVec4(glm::fvec4());
                else if (type == "i32")
                    value = uniform.getI32(0);
                else if (type == "ivec2")
                    value = uniform.getIVec2(glm::ivec2());
                else if (type == "ivec3")
                    value = uniform.getIVec3(glm::ivec3());
                else if (type == "ivec4")
                    value = uniform.getIVec4(glm::ivec4());
                else if (type == "bool")
                    value = uniform.getBool(false);
                else if (type == "bvec2")
                    value = uniform.getBVec2(glm::bvec2());
                else if (type == "bvec3")
                    value = uniform.getBVec3(glm::bvec3());
                else if (type == "bvec4")
                    value = uniform.getBVec4(glm::bvec4());
                else if (type == "mat2")
                    value = uniform.getMat2(glm::mat2());
                else if (type == "mat3")
                    value = uniform.getMat3(glm::mat3());
                else if (type == "mat4")
                    value = uniform.getMat4(glm::mat4());
                else
                {
                    Log::errorf("Failed loading Material: %s is not a valid uniform type", type);
                    return false;
                }

                setUniform(name, value);
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

        void Material::setUniform(const std::string& uniform, const Variant& data)
        {
            if (m_parameters.find(uniform) != m_parameters.end())
                m_parameters[uniform].data = data;
            else
            {
                ShaderUniform* shader_uniform = m_program->getUniform(uniform);
                if (shader_uniform)
                {
                    ShaderUniform material_uniform;
                    material_uniform.location = shader_uniform->location;
                    material_uniform.type = shader_uniform->type;
                    material_uniform.data = data;
                    m_parameters[uniform] = material_uniform;
                }
                else
                    Log::warnf("Attempting to set undefined uniform %s in material %s", uniform, getName());
            }
        }

        ShaderUniform* Material::getUniform(std::string uniform)
        {
            auto find = m_parameters.find(uniform);
            if (find != m_parameters.end())
                return &find->second;
            return nullptr;
        }

        void Material::removeUniform(std::string uniform)
        {
            m_parameters.erase(uniform);
        }

        void Material::use() const
        {
            ERIS_ASSERT(m_program);

            Graphics* graphics = m_context->getModule<Graphics>();

            glCullFace((glm::u32) m_cull_mode);

            m_program->use();

            for (auto parameter : m_parameters)
            {
                if (parameter.second.data.which() != 0)
                    graphics->bindUniform(parameter.second.location, parameter.second.type, parameter.second.data);
            }
            
            for (auto texture_unit : m_textures)
            {
                if (texture_unit.texture)
                {
                    glActiveTexture(GL_TEXTURE0 + texture_unit.unit);
                    texture_unit.texture->use();
                    glUniform1i(glGetUniformLocation(m_program->getHandle(), texture_unit.uniform.c_str()), texture_unit.unit);
                }
            }

            glActiveTexture(GL_TEXTURE0);
        }
}