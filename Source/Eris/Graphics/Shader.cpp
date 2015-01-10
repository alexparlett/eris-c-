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
#include "Shader.h"

#include "Core/Log.h"

namespace Eris
{

    Shader::Shader(Context* context) :
        Resource(context),
        m_handle(-1),
        m_type(ShaderType::FRAG)
    {
    }

    Shader::~Shader()
    {
        if (m_handle != -1)
            release();
    }

    bool Shader::load(Deserializer& deserializer)
    {
        std::size_t ds_size = deserializer.getSize();
        SharedArrayPtr<char> buffer(ds_size);
        std::size_t in_size = deserializer.read(buffer.get(), ds_size);

        if (ds_size != in_size)
            return false;
        
        Graphics* graphics = m_context->getModule<Graphics>();

        if (deserializer.getPath().extension() == ".frag")
            m_type = ShaderType::FRAG;
        else
            m_type = ShaderType::VERT;

        glfwMakeContextCurrent(graphics->getResourceWindow());

        switch (m_type)
        {
        case ShaderType::FRAG:
            m_handle = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::VERT:
            m_handle = glCreateShader(GL_VERTEX_SHADER);
            break;
        }

        const char* buf = buffer.get();
        glShaderSource(m_handle, 1, &buf, nullptr);
        glCompileShader(m_handle);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(m_handle, 512, NULL, infoLog);
            Log::errorf("Shader compilation failed: %s", infoLog);
            glfwMakeContextCurrent(nullptr);
            return false;
        }

        glfwMakeContextCurrent(nullptr);
        return true;
    }

    bool Shader::save(Serializer& serializer)
    {
        return false;
    }

    void Shader::release()
    {
        Graphics* graphics = m_context->getModule<Graphics>();
        GLFWwindow* win = glfwGetCurrentContext();

        if (!win)
            glfwMakeContextCurrent(graphics->getResourceWindow());
        
        glDeleteShader(m_handle);
        m_handle = -1;
        
        if (!win)
            glfwMakeContextCurrent(nullptr);
    }
}