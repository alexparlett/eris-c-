//
// Copyright (c) 2013-2015 the Eris project.
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

#include "Deserializer.h"
#include "Serializer.h"

#include "Core/Context.h"
#include "Core/Object.h"

namespace Eris
{
    enum class FileMode : glm::u8
    {
        READ,
        WRITE
    };

    class File : public Object, public Deserializer, public Serializer
    {
    public:
        File(Context* context);
        File(Context* context, const Path& path, FileMode mode = FileMode::READ);
        virtual ~File();

        void open(const Path& path, FileMode mode = FileMode::READ);
        void flush();
        void close();

        virtual File& operator << (const char* data);
        virtual File& operator << (const std::string& data);
        virtual File& operator << (const SerializerTraits& data);

        virtual File& operator >> (char* buffer);

        virtual std::size_t read(void* buffer, std::size_t count);
        virtual std::size_t seek(std::size_t position);
        virtual std::size_t write(void* data, std::size_t count);

        bool opened() const { return m_handle.is_open(); }

        virtual std::size_t getSize() const { return m_size; }
        FileMode getMode() const { return m_mode; }
        virtual Path getPath() const { return m_path; }

    private:
        std::size_t m_size;
        Path m_path;
        std::fstream m_handle;
        FileMode m_mode;
    };
}