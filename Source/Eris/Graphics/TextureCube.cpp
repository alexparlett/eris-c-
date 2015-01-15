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
#include "TextureCube.h"

#include "Core/Log.h"
#include "Resource/ResourceCache.h"
#include "Resource/Image.h"
#include "Resource/XMLFile.h"

namespace Eris
{
    glm::i32 facesMap[]
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    TextureCube::TextureCube(Context* context) :
        Texture(context)
    {
    }

    bool TextureCube::load(Deserializer& deserializer)
    {
        ResourceCache* rc = m_context->getModule<ResourceCache>();

        SharedPtr<XMLFile> file(new XMLFile(m_context));
        if (!file->load(deserializer))
            return false;

        std::map<glm::i32, SharedPtr<Image>> faces;
        XMLElement facesEle = file->getRoot().getChild("faces");
        for (auto face : facesEle)
        {
            Path image_path = face.getValue();
            if (image_path.parent_path().empty())
                image_path = deserializer.getPath().parent_path() /= image_path;
            
            SharedPtr<Image> image = SharedPtr<Image>(rc->getTempResource<Image>(image_path));
            if (!image)
                return false;

            image->flip();

            faces[facesMap[face.getI32("pos", 0)]] = image;
        }

        if (faces.size() != 6)
        {
            Log::error("Failed loading TextureCube: not enough faces defined");
            return false;
        }

        parseParameters(file->getRoot());

        return compile(faces);
    }

    bool TextureCube::save(Serializer& serializer)
    {
        return true;
    }

    void TextureCube::use() const
    {
        ERIS_ASSERT(m_handle > 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
    }

    bool TextureCube::compile(const std::map<glm::i32, SharedPtr<Image>>& faces)
    {
        GLFWwindow *win = glfwGetCurrentContext();
        Graphics* graphics = m_context->getModule<Graphics>();
        glfwMakeContextCurrent(win ? win : graphics->getResourceWindow());

        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

        for (auto face : faces)
        {
            glm::i32 unit = face.first;
            glm::i32 format = getFormat(face.second);

            glGetError();
            glTexImage2D(unit, 0, format, face.second->getWidth(), face.second->getHeight(), 0, format, GL_UNSIGNED_BYTE, face.second->getData());
            if (glGetError())
            {
                glfwMakeContextCurrent(win);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                glDeleteTextures(1, &m_handle);
                return false;
            }
        }

        setParameters();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (glm::i32) m_k_wrap_mode);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glfwMakeContextCurrent(win);

        return true;
    }
}