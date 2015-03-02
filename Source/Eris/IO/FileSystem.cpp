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

#include "FileSystem.h"

#include "Core/Log.h"

#include <ShlObj.h>

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
        {
            m_allowed_paths.insert(path);
            Log::infof("Added allowed path %s", path);
        }
    }

    void FileSystem::removePath(const Path& path)
    {
        if (!path.empty())
        {
            m_allowed_paths.erase(path);
            Log::infof("Removed allowed path %s", path);
        }
    }

    bool FileSystem::createDirectory(const Path& src)
    {
        if (!isAccessible(src))
        {
            Log::errorf("Unable to access directory: %s", src);
            return false;
        }

        return std::tr2::sys::create_directory(src);
    }

    bool FileSystem::copy(const Path& src, const Path& dest)
    {
        if (!isAccessible(src))
        {
            Log::errorf("Unable to access file: %s", src);
            return false;
        }

        if (!isAccessible(dest))
        {
            Log::errorf("Unable to access file: %s", dest);
            return false;
        }

        try
        {
            std::tr2::sys::copy_file(src, dest);
            return true;
        }
        catch (std::tr2::sys::basic_filesystem_error<Path> e)
        {
            return false;
        }
    }

    bool FileSystem::rename(const Path& src, const Path& new_name)
    {
        if (!isAccessible(src))
        {
            Log::errorf("Unable to access file: %s", src);
            return false;
        }

        return std::tr2::sys::rename(src, new_name);
    }

    bool FileSystem::remove(const Path& file)
    {
        if (!isAccessible(file))
        {
            Log::errorf("Unable to access file: %s", file);
            return false;
        }

        return std::tr2::sys::remove_all(file) > 0;
    }

    bool FileSystem::isAccessible(const Path& path) const
    {
        if (path.empty())
            return false;

        Path full_path;
        if (!path.is_complete())
            full_path = getCurrentDir() /= path;
        else
            full_path = path;

        if (m_allowed_paths.empty())
            return true;

        for (auto i : m_allowed_paths)
        {
            if (i == full_path)
                return true;
            
            bool allowed = true;
            for (auto ie = i.begin(), pe = full_path.begin(); ie != i.end() && pe != full_path.end(); ie++, pe++)
            {              
                if (*pe != ".." && (*ie) == (*pe))
                    continue;

                allowed = false;
                break;
            }

            if (allowed)
                return true;
        }

        return false;
    }

    bool FileSystem::getExists(const Path& path) const
    {
        if (!isAccessible(path))
        {
            Log::warnf("Unable to access file: %s", path);
            return false;
        }

        return std::tr2::sys::exists(path);
    }

    bool FileSystem::hasRestrictedPaths() const
    {
        return m_allowed_paths.size() > 0;
    }

    void FileSystem::scanDir(std::vector<Path>& output, const Path& path, std::string filter, glm::uint flags, bool recusive)
    {
        if (!isAccessible(path))
        {
            Log::warnf("Unable to access path: %s", path);
            return;
        }

        std::tr2::sys::recursive_directory_iterator iter(path);

        if (!recusive)
            iter.no_push();

        std::tr2::sys::recursive_directory_iterator prev;
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

            if (is_directory(iter->status()) && (flags & SCAN_DIRS) != 0)
            {
                output.push_back(entry_path);
            }
            else if (is_regular_file(iter->status()) && (flags & SCAN_FILES) != 0)
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
        return std::tr2::sys::current_path<Path>();
    }

    Path FileSystem::getProgramDir() const
    {
        return std::tr2::sys::initial_path<Path>();
    }

    Path FileSystem::getDocumentsDir() const
    {
        char path[MAX_PATH];
        SHGetSpecialFolderPath(nullptr, path, CSIDL_PERSONAL, FALSE);

        Path out(path);
        out /= "Games";
        out /= ERIS_APP;

        return out;
    }

    Path FileSystem::getApplicationPreferencesDir() const
    {
        char path[MAX_PATH];
        SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, FALSE);

        Path out(path);
        out /= ERIS_ORG;
        out /= ERIS_APP;

        return out;
    }

}