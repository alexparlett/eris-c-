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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Collections/StringHash.h"
#include "Memory/Pointers.h"

#include <typeinfo>
#include <typeindex>

namespace Eris
{
    static const glm::uint PRIORITY_LAST = 0xffffffff;

    struct BackgroundTask
    {
        Path m_path;
        SharedPtr<Resource> m_resource;
    };

    class BackgroundLoader : public Object
    {
    public:
        BackgroundLoader(Context* context);

        void add(const Path& path, Resource* res);
        void run();
        void stop();

    private:
        bool poll();
        void load();

        BackgroundTask m_current;
        std::queue<BackgroundTask> m_waiting;
        std::mutex m_mutex;
        std::thread m_thread;
        std::condition_variable m_conditional;
        std::atomic<bool> m_control;
    };

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

        bool addDirectory(const Path& path, glm::uint priority = PRIORITY_LAST);
        bool removeDirectory(const Path& path);

        template<typename T> T* getResource(const Path& path, bool error_on_fail = true);
        template<typename T> void loadResource(const Path& path, bool immediate = true, bool error_on_fail = true);

        template<typename T> void releaseResource(const Path& path, bool force = false);
        void releaseResources(std::type_index type, bool force = false);
        void releaseResources(bool force = false);

    private:
        Resource* findResource(std::type_index type, const Path& path);
        Path findFile(const Path& name);

        std::unordered_map<std::type_index, ResourceGroup> m_groups;
        std::vector<Path> m_directories;
        SharedPtr<BackgroundLoader> m_loader;
        std::mutex m_mutex;
    };

    template<typename T>
    inline T* ResourceCache::getResource(const Path& path, bool error_on_fail /*= true*/)
    {
        std::type_index type(typeid(T));

        T* resource = static_cast<T*>(findResource(type, path));
        if (resource)
            return resource;

        loadResource(path, true, error_on_fail);

        resource = static_cast<T*>(findResource(type, path));
        if (resource)
            return resource;

        if (error_on_fail)
        {
            //todo: Send Error Event
        }

        return nullptr;
    }

    template<typename T>
    inline void ResourceCache::loadResource(const Path& path, bool immediate /*= true*/, bool error_on_fail /*= true*/)
    {
        SharedPtr<T> resource = SharedPtr<T>(new T(m_context));
        resource->setName(path);
        
        Path full_path = findFile(path);
        if (!path.empty())
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_groups[std::type_index(typeid(T))].m_resources[path] = resource;
            }

            if (!immediate)
            {
                resource->setAsyncState(AsyncState::QUEUED);
                m_loader->add(full_path, resource);
                return;
            }
            else
            {
                resource->setAsyncState(AsyncState::LOADING);

                SharedPtr<File> file(new File(m_context, full_path));
                if (file && file->isOpen())
                {
                    if (resource->load(*file))
                    {
                        resource->setAsyncState(AsyncState::SUCCESS);
                        return;
                    }
                    else
                        resource->setAsyncState(AsyncState::FAILED);
                }
                else
                    resource->setAsyncState(AsyncState::FAILED);
            }
        }

        if (error_on_fail)
        {
            //todo: Error on load
        }
    }

    template<typename T>
    inline void ResourceCache::releaseResource(const Path& path, bool force)
    {
        std::type_index type(typeid(T));
        Resource* res = findResource(type, path);
        if (!res)
            return;

        std::lock_guard<std::mutex> lock(m_mutex);
        if ((res->refs() == 1 && res->weakRefs() == 0) || force)
            m_groups[type].m_resources.erase(path);
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
