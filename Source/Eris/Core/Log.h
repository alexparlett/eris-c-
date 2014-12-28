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

#include "Context.h"
#include "Object.h"

#include "Thread/SpinLock.h"

#include <fstream>

namespace Eris
{
    class Log : public Object
    {
    public:
        enum class Level : glm::u8
        {
            ASSERT,
            DEBUG,
            INFO,
            WARNING,
            FATAL,
            NONE
        };

        Log(Context* context);
        ~Log();

        void open(const std::string& filename, Level level = Level::FATAL, bool timestamp = true);
        void close();

        void setTimestamp(bool timestamp);
        void setLevel(Level level);
        
        bool isOpen() const { return m_handle.is_open(); }

        static void raw(const std::string& msg);
        static void debug(const std::string& msg);
        static void info(const std::string& msg);
        static void warn(const std::string& msg);
        static void fatal(const std::string& msg);

    private:
        void write(const std::string& msg, Level level);

        std::ofstream m_handle;
        bool m_timestamp;
        Level m_level;
        SpinLock m_lock;
    };

    template<> inline void Context::registerModule(Log* engine)
    {
        m_log = SharedPtr<Log>(engine);
    }

    template<> inline Log* Context::getModule()
    {
        ERIS_ASSERT(m_clock);
        return m_log.get();
    }
}