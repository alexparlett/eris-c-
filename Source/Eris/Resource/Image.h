//
// Copyright (c) 2008-2015 the Eris project.
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

#include "Resource.h"

#include "Memory/ArrayPointers.h"

namespace Eris
{
    class Image : public Resource
    {
    public:
        Image(Context* context);
        ~Image();

        virtual bool load(Deserializer& deserializer);
        virtual bool save(Serializer& serializer);

        bool resize(glm::i32 width, glm::i32 height);

        void setWidth(glm::i32 width);
        void setHeight(glm::i32 height);
        void setChannels(glm::i32 channels);
        void setData(unsigned char* data);

        glm::i32 getWidth() const { return m_width; }
        glm::i32 getHeight() const { return m_height; }
        glm::i32 getChannels() const { return m_channels; }
        const unsigned char* getData() const { return m_data; }

    private:
        glm::i32 m_width;
        glm::i32 m_height;
        glm::i32 m_channels;
        SharedArrayPtr<unsigned char> m_data;
    };
}
