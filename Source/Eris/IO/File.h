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

#include <fstream>

namespace Eris
{
    enum FileMode
    {
        FM_READ = 0,
        FM_WRITE,
        FM_READWRITE
    };

    enum WriteMode
    {
        WM_TRUNC = 0,
        WM_APPEND
    };

    class File : public Object
    {
        OBJECT(File)

    public:
        File(Context* context);
        File(Context* context, const String& filename, FileMode mode = FM_READ, WriteMode  = WM_TRUNC);
        virtual ~File();

        File& operator<< (const char* data);
        File& operator<< (const void* data);
        File& operator>> (void* data);

        virtual unsigned Read(void* dest, unsigned size);
        virtual unsigned Seek(unsigned position);
        virtual unsigned Write(const void* data, unsigned size);
        virtual const String& GetName() const { return fileName_; }
        virtual unsigned GetChecksum() const;

        bool Open(const String& filename, FileMode fm = FM_READ, WriteMode wm = WM_TRUNC);
        void Flush();
        void Close();
        void SetName();

        FileMode GetFileMode() const { return fileMode_; }
        WriteMode GetWriteMode() const { return writeMode_; }
        bool IsOpen() const { return handle_.is_open(); }

    private:
        std::fstream handle_;
        FileMode fileMode_;
        WriteMode writeMode_;
        String fileName_;
    };
}
