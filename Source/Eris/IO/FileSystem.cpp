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

#include "ArrayPtr.h"
#include "Context.h"
#include "File.h"
#include "FileSystem.h"
#include "Log.h"

#include <windows.h>
#include <Shlobj.h>

namespace Eris
{

    FileSystem::FileSystem(Context* context) :
        Object(context)
    {

    }

    FileSystem::~FileSystem()
    {

    }

    bool FileSystem::SetCurrentDir(const String& pathName)
    {
        if (!CheckAccess(pathName))
            return false;

        if (SetCurrentDirectory(pathName.CString()) == FALSE)
        {
            LOGERROR("Failed to set current directory to %s", pathName);
            return false;
        }

        return true;
    }

    bool FileSystem::CreateDir(const String& pathName)
    {
        if (!CheckAccess(pathName))
        {
            return false;
        }

        bool success = (CreateDirectory(pathName.CString(), 0) == TRUE) || (GetLastError() == ERROR_ALREADY_EXISTS);

        if (success)
            LOGDEBUG("Created directory %s", pathName);
        else
            LOGERROR("Failed to create directory %s", pathName);

        return success;
    }

    bool FileSystem::Copy(const String& srcFileName, const String& destFileName)
    {
        if (!CheckAccess(GetPath(srcFileName)) || !CheckAccess(GetPath(destFileName)))
            return false;

        SharedPtr<File> src = SharedPtr<File>(new File(context_, srcFileName, FM_READ));
        if (!src->IsOpen())
            return false;

        SharedPtr<File> dest = SharedPtr<File>(new File(context_, destFileName, FM_WRITE, WM_TRUNC));
        if (!dest->IsOpen())
            return false;

        SharedArrayPtr<void> buffer(new unsigned char[src->GetSize()]);
        *src >> *buffer;
        *dest << *buffer;

        return src->GetSize() == dest->GetSize();
    }

    bool FileSystem::Rename(const String& srcFileName, const String& destFileName)
    {
        if (!CheckAccess(GetPath(srcFileName)) || !CheckAccess(GetPath(destFileName)))
            return false;

        return MoveFile(srcFileName.CString(), destFileName.CString()) != FALSE;
    }

    bool FileSystem::Delete(const String& fileName)
    {
        if (!CheckAccess(GetPath(fileName)))
            return false;

        return DeleteFile(fileName.CString()) != FALSE;
    }

    void FileSystem::RegisterPath(const String& pathName)
    {
        if (pathName.Empty())
            return;

        allowedPaths_.Insert(pathName);
    }

    void FileSystem::RemovePath(const String& pathName)
    {
        if (pathName.Empty())
            return;

        allowedPaths_.Erase(pathName);
    }

    Eris::String FileSystem::GetCurrentDir() const
    {
        char path[MAX_PATH];
        path[0] = 0;
        GetCurrentDirectory(MAX_PATH, path);
        return AddTrailingSlash(String(path));
    }

    bool FileSystem::CheckAccess(const String& pathName) const
    {
        String fixedPath = AddTrailingSlash(pathName);

        // If no allowed directories defined, succeed always
        if (allowedPaths_.Empty())
            return true;

        // If there is any attempt to go to a parent directory, disallow
        if (fixedPath.Contains(".."))
            return false;

        // Check if the path is a partial match of any of the allowed directories
        for (HashSet<String>::ConstIterator i = allowedPaths_.Begin(); i != allowedPaths_.End(); ++i)
        {
            if (fixedPath.Find(*i) == 0)
                return true;
        }

        // Not found, so disallow
        return false;
    }

    unsigned FileSystem::GetLastModifiedTime(const String& fileName) const
    {
        if (fileName.Empty() || !CheckAccess(fileName))
            return 0;

        struct _stat st;
        if (!_stat(fileName.CString(), &st))
            return (unsigned)st.st_mtime;
        else
            return 0;
    }

    bool FileSystem::FileExists(const String& fileName) const
    {
        if (!CheckAccess(GetPath(fileName)))
            return false;

        String fixedName = GetPath(RemoveTrailingSlash(fileName));

        DWORD attributes = GetFileAttributes(fixedName.CString());
        if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
            return false;

        return true;
    }

    bool FileSystem::DirExists(const String& pathName) const
    {
        if (!CheckAccess(pathName))
            return false;

        String fixedName = RemoveTrailingSlash(pathName);

        DWORD attributes = GetFileAttributes(fixedName.CString());
        if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            return false;

        return true;
    }

