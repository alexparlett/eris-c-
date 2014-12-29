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

#include "FileSystem.h"

#include "Core/Log.h"

#include <ShlObj.h>

using namespace std::tr2::sys;

namespace Eris
{
    FileSystem::FileSystem(Context* context) : 
        Object(context),
        m_allowed_paths(ChainAllocator<Path>(0, sizeof(Path), 128 * sizeof(Path)))
    {

    }

    void FileSystem::addPath(const Path& path)
    {
        if (!path.empty())
            m_allowed_paths.insert(path);
    }

    void FileSystem::removePath(const Path& path)
    {
        if (!path.empty())
            m_allowed_paths.erase(path);
    }

    bool FileSystem::copy(const Path& src, const Path& dest)
    {
        if (!accessible(src))
        {
            Log::errorf("Unable to access file: %s", src.string());
            return false;
        }

        if (!accessible(dest))
        {
            Log::errorf("Unable to access file: %s", dest.string());
            return false;
        }

        try
        {
            copy_file(src, dest);
            return true;
        }
        catch (basic_filesystem_error<Path> e)
        {
            return false;
        }
    }

    bool FileSystem::rename(const Path& src, const Path& new_name)
    {
        if (!accessible(src))
        {
            Log::errorf("Unable to access file: %s", src.string());
            return false;
        }

        return rename(src, new_name);
    }

    bool FileSystem::remove(const Path& file)
    {
        if (!accessible(file))
        {
            Log::errorf("Unable to access file: %s", file.string());
            return false;
        }

        return remove_all(file) > 0;
    }

    bool FileSystem::accessible(const Path& path) const
    {
        if (m_allowed_paths.empty())
            return true;

        for (auto element : path)
        {
            if (element == "..")
                return false;
        }



        return m_allowed_paths.find(path) != m_allowed_paths.end();
    }

    bool FileSystem::exists(const Path& path) const
    {
        if (!accessible(path))
        {
            Log::warnf("Unable to access file: %s", path.string());
            return false;
        }

        return exists(path);
    }

    bool FileSystem::hasRestrictedPaths() const
    {
        return m_allowed_paths.size() > 0;
    }

    void FileSystem::scanDir(std::vector<Path>& output, const Path& path, std::string filter, glm::uint flags, bool recusive)
    {
        if (!accessible(path))
        {
            Log::warnf("Unable to access path: %s", path.string());
            return;
        }

        recursive_directory_iterator iter(path);

        if (!recusive)
            iter.no_push();

        recursive_directory_iterator prev;
        while (1)
        {
            Path entry_path = iter->path();

            if (entry_path.stem() == ".." || (entry_path.stem()[0] == '.' && (flags & SCAN_HIDDEN) == 0))
            { 
                prev = iter;
                iter++;
                continue;
            }

            if (filter == "*")
                filter.clear();

            std::string filter_extension = StringEmpty;
            if (!filter.empty())
                filter_extension = filter.substr(filter.find_last_of('.'));

            if (is_directory(entry_path) && (flags & SCAN_DIRS) != 0)
            {
                output.push_back(entry_path);
            }
            else if (is_regular_file(entry_path) && (flags & SCAN_FILES) != 0)
            {
                if (filter.empty() || filter_extension == entry_path.extension())
                    output.push_back(entry_path);
            }

            prev = iter;
            if (iter == prev)
                break;
        }
    }

    Path FileSystem::getCurrentDir() const
    {
        return current_path<Path>();
    }

    Path FileSystem::getProgramDir() const
    {
        return initial_path<Path>();
    }

    Path FileSystem::getDocumentsDir() const
    {
        char path[MAX_PATH];
        SHGetSpecialFolderPath(nullptr, path, CSIDL_PERSONAL, FALSE);
        return Path(path);
    }

    Path FileSystem::getApplicationPreferencesDir(const std::string& org, const std::string& app) const
    {
        char path[MAX_PATH];
        SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, FALSE);

        Path out(path);
        out /= org;
        out /= app;

        return out;
    }

}