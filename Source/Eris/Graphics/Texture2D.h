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
    enum class WrapMode : GLint
    {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
        CLAMP = GL_CLAMP
    };

    class Texture2D : public Resource
    {
    public:
        Texture2D(Context* context);

        virtual bool load(Deserializer& deserializer) override;
        virtual bool save(Serializer& serializer) override;

        GLuint getHandle() const { return m_handle; }
        bool getGenerateMipMaps() const { return m_generate_mip_maps; }
        WrapMode getUWrapMode() const { return m_u_wrap_mode; }
        WrapMode getVWrapMode() const { return m_v_wrap_mode; }

        void setGenerateMipMaps(bool generate);
        void setUWrapMode(WrapMode u_wrap_mode);
        void setVWrapMode(WrapMode v_wrap_mode);

        void use() const;

    private:
        bool m_generate_mip_maps;
        WrapMode m_u_wrap_mode;
        WrapMode m_v_wrap_mode;
        GLuint m_handle;
    };
}