    void FileSystem::ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags, bool recursive) const
    {
        result.Clear();

        if (CheckAccess(pathName))
        {
            String initialPath = AddTrailingSlash(pathName);
            ScanDirInternal(result, initialPath, initialPath, filter, flags, recursive);
        }
    }

    Eris::String FileSystem::GetProgramDir() const
    {
        return GetPath(_pgmptr);
    }

    Eris::String FileSystem::GetUserDocumentsDir() const
    {
        char pathName[MAX_PATH];
        pathName[0] = 0;
        SHGetSpecialFolderPath(0, pathName, CSIDL_PERSONAL, 0);
        return AddTrailingSlash(String(pathName));
    }

    Eris::String FileSystem::GetAppPreferencesDir(const String& org, const String& app) const
    {
        char pathName[MAX_PATH];
        pathName[0] = 0;
        SHGetSpecialFolderPath(0, pathName, CSIDL_APPDATA, 0);
        return AddTrailingSlash(String(pathName));
    }

    void FileSystem::ScanDirInternal(Vector<String>& result, String path, const String& startPath, const String& filter, unsigned flags, bool recursive) const
    {
        path = AddTrailingSlash(path);
        String deltaPath;
        if (path.Length() > startPath.Length())
            deltaPath = path.Substring(startPath.Length());

        String filterExtension = filter.Substring(filter.Find('.'));
        if (filterExtension.Contains('*'))
            filterExtension.Clear();

        WIN32_FIND_DATAW info;
        HANDLE handle = FindFirstFileW(WString(path + "*").CString(), &info);
        if (handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                String fileName(info.cFileName);
                if (!fileName.Empty())
                {
                    if (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && !(flags & SF_HIDDEN))
                        continue;
                    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (flags & SF_DIRS)
                            result.Push(deltaPath + fileName);
                        if (recursive && fileName != "." && fileName != "..")
                            ScanDirInternal(result, path + fileName, startPath, filter, flags, recursive);
                    }
                    else if (flags & SF_FILES)
                    {
                        if (filterExtension.Empty() || fileName.EndsWith(filterExtension))
                            result.Push(deltaPath + fileName);
                    }
                }
            } while (FindNextFileW(handle, &info));

            FindClose(handle);
        }
    }

    void SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension)
    {
        String fullPathCopy = GetInternalPath(fullPath);

        unsigned extPos = fullPathCopy.FindLast('.');
        unsigned pathPos = fullPathCopy.FindLast('/');

        if (extPos != String::NPOS && (pathPos == String::NPOS || extPos > pathPos))
        {
            extension = fullPathCopy.Substring(extPos);
            if (lowercaseExtension)
                extension = extension.ToLower();
            fullPathCopy = fullPathCopy.Substring(0, extPos);
        }
        else
            extension.Clear();

        pathPos = fullPathCopy.FindLast('/');
        if (pathPos != String::NPOS)
        {
            fileName = fullPathCopy.Substring(pathPos + 1);
            pathName = fullPathCopy.Substring(0, pathPos + 1);
        }
        else
        {
            fileName = fullPathCopy;
            pathName.Clear();
        }
    }

    String GetPath(const String& fullPath)
    {
        String path, file, extension;
        SplitPath(fullPath, path, file, extension);
        return path;
    }

    String GetFileName(const String& fullPath)
    {
        String path, file, extension;
        SplitPath(fullPath, path, file, extension);
        return file;
    }

    String GetExtension(const String& fullPath, bool lowercaseExtension)
    {
        String path, file, extension;
        SplitPath(fullPath, path, file, extension, lowercaseExtension);
        return extension;
    }

    String GetFileNameAndExtension(const String& fileName, bool lowercaseExtension)
    {
        String path, file, extension;
        SplitPath(fileName, path, file, extension, lowercaseExtension);
        return file + extension;
    }

    String ReplaceExtension(const String& fullPath, const String& newExtension)
    {
        String path, file, extension;
        SplitPath(fullPath, path, file, extension);
        return path + file + newExtension;
    }

    String AddTrailingSlash(const String& pathName)
    {
        String ret = pathName.Trimmed();
        ret.Replace('\\', '/');
        if (!ret.Empty() && ret.Back() != '/')
            ret += '/';
        return ret;
    }

    String RemoveTrailingSlash(const String& pathName)
    {
        String ret = pathName.Trimmed();
        ret.Replace('\\', '/');
        if (!ret.Empty() && ret.Back() == '/')
            ret.Resize(ret.Length() - 1);
        return ret;
    }

    String GetParentPath(const String& path)
    {
        unsigned pos = RemoveTrailingSlash(path).FindLast('/');
        if (pos != String::NPOS)
            return path.Substring(0, pos + 1);
        else
            return String();
    }

    String GetInternalPath(const String& pathName)
    {
        return pathName.Replaced('\\', '/');
    }

    String GetNativePath(const String& pathName)
    {
        return pathName.Replaced('/', '\\');
    }

    WString GetWideNativePath(const String& pathName)
    {
        return WString(pathName.Replaced('/', '\\'));
    }

    bool IsAbsolutePath(const String& pathName)
    {
        if (pathName.Empty())
            return false;

        String path = GetInternalPath(pathName);

        if (path[0] == '/')
            return true;

        if (path.Length() > 1 && IsAlpha(path[0]) && path[1] == ':')
            return true;

        return false;
    }
}