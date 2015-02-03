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

#include "Core/Context.h"
#include "Resource/Resource.h"

namespace Eris
{
    class Image;
    class XMLElement;

    enum class WrapMode : glm::i32
    {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
        CLAMP = GL_CLAMP
    };

    class Texture : public Resource
    {
    public:
        Texture(Context* context);
        virtual ~Texture() {}

        glm::u32 getHandle() const { return m_handle; }
        bool getGenerateMipMaps() const { return m_generate_mip_maps; }
        WrapMode getUWrapMode() const { return m_u_wrap_mode; }
        WrapMode getVWrapMode() const { return m_v_wrap_mode; }
        WrapMode getWWrapMode() const { return m_w_wrap_mode; }

        void setGenerateMipMaps(bool generate);
        void setUWrapMode(WrapMode u_wrap_mode);
        void setVWrapMode(WrapMode v_wrap_mode);
        void setWWrapMode(WrapMode k_wrap_mode);

        virtual void use() const = 0;

    protected:
        glm::i32 getFormat(Image* image);
        void parseParameters(const XMLElement& element);
        void setParameters();

        bool m_generate_mip_maps;
        WrapMode m_u_wrap_mode;
        WrapMode m_v_wrap_mode;
        WrapMode m_w_wrap_mode;
        glm::u32 m_handle;
    };
}