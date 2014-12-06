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
#include "Log.h"
#include "Timer.h"

namespace Eris
{
    static Log* logInstance;

    Log::Log(Context* context) : 
        Object(context),
        file_(0),
        timestamp_(true),
        level_(LL_WARN)
    {
        logInstance = this;
    }

    Log::~Log()
    {
        if (file_->IsOpen())
            file_->Close();

        file_.Reset();
    }

    void Log::Open(const String& filename)
    {
        if (file_->IsOpen())
        {
            if (file_->GetName() == filename)
                return;

            file_->Close();
            file_.Reset();
        }

        if (!file_->Open(filename, FM_WRITE, WM_TRUNC))
        {

        }
    }

    void Log::Close()
    {
        if (file_->IsOpen())
        {
            file_->Close();
            file_.Reset();
        }
    }

    void Log::SetTimestamp(bool enable)
    {
        timestamp_ = enable;
    }

    void Log::SetLevel(LogLevel level)
    {
        level_ = level;
    }

    void Log::Raw(const String& msg)
    {
        Write(msg, LL_ALL);
    }

    void Log::Info(const String& msg)
    {
        Write(msg, LL_INFO);
    }

    void Log::Debug(const String& msg)
    {
        Write(msg, LL_DEBUG);
    }

    void Log::Warn(const String& msg)
    {
        Write(msg, LL_WARN);
    }

    void Log::Error(const String& msg)
    {
        Write(msg, LL_ERROR);
    }

    void Log::WriteStatic(const String& msg, LogLevel level)
    {
        if (logInstance)
            logInstance->Write(msg, level);
    }

    void Log::Write(const String& msg, LogLevel level)
    {
        if (level >= level_ && file_ && file_->IsOpen())
        {
            MutexGuard lock(mutex_);

            if (timestamp_)
            {
                file_ << "[";
                file_ << Time::GetTimestamp().CString();
                file_ << "] ";
            }

            file_ << msg.CString();
            file_ << std::endl;
        }
    }
}