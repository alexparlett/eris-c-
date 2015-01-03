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
            m_handle.open(path, std::ios::in);
        else
            m_handle.open(path, std::ios::out | std::ios::trunc);

        if (m_handle.bad() || m_handle.fail())
        {
            Log::errorf("Unable to open file: %s", path.string());
            return;
        }

        m_handle.seekg(0, m_handle.end);
        m_size = m_handle.tellg();
        m_handle.seekg(0, m_handle.beg);

        m_path = path;
        m_mode = mode;
    }

    void File::flush()
    {
        if (m_handle.is_open())
            m_handle.flush();
    }

    void File::close()
    {
        if (m_handle.is_open())
            m_handle.close();
    }

    File& File::operator<<(const void* data)
    {
        if (m_handle.is_open())
            m_handle << data;

        flush();

        return *this;
    }

    File& File::operator>>(void* buffer)
    {
        if (m_handle.is_open())
            m_handle >> buffer;

        return *this;
    }

    File& File::operator>>(char* buffer)
    {
        if (m_handle.is_open())
            m_handle >> buffer;

        return *this;
    }

    std::size_t File::read(void* buffer, std::size_t count)
    {
        if (m_handle.is_open())
        {
            m_handle.read((char*) buffer, count);
            return m_handle.gcount();
        }

        return 0;
    }

    std::size_t File::seek(std::size_t position)
    {
        if (m_handle.is_open())
        {
            m_handle.seekg(position);
            m_handle.seekp(position);

            std::size_t gpos = m_handle.tellg();
            std::size_t ppos = m_handle.tellp();

            return glm::max(gpos, ppos);
        }

        return 0;
    }

    std::size_t File::write(void* data, std::size_t count)
    {
        if (m_handle.is_open())
        {
            std::size_t opos = m_handle.tellp();
            m_handle.write((char*) data, count);
            std::size_t npos = m_handle.tellp();

            return npos - opos;
        }

        return 0;
    }
}