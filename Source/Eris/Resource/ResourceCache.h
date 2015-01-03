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

#include "Resource.h"
#include "ResourceLoader.h"

#include "Core/Context.h"
#include "Core/Object.h"
#include "Collections/StringHash.h"
#include "Memory/Pointers.h"
#include "IO/File.h"

#include <typeinfo>
#include <typeindex>

namespace Eris
{
    static const glm::uint PRIORITY_LAST = std::numeric_limits<glm::uint>::max();

    class ResourceCache : public Object
    {
        friend class BackgroundLoader;

    public:
        ResourceCache(Context* context);
        virtual ~ResourceCache();

        bool addDirectory(const Path& path, glm::uint priority = PRIORITY_LAST);
        bool removeDirectory(const Path& path);

        template<typename T> T* getResource(const Path& path, bool error_on_fail = true);
        template<typename T> void loadResource(const Path& path, bool immediate = true, bool error_on_fail = true);

        void releaseResource(std::type_index type, const Path& path, bool force = false);
        void releaseResources(std::type_index type, bool force = false);
        void releaseResources(bool force = false);

    private:
        Resource* findResource(std::type_index type, const Path& path);
        Path findFile(const Path& name);
        bool _loadResource(Resource* res, const Path& path, bool immediate = true);

        std::unordered_map<std::type_index, std::unordered_map<Path, SharedPtr<Resource>>> m_groups;
        std::vector<Path> m_directories;
        SharedPtr<ResourceLoader> m_loader;
        std::mutex m_resource_mutex;
    };

    template<typename T>
    inline T* ResourceCache::getResource(const Path& path, bool error_on_fail /*= true*/)
    {
        std::type_index type(typeid(T));

        Resource* resource = findResource(type, path);
        if (resource && resource->getAsyncState() == AsyncState::SUCCESS)
            return static_cast<T*>(resource);
        else if (resource && resource->getAsyncState() == AsyncState::LOADING)
        {
            while (resource->getAsyncState() == AsyncState::LOADING);
            if (resource->getAsyncState() == AsyncState::SUCCESS)
                return static_cast<T*>(resource);
        }

        loadResource<T>(path, true, false);

        resource = findResource(type, path);
        if (resource)
            return static_cast<T*>(resource);

        if (error_on_fail)
        {
            //todo: Send Error Event
        }

        return nullptr;
    }

    template<typename T> 
    inline void ResourceCache::loadResource(const Path& path, bool immediate, bool error_on_fail)
    {
        Path full_path = findFile(path);
        if (!path.empty())
        {
            T* resource = new T(m_context);
            resource->setName(path);

            {
                std::lock_guard<std::mutex> lock(m_resource_mutex);
                m_groups[std::type_index(typeid(T))][path] = SharedPtr<Resource>(resource);
            }

            if (_loadResource(resource, full_path, immediate))
                return;
        }

        if (error_on_fail)
        {
            //todo: Send Error Event
        }
    }


    template<> inline void Context::registerModule(ResourceCache* module)
    {
        m_cache = SharedPtr<ResourceCache>(module);
    }

    template<> inline ResourceCache* Context::getModule()
    {
        ERIS_ASSERT(m_cache);
        return m_cache.get();
    }
}
