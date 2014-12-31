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

#include "File.h"
#include "FileSystem.h"

#include "Core/Log.h"

namespace Eris
{

    File::File(Context* context) : 
        Object(context),
        m_mode(FileMode::READ)
    {
    }

    File::File(Context* context, const Path& path, FileMode mode /*= FileMode::READ*/) :
        Object(context),
        m_mode(mode),
        m_path(path)
    {
        open(m_path, m_mode);
    }

    File::~File()
    {
        flush();
        close();
    }

    void File::open(const Path& path, FileMode mode /*= FileMode::READ*/)
    {
        FileSystem* fs = m_context->getModule<FileSystem>();
        if (!fs->accessible(path))
        {
            Log::errorf("Access denied: %s", path.string());
            return;
        }

        if (mode == FileMode::READ)
            m_stream.open(path, std::ios::in);
        else
            m_stream.open(path, std::ios::out | std::ios::trunc);

        if (m_stream.bad())
        {
            Log::errorf("Unable to open file: %s", path.string());
            return;
        }

        m_path = path;
        m_mode = mode;
    }

    void File::flush()
    {
        if (m_stream.is_open())
            m_stream.flush();
    }

    void File::close()
    {
        if (m_stream.is_open())
            m_stream.close();
    }

    File& File::operator<<(const void* data)
    {
        if (m_stream.is_open())
            m_stream << data;

        flush();

        return *this;
    }

    File& File::operator>>(void* buffer)
    {
        if (m_stream.is_open())
            m_stream >> buffer;

        return *this;
    }

    std::size_t File::read(void* buffer, std::size_t count)
    {
        std::size_t opos = m_stream.tellg();
        m_stream.read((char*) buffer, count);
        std::size_t npos = m_stream.tellg();

        return npos - opos;
    }

    std::size_t File::seek(std::size_t position)
    {
        m_stream.seekg(position);
        m_stream.seekp(position);

        std::size_t gpos = m_stream.tellg();
        std::size_t ppos = m_stream.tellp();

        return glm::max(gpos, ppos);
    }

    std::size_t File::write(void* data, std::size_t count)
    {
        std::size_t opos = m_stream.tellp();
        m_stream.write((char*) data, count);
        std::size_t npos = m_stream.tellp();

        return npos - opos;
    }

    bool File::isOpen() const
    {
        return m_stream.is_open();
    }

    bool File::isEmpty() const
    {
        return !isOpen() || getSize() <= 0;
    }

    std::size_t File::getSize() const
    {
        return file_size(m_path);
    }
}