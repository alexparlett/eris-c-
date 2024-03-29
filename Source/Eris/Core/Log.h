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

#include "Context.h"
#include "Object.h"

#include "IO/FileSystem.h"
#include "Thread/SpinLock.h"
#include "Collections/Functions.h"

#include <utility>
#include <fstream>

namespace Eris
{
    class Log : public Object
    {
    public:
        enum class Level : glm::u8
        {
            DEBUG,
            INFO,
            WARN,
            FATAL,
            NONE
        };

        Log(Context* context);
        virtual ~Log();

        void open(const Path& file, Level level = Level::FATAL, bool timestamp = true);
        void close();

        void setTimestamp(bool timestamp);
        void setLevel(Level level);
        
        bool opened() const { return m_handle.is_open(); }

        static void raw(const std::string& msg);
        static void debug(const std::string& msg);
        static void info(const std::string& msg);
        static void warn(const std::string& msg);
        static void error(const std::string& msg);

        template<typename... Args>
        static void rawf(const std::string& msg, Args... args)
        {
            raw(std::string_format(msg, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void debugf(const std::string& msg, Args... args)
        {
            debug(std::string_format(msg, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void infof(const std::string& msg, Args... args)
        {
            info(std::string_format(msg, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void warnf(const std::string& msg, Args... args)
        {
            warn(std::string_format(msg, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void errorf(const std::string& msg, Args... args)
        {
            error(std::string_format(msg, std::forward<Args>(args)...));
        }

    private:
        void write(const std::string& msg, Level level);
        static void errorCallback(int error, const char* msg);

        std::ofstream m_handle;
        bool m_timestamp;
        Level m_level;
        SpinLock m_lock;
    };

    template<> inline void Context::registerModule(Log* module)
    {
        m_log = SharedPtr<Log>(module);
    }

    template<> inline Log* Context::getModule()
    {
        ERIS_ASSERT(m_log);
        return m_log.get();
    }
}