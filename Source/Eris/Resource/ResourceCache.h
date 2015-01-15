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

#include "Events.h"
#include "Resource.h"
#include "ResourceLoader.h"

#include "Core/Context.h"
#include "Core/Object.h"
#include "Core/Log.h"
#include "Collections/StringHash.h"
#include "Memory/Pointers.h"
#include "IO/File.h"

#include <typeinfo>
#include <typeindex>

namespace Eris
{
    static const glm::uint PRIORITY_LAST = std::numeric_limits<glm::uint>::max();

    struct ResourceGroup
    {
        std::unordered_map<Path, SharedPtr<Resource>> m_resources;
    };

    class ResourceCache : public Object
    {
        friend class BackgroundLoader;

    public:
        ResourceCache(Context* context);
        virtual ~ResourceCache();

        void initialize();
        void terminate();

        bool addDirectory(const Path& path, glm::uint priority = PRIORITY_LAST);
        bool removeDirectory(const Path& path);

        template<typename T, typename Base = T> T* getResource(const Path& path, bool error_on_fail = true);
        template<typename T, typename Base = T> T* getTempResource(const Path& path, bool error_on_fail = false);
        template<typename T, typename Base = T> void loadResource(const Path& path, bool immediate = true, bool error_on_fail = true);

        void releaseResource(std::type_index type, const Path& path, bool force = false);
        void releaseResources(std::type_index type, bool force = false);
        void releaseResources(bool force = false);

    private:
        Resource* findResource(std::type_index type, const Path& path);
        Path findFile(const Path& name);
        bool _loadResource(Resource* res, const Path& path, bool immediate = true);

        bool m_initialized;
        std::unordered_map<std::type_index, ResourceGroup> m_groups;
        std::vector<Path> m_directories;
        SharedPtr<ResourceLoader> m_loader;
        std::mutex m_resource_mutex;
    };

    template<typename T, typename Base>
    inline T* ResourceCache::getResource(const Path& path, bool error_on_fail /*= true*/)
    {
        std::type_index type(typeid(Base));

        Resource* resource = findResource(type, path);
        if (resource && resource->getAsyncState() == AsyncState::SUCCESS)
            return static_cast<T*>(resource);
        else if (resource && resource->getAsyncState() == AsyncState::LOADING)
        {
            while (resource->getAsyncState() == AsyncState::LOADING);
            if (resource->getAsyncState() == AsyncState::SUCCESS)
                return static_cast<T*>(resource);
        }

        loadResource<T, Base>(path, true, false);

        resource = findResource(type, path);
        if (resource)
            return static_cast<T*>(resource);

        if (error_on_fail)
        {
            ResourceLoadingFailed* event = m_context->getFrameAllocator().newInstance<ResourceLoadingFailed>();
            event->resource = path;
            event->type = typeid(T).name();
            sendEvent(ResourceLoadingFailed::getTypeStatic(), event);
        }

        return nullptr;
    }

    template<typename T, typename Base>
    inline T* ResourceCache::getTempResource(const Path& path, bool error_on_fail)
    {
        std::type_index type(typeid(Base));

        Path final_path = path;
        if (path.is_complete())
        {
            for (auto dir : m_directories)
            {
                Path new_path = "";
                std::size_t start_pos = path.string().find(dir.string());
                if (start_pos != std::string::npos)
                {
                    new_path = path.string().replace(start_pos, dir.string().length() + 1, "");
                    if (new_path.string().length() < final_path.string().length())
                        final_path = new_path;
                }
            }
        }

        Resource* resource = findResource(type, final_path);
        if (resource && resource->getAsyncState() == AsyncState::SUCCESS)
            return static_cast<T*>(resource);
        else if (resource && resource->getAsyncState() == AsyncState::LOADING)
        {
            while (resource->getAsyncState() == AsyncState::LOADING);
            if (resource->getAsyncState() == AsyncState::SUCCESS)
                return static_cast<T*>(resource);
        }

        resource = new T(m_context);
        resource->setName(path);

        Path full_path = findFile(final_path);
        if (!full_path.empty() && _loadResource(resource, full_path))
            return static_cast<T*>(resource);

        delete resource;

        return nullptr;
    }

    template<typename T, typename Base> 
    inline void ResourceCache::loadResource(const Path& path, bool immediate, bool error_on_fail)
    {
        Path full_path = findFile(path);
        if (!path.empty())
        {
            Base* resource = new T(m_context);
            resource->setName(path);

            {
                std::lock_guard<std::mutex> lock(m_resource_mutex);
                m_groups[std::type_index(typeid(Base))].m_resources[path] = SharedPtr<Resource>(resource);
            }

            if (_loadResource(resource, full_path, immediate))
                return;
        }

        if (error_on_fail)
        {
            ResourceLoadingFailed* event = m_context->getFrameAllocator().newInstance<ResourceLoadingFailed>();
            event->resource = path;
            event->type = typeid(T).name();
            sendEvent(ResourceLoadingFailed::getTypeStatic(), event);
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
