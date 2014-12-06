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

#include "Object.h"
#include "Mutex.h"
#include "Ptr.h"
#include "StringUtils.h"

namespace Eris
{
    class File;

    enum LogLevel
    {
        LL_ALL = 0,
        LL_DEBUG,
        LL_INFO,
        LL_WARN,
        LL_ERROR,
        LL_NONE
    };

    class Log : public Object
    {
        OBJECT(Log)

    public:
        Log(Context* context);
        virtual ~Log();

        void Open(const String& filename);
        void Close();

        void SetTimestamp(bool enable);
        void SetLevel(LogLevel level);

        void Raw(const String& msg);
        void Info(const String& msg);
        void Debug(const String& msg);
        void Warn(const String& msg);
        void Error(const String& msg);

    public:
        static void WriteStatic(const String& msg, LogLevel level);

    private:
        void Write(const String& msg, LogLevel level);

        bool timestamp_;
        LogLevel level_;
        Mutex mutex_;
        SharedPtr<File> file_;
    };

#define LOGDEBUG(message, ...) Eris::Log::WriteStatic(Eris::ToString(message, __VA_ARGS__), LL_DEBUG);
#define LOGINFO(message, ...) Eris::Log::WriteStatic(Eris::ToString(message, __VA_ARGS__), LL_INFO);
#define LOGWARN(message, ...) Eris::Log::WriteStatic(Eris::ToString(message, __VA_ARGS__), LL_WARN);
#define LOGERROR(message, ...) Eris::Log::WriteStatic(Eris::ToString(message, __VA_ARGS__), LL_ERROR);
}
