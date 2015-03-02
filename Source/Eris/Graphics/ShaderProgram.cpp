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
#include "Renderer.h"
#include "ShaderProgram.h"

#include "Core/Log.h"
#include "Core/Profiler.h"

#include <sstream>

namespace Eris
{

    ShaderProgram::ShaderProgram(Context* context) :
        Resource(context),
        m_handle(0)
    {
    }

    bool ShaderProgram::load(Deserializer& deserializer)
    {
        PROFILE(LoadProgram);

        std::size_t ds_size = deserializer.getSize();
        SharedArrayPtr<char> buffer(ds_size);
        std::size_t in_size = deserializer.read(buffer.get(), ds_size);

        if (ds_size != in_size)
            return false;

        std::stringstream vert_stream, frag_stream;

        char* line = nullptr;
        char* next_line = nullptr;

        line = strtok_s(buffer.get(), "[\r\n\0", &next_line);
        while (line)
        {
            if (line[0] == '#')
            {
                vert_stream << line << std::endl;
                frag_stream << line << std::endl;
            }
            if (strstr(line, "frag"))
            {
                line = strtok_s(nullptr, "[]\0", &next_line);
                line = strtok_s(nullptr, "[]\0", &next_line);
                if (line)
                    frag_stream << line << std::endl;
            }
            else if (strstr(line, "vert"))
            {
                line = strtok_s(nullptr, "[]\0", &next_line);
                line = strtok_s(nullptr, "[]\0", &next_line);
                if (line)
                    vert_stream << line << std::endl;
            }

            line = strtok_s(nullptr, "[\r\n\0", &next_line);
        }

        return compile(vert_stream.str().c_str(), frag_stream.str().c_str());
    }

    bool ShaderProgram::save(Serializer& serializer)
    {
        return true;
    }

    void ShaderProgram::use() const
    {
        ERIS_ASSERT(m_handle > 0);

        Renderer* renderer = m_context->getModule<Renderer>();

        glUseProgram(m_handle);

        for (auto parameter : m_parameters)
        {
            if (parameter.second.data.which() != 0)
                renderer->bindUniform(parameter.second.location, parameter.second.type, parameter.second.data);
        }
    }

    void ShaderProgram::setUniform(const std::string& uniform, const Variant& data)
    {
        if (m_parameters.find(uniform) != m_parameters.end())
            m_parameters[uniform].data = data;
    }

    ShaderUniform* ShaderProgram::getUniform(std::string uniform)
    {
        auto find = m_parameters.find(uniform);
        if (find != m_parameters.end())
            return &find->second;
        return nullptr;
    }

    void ShaderProgram::removeUniform(std::string uniform)
    {
        m_parameters.erase(uniform);
    }

    bool ShaderProgram::compile(const char* vert_source, const char* frag_source)
    {
        ERIS_ASSERT(vert_source);
        ERIS_ASSERT(frag_source);

        GLuint vertex, fragment;
        GLint success;
        GLchar info_log[512];

        GLFWwindow *win = glfwGetCurrentContext();
        Graphics* graphics = m_context->getModule<Graphics>();
        glfwMakeContextCurrent(win ? win : graphics->getResourceWindow());

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vert_source, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, info_log);
            Log::errorf("Vertex Shader compilation failed: %s", info_log);

            glDeleteShader(vertex);

            glfwMakeContextCurrent(win);
            return false;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &frag_source, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, info_log);
            Log::errorf("Fragment Shader compilation failed: %s", info_log);

            glDeleteShader(vertex);
            glDeleteShader(fragment);

            glfwMakeContextCurrent(win);
            return false;
        }

        m_handle = glCreateProgram();

        glAttachShader(m_handle, vertex);
        glAttachShader(m_handle, fragment);
        glLinkProgram(m_handle);

        glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_handle, 512, NULL, info_log);
            Log::errorf("Shader Program linking failed: %s", info_log);

            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(m_handle);

            glfwMakeContextCurrent(win);
            return false;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        glm::i32 uniform_count;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &uniform_count);

        for (auto i = 0; i < uniform_count; i++)
        {
            glm::i32 size;
            glm::u32 type;
            char name[64];
            glGetActiveUniform(m_handle, i, 64, nullptr, &size, &type, name);

            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
                continue;

            ShaderUniform parameter;
            parameter.type = type;
            parameter.location = i;
            m_parameters[name] = parameter;
        }

        glfwMakeContextCurrent(win);

        return true;
    }
}