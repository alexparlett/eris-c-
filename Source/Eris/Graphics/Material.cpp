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
#include "Core/Profiler.h"
#include "Resource/ResourceCache.h"
#include "Resource/JsonFile.h"

#include <boost/functional/hash.hpp>

namespace Eris
{
    boost::uuids::random_generator Material::s_uuid_generator;

    Material::Material(Context* context) :
        Resource(context),
        m_cull_mode(CullMode::BACK),
        m_render_key(boost::hash<boost::uuids::uuid>()(s_uuid_generator()))
    {
    }

    bool Material::load(Deserializer& deserializer)
    {
        PROFILE(LoadMaterial);

        ResourceCache* rc = m_context->getModule<ResourceCache>();

        SharedPtr<JsonFile> file(new JsonFile(m_context));
        if (!file->load(deserializer))
            return false;

        JsonElement root = file->getRoot();
        JsonElement program = root.getChild("program");
        if (!program)
        {
            Log::errorf("Failed loading Material: missing <program> element.");
            return false;
        }

        m_program = rc->getResource<ShaderProgram>(program.getString(StringEmpty));
        if (!m_program)
            return false;

        JsonElement textures = root.getChild("textures");
        for (auto texture : textures)
        {
            TextureUnit texture_unit;
            texture_unit.unit = texture.getChild("unit").getI32(-1);
            texture_unit.uniform = texture.getChild("uniform").getString("diffuse");

            if (texture.getChild("type").getString("2d") == "2d")
                texture_unit.texture = rc->getResource<Texture2D, Texture>(texture.getChild("file").getString(StringEmpty));
            else if (texture.getChild("type").getString("2d") == "cube")
                texture_unit.texture = rc->getResource<TextureCube, Texture>(texture.getChild("file").getString(StringEmpty));

            if (!texture_unit.texture || texture_unit.uniform.empty() || texture_unit.unit < 0 || texture_unit.unit > 31)
            {
                Log::errorf("Failed loading Material: <texture> element with missing or invalid unit, uniform, type or texture value");
                return false;
            }

            m_textures[texture_unit.unit]  = texture_unit;
        }

        JsonElement uniforms = root.getChild("uniforms");
        for (auto uniform : uniforms)
        {
            std::string name = uniform.getChild("name").getString(StringEmpty);
            std::string type = uniform.getChild("type").getString(StringEmpty);

            Variant value;
            if (type == "f32")
                value = uniform.getChild("value").getF32(0.f);
            else if (type == "fvec2")
                value = uniform.getChild("value").getFVec2(glm::fvec2());
            else if (type == "fvec3")
                value = uniform.getChild("value").getFVec3(glm::fvec3());
            else if (type == "fvec4")
                value = uniform.getChild("value").getFVec4(glm::fvec4());
            else if (type == "i32")
                value = uniform.getChild("value").getI32(0);
            else if (type == "ivec2")
                value = uniform.getChild("value").getIVec2(glm::ivec2());
            else if (type == "ivec3")
                value = uniform.getChild("value").getIVec3(glm::ivec3());
            else if (type == "ivec4")
                value = uniform.getChild("value").getIVec4(glm::ivec4());
            else if (type == "bool")
                value = uniform.getChild("value").getBool(false);
            else if (type == "bvec2")
                value = uniform.getChild("value").getBVec2(glm::bvec2());
            else if (type == "bvec3")
                value = uniform.getChild("value").getBVec3(glm::bvec3());
            else if (type == "bvec4")
                value = uniform.getChild("value").getBVec4(glm::bvec4());
            else if (type == "mat2")
                value = uniform.getChild("value").getMat2(glm::mat2());
            else if (type == "mat3")
                value = uniform.getChild("value").getMat3(glm::mat3());
            else if (type == "mat4")
                value = uniform.getChild("value").getMat4(glm::mat4());
            else
            {
                Log::errorf("Failed loading Material: %s is not a valid uniform type", type);
                return false;
            }

            setUniform(name, value);
        }

        JsonElement cull = root.getChild("cull");
        if (cull)
        {
            std::string value = cull.getString("back");
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
        PROFILE(UseMaterial);
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