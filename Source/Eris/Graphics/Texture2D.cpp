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
#include "Texture2D.h"

#include "Core/Log.h"
#include "Memory/Pointers.h"
#include "Resource/Image.h"

namespace Eris
{

    Texture2D::Texture2D(Context* context) :
        Resource(context),
        m_handle(0),
        m_u_wrap_mode(WrapMode::REPEAT),
        m_v_wrap_mode(WrapMode::REPEAT),
        m_generate_mip_maps(true)
    {
    }

    bool Texture2D::load(Deserializer& deserializer)
    {
        SharedPtr<Image> image(new Image(m_context));
        if (!image->load(deserializer))
            return false;

        image->flip();

        GLint format;
        switch (image->getComponents())
        {
        case 1:
            format = GL_ALPHA;
            break;
        case 2:
            format = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        GLFWwindow *win = glfwGetCurrentContext();
        Graphics* graphics = m_context->getModule<Graphics>();
        glfwMakeContextCurrent(win ? win : graphics->getResourceWindow());

        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glGetError();
        glTexImage2D(GL_TEXTURE_2D, 0, format, image->getWidth(), image->getHeight(), 0, format, GL_UNSIGNED_BYTE, image->getData());
        if (glGetError())
        {
            Log::errorf("Failed creating texture: %d", deserializer.getPath().string().c_str());
            glfwMakeContextCurrent(win);
            return false;
        }

        if (m_generate_mip_maps)
            glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) m_u_wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) m_v_wrap_mode);

        glBindTexture(GL_TEXTURE_2D, 0);

        glfwMakeContextCurrent(win);

        return true;
    }

    bool Texture2D::save(Serializer& serializer)
    {
        return true;
    }

    void Texture2D::setGenerateMipMaps(bool generate)
    {
        m_generate_mip_maps = generate;
    }

    void Texture2D::setUWrapMode(WrapMode u_wrap_mode)
    {
        m_u_wrap_mode = u_wrap_mode;
    }

    void Texture2D::setVWrapMode(WrapMode v_wrap_mode)
    {
        m_v_wrap_mode = v_wrap_mode;
    }

    void Texture2D::use() const
    {
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }

}