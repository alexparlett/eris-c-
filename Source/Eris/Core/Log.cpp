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

#include "Log.h"

#include "Core/Clock.h"
#include "Thread/Lock.h"

#include "../gitversion.h"

namespace Eris
{
    static Log* log;

    Log::Log(Context* context) :
        Object(context),
        m_level(Level::FATAL),
        m_timestamp(true)
    {
        log = this;
        glfwSetErrorCallback(&Log::errorCallback);
    }

    Log::~Log()
    {
        if (m_handle.is_open())
            m_handle.close();
    }

    void Log::open(const Path& file, Level level /*= Level::FATAL*/, bool timestamp /*= true*/)
    {
        m_handle.open(file, std::ios::trunc | std::ios::out);
        ERIS_ASSERT(m_handle.good());
        if (!m_handle.good())
        {
            if (m_handle.is_open())
                m_handle.close();
            return;
        }

        rawf("Starting version %s", ERIS_VERSION);
    }

    void Log::close()
    {
        if (m_handle.is_open())
        {
            m_handle.close();
        }
    }

    void Log::setTimestamp(bool timestamp)
    {
        m_timestamp = timestamp;
    }

    void Log::setLevel(Level level)
    {
        m_level = level;
    }

    void Log::raw(const std::string& msg)
    {
        if (log && log->isOpen())
            log->write(msg, Level::NONE);
    }

    void Log::debug(const std::string& msg)
    {
        if (log && log->isOpen())
            log->write(msg, Level::DEBUG);
    }

    void Log::info(const std::string& msg)
    {
        if (log && log->isOpen())
            log->write(msg, Level::INFO);
    }

    void Log::warn(const std::string& msg)
    {
        if (log && log->isOpen())
            log->write(msg, Level::WARN);
    }

    void Log::error(const std::string& msg)
    {
        if (log && log->isOpen())
            log->write(msg, Level::FATAL);
    }

    void Log::write(const std::string& msg, Level level)
    {
        if (level >= m_level)
        {
            LockGuard<SpinLock> lock(m_lock);

            if (m_timestamp)
                m_handle << "[" << m_context->getModule<Clock>()->getTimestamp().c_str() << "] ";

            switch (level)
            {
            case Level::NONE:
                break;
            case Level::DEBUG:
                m_handle << "[DEBUG] ";
                break;
            case Level::INFO:
                m_handle << "[INFO] ";
                break;
            case Level::WARN:
                m_handle << "[WARN] ";
                break;
            case Level::FATAL:
                m_handle << "[ERROR] ";
                break;
            default:
                break;
            }
    
            m_handle << msg.c_str();
            m_handle << std::endl;
        }
    }

    void Log::errorCallback(int error, const char* msg)
    {
        Eris::Log::errorf("GLFW error: %d %s", error, msg);
    }
}