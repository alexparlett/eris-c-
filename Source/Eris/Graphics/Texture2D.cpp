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
#include "Core/Profiler.h"
#include "Memory/Pointers.h"
#include "Resource/Image.h"
#include "Resource/ResourceCache.h"
#include "Resource/XMLFile.h"

namespace Eris
{
    Texture2D::Texture2D(Context* context) :
        Texture(context)
    {
    }

    bool Texture2D::load(Deserializer& deserializer)
    {
        PROFILE(LoadTexture2D);

        SharedPtr<Image> image(new Image(m_context));
        if (!image->load(deserializer))
            return false;

        image->flip();

        Path xml_path = deserializer.getPath();
        xml_path.replace_extension(".tex");
        SharedPtr<XMLFile> properties = SharedPtr<XMLFile>(m_context->getModule<ResourceCache>()->getTempResource<XMLFile>(xml_path));
        if (properties)
            parseParameters(properties->getRoot());

        glm::i32 format = getFormat(image);

        GLFWwindow *win = glfwGetCurrentContext();
        Graphics* graphics = m_context->getModule<Graphics>();
        glfwMakeContextCurrent(win ? win : graphics->getResourceWindow());

        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glGetError();
        glTexImage2D(GL_TEXTURE_2D, 0, format, image->getWidth(), image->getHeight(), 0, format, GL_UNSIGNED_BYTE, image->getData());
        if (glGetError())
        {
            glfwMakeContextCurrent(win);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &m_handle);
            return false;
        }

        setParameters();

        glBindTexture(GL_TEXTURE_2D, 0);
        glfwMakeContextCurrent(win);

        return true;
    }

    bool Texture2D::save(Serializer& serializer)
    {
        return true;
    }

    void Texture2D::use() const
    {
        ERIS_ASSERT(m_handle > 0);
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }
}