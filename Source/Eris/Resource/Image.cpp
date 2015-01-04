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

#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>

namespace Eris
{

    Image::Image(Context* context) :
        Resource(context),
        m_data(nullptr),
        m_height(0),
        m_width(0),
        m_channels(0)
    {
    }

    Image::~Image()
    {
    }

    bool Image::load(Deserializer& deserializer)
    {
        std::size_t ds_size = deserializer.getSize();

        SharedArrayPtr<unsigned char> buffer(ds_size);
        std::size_t read_size = deserializer.read(buffer.get(), ds_size);

        if (!buffer || read_size != ds_size)
            return false;

        m_data = stbi_load_from_memory(buffer, ds_size, &m_width, &m_height, &m_channels, 0);

        if (!m_data || m_width <= 0 || m_height <= 0 || m_channels <= 0)
            return false;

        return true;
    }

    bool Image::save(Serializer& serializer)
    {
        glm::i32 out_size;
        SharedArrayPtr<unsigned char> buffer(stbi_write_png_to_mem(m_data, 0, m_width, m_height, m_channels, &out_size));

        if (!buffer || out_size <= 0)
            return false;

        serializer.write(buffer, out_size);

        return true;
    }

}