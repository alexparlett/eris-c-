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

#include "Context.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"
#include "MathDefs.h"

namespace Eris
{


    File::File(Context* context) : 
        Object(context)
    {

    }

    File::File(Context* context, const String& filename, FileMode mode /*= FM_READ*/, WriteMode /*= WM_TRUNC*/) :
        Object(context)
    {

    }

    File::~File()
    {

    }

    File& File::operator<<(const void * data)
    {
        if (handle_.is_open() && (fileMode_ == FM_WRITE || fileMode_ == FM_READWRITE))
            handle_ << data;

        return *this;
    }

    File& File::operator>>(void* data)
    {
        if (handle_.is_open() && (fileMode_ == FM_READ || fileMode_ == FM_READWRITE))
            handle_ >> data;

        return *this;
    }

    unsigned File::Read(void* dest, unsigned size)
    {
        if (!handle_.is_open())
            return 0;

        if (fileMode_ == FM_WRITE)
        {
            LOGERROR("Attempted to read File with mode set to Write.");
            return 0;
        }

        handle_.read((char*)dest, size);
        return handle_.gcount();
    }

    unsigned File::Seek(unsigned position)
    {
        if (!handle_.is_open())
            return 0;

        if (fileMode_ == FM_READ)
        {
            handle_.seekg(0,position);
            return handle_.tellg();
        }
        else if (fileMode_ == FM_WRITE)
        {
            handle_.seekp(0,position);
            return handle_.tellp();
        }
        else
        {
            handle_.seekg(0,position);
            handle_.seekp(0,position);
            return handle_.tellg() > handle_.tellp() ? handle_.tellg() : handle_.tellp();
        }
    }

    unsigned File::Write(const void* data, unsigned size)
    {
        if (!handle_.is_open())
            return 0;

        if (fileMode_ == FM_READ)
        {
            LOGERROR("Attempted to write File with mode set to Read.");
            return 0;
        }

        unsigned start = handle_.tellp();

        handle_.write((char*)data, size);
        if (handle_.bad())
        {
            LOGERROR("Failed writing to file.");
            handle_.seekp(0, start);
            return 0;
        }

        if (fileMode_ == FM_READWRITE)
            handle_.sync();

        unsigned current = handle_.tellp();
        handle_.seekp(0, handle_.end);
        unsigned total = handle_.tellp();
        handle_.seekp(0, current);

        return total;
    }

    unsigned File::GetChecksum()
    {
        if (checksum_)
            return checksum_;

        if (!handle_.is_open() || fileMode_ == FM_WRITE)
            return 0;

        unsigned oldPos = handle_.tellg();
        checksum_ = 0;

        Seek(0);
        while (!handle_.eof())
        {
            unsigned char block[1024];
            unsigned readBytes = Read(block, 1024);
            for (unsigned i = 0; i < readBytes; i++)
                checksum_ = SDBMHash(checksum_, block[i]);
        }
        Seek(oldPos);

        return checksum_;
    }

    bool File::Open(const String& filename, FileMode fm, WriteMode wm)
    {
        if (filename.Empty())
        {
            LOGERROR("Could not open file with empty name.");
            return false;
        }

        Close();

        FileSystem* fs = context_->GetModule<FileSystem>();
        if (fs && !fs->CheckAccess(fileName))
        {
            LOGERROR("Access denied to %s", filename);
            return false;
        }

        unsigned openmode = 0;

        switch (fm)
        {
        case FM_READ:
            openmode |= std::ios::in;
            break;
        case FM_WRITE:
            openmode |= std::ios::out;
            break;
        case FM_READWRITE:
            openmode |= std::ios::in;
            openmode |= std::ios::out;
            break;
        }

        if (fm == FM_READWRITE || fm == FM_WRITE)
        {
            switch (wm)
            {
            case WM_APPEND:
                openmode |= std::ios::app;
                break;
            case WM_TRUNC:
                openmode |= std::ios::trunc;
                break;
            }
        }

        handle_.open(filename.CString(), openmode);

        if (handle_.bad())
        {
            LOGERROR("Could not open file %s", filename);
            return false;
        }

        fileName_ = filename;
        fileMode_ = fm;
        writeMode_ = wm;
        checksum_ = 0;

        return true;
    }

    void File::Flush()
    {
        if (!handle_.is_open())
            return;

        handle_.flush();
    }

    void File::Close()
    {
        if (handle_.is_open())
        {
            checksum_ = 0;
            handle_.close();
        }
    }
}