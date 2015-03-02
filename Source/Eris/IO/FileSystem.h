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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/Allocator.h"

namespace Eris
{
    static const glm::uint SCAN_FILES = 1;
    static const glm::uint SCAN_DIRS = 2;
    static const glm::uint SCAN_HIDDEN = 4;

    class FileSystem : public Object
    {
    public:
        FileSystem(Context* context);

        void addPath(const Path& path);
        void removePath(const Path& path);

        bool createDirectory(const Path& src);
        bool copy(const Path& src, const Path& dest);
        bool rename(const Path& src, const Path& new_name);
        bool remove(const Path& file);

        bool isAccessible(const Path& path) const;
        bool getExists(const Path& path) const;

        bool hasRestrictedPaths() const;

        void scanDir(std::vector<Path>& output, const Path& path, std::string filter = StringEmpty, glm::uint flags = SCAN_FILES, bool recusive = false);

        Path getCurrentDir() const;
        Path getProgramDir() const;
        Path getDocumentsDir() const;
        Path getApplicationPreferencesDir() const;

    private:
        std::unordered_set<Path, std::hash<Path>, std::equal_to<Path>, ChainAllocator<Path>> m_allowed_paths;
    };

    template<> inline void Context::registerModule(FileSystem* module)
    {
        m_fs = SharedPtr<FileSystem>(module);
    }

    template<> inline FileSystem* Context::getModule()
    {
        ERIS_ASSERT(m_fs);
        return m_fs.get();
    }
}