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

#include "Texture.h"

#include "Resource/Image.h"
#include "Resource/XMLElement.h"

namespace Eris
{
    WrapMode wrapModesMap[]
    {
        WrapMode::REPEAT,
        WrapMode::MIRRORED_REPEAT,
        WrapMode::CLAMP_TO_BORDER,
        WrapMode::CLAMP_TO_EDGE,
        WrapMode::CLAMP
    };

    Texture::Texture(Context* context) :
        Resource(context),
        m_handle(0),
        m_u_wrap_mode(WrapMode::REPEAT),
        m_v_wrap_mode(WrapMode::REPEAT),
        m_w_wrap_mode(WrapMode::REPEAT),
        m_generate_mip_maps(true)
    {
    }

    void Texture::setGenerateMipMaps(bool generate)
    {
        m_generate_mip_maps = generate;
    }

    void Texture::setUWrapMode(WrapMode u_wrap_mode)
    {
        m_u_wrap_mode = u_wrap_mode;
    }

    void Texture::setVWrapMode(WrapMode v_wrap_mode)
    {
        m_v_wrap_mode = v_wrap_mode;
    }

    void Texture::setWWrapMode(WrapMode w_wrap_mode)
    {
        m_w_wrap_mode = w_wrap_mode;
    }

    glm::i32 Texture::getFormat(Image* image)
    {
        switch (image->getComponents())
        {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return GL_RGB;
        }
    }

    void Texture::parseParameters(const XMLElement& element)
    {
        for (auto ele : element)
        {
            if (ele.getName() == "mipmaps")
            {
                m_generate_mip_maps = ele.getBool(true);
            }
            else if (ele.getName() == "wrap")
            {
                m_u_wrap_mode = wrapModesMap[ele.getI32("u", 0)];
                m_v_wrap_mode = wrapModesMap[ele.getI32("v", 0)];
                m_w_wrap_mode = wrapModesMap[ele.getI32("w", 0)];
            }
        }
    }

    void Texture::setParameters()
    {
        if (m_generate_mip_maps)
            glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_generate_mip_maps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (glm::i32) m_u_wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (glm::i32) m_v_wrap_mode);
    }

}