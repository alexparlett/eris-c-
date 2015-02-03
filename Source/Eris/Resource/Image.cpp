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

#include "Core/Log.h"
#include "Core/Profiler.h"

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
        m_components(0)
    {
    }

    bool Image::load(Deserializer& deserializer)
    {
        PROFILE(LoadImage);

        std::size_t ds_size = deserializer.getSize();

        SharedArrayPtr<unsigned char> buffer(ds_size);
        std::size_t read_size = deserializer.read(buffer.get(), ds_size);

        if (!buffer || read_size != ds_size)
            return false;

        m_data = stbi_load_from_memory(buffer, ds_size, &m_width, &m_height, &m_components, 0);

        if (!m_data || m_width <= 0 || m_height <= 0 || m_components <= 0)
        {
            Log::errorf("Failed loading Image: %s", stbi_failure_reason());
            return false;
        }

        return true;
    }

    bool Image::save(Serializer& serializer)
    {
        glm::i32 out_size;
        SharedArrayPtr<unsigned char> buffer(stbi_write_png_to_mem(m_data, 0, m_width, m_height, m_components, &out_size));

        if (!buffer || out_size <= 0)
            return false;

        serializer.write(buffer, out_size);

        return true;
    }

    bool Image::resize(glm::i32 width, glm::i32 height)
    {
        if (m_width == width && m_height == height)
            return false;

        if (width == 0 || height == 0)
            return false;

        SharedArrayPtr<unsigned char> new_buffer(width * height * m_components);
        glm::i32 new_width = 0, new_height = 0;
        if (stbir_resize_uint8(m_data.get(), m_width, m_height, 0, new_buffer.get(), new_width, new_height, 0, m_components) == 0)
        {
            Log::errorf("Failed to resize image: %s to width: %d height: %d", getName(), width, height);
            return false;
        }

        m_width = new_width;
        m_height = new_height;
        m_data = new_buffer;

        return true;
    }

    void Image::flip()
    {
        glm::u64 row_size = m_components * m_width;
        SharedArrayPtr<unsigned char> row_buffer(row_size);
        glm::u32 half_rows = m_height / 2;

        for (glm::u32 i = 0; i < half_rows; i++)
        {
            unsigned char* row = m_data.get() + getPixelOffset(0, i, m_width, m_height, m_components);
            unsigned char* oppositeRow = m_data.get() + getPixelOffset(0, m_height - i - 1, m_width, m_height, m_components);

            memcpy(row_buffer.get(), row, row_size);
            memcpy(row, oppositeRow, row_size);
            memcpy(oppositeRow, row_buffer.get(), row_size);
        }
    }

    void Image::setWidth(glm::i32 width)
    {
        ERIS_ASSERT(width > 0);
        if (width > 0)
            m_width = width;
    }

    void Image::setHeight(glm::i32 height)
    {
        ERIS_ASSERT(height > 0);
        if (height > 0)
            m_height = height;
    }

    void Image::setComponents(glm::i32 components)
    {
        ERIS_ASSERT(components > 0);
        if (components > 0)
            m_components = components;
    }

    void Image::setData(unsigned char* data)
    {
        ERIS_ASSERT(data);
        if (data)
            m_data = data;
    }

    glm::u32 Image::getPixelOffset(glm::u32 column, glm::u32 row, glm::i32 width, glm::i32 height, glm::i32 channels)
    {
        return (row * width + column) * channels;
    }

}