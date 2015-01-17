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

#include "ResourceCache.h"

#include "IO/File.h"
#include "IO/FileSystem.h"

namespace Eris
{
    ResourceCache::ResourceCache(Context* context) :
        Object(context),
        m_loader(new ResourceLoader(context)),
        m_initialized(false)
    {
    }

    ResourceCache::~ResourceCache()
    {
        if (m_initialized)
            terminate();
    }

    void ResourceCache::initialize()
    {
        if (!m_initialized)
        {
            m_loader->start();
            m_initialized = true;
        }
    }

    void ResourceCache::terminate()
    {
        m_initialized = false;

        if (m_loader)
        {
            m_loader->stop();
            m_loader.reset();
            m_loader = nullptr;
        }

        releaseResources(true);
    }

    bool ResourceCache::addDirectory(const Path& path, glm::uint priority /*= PRIORITY_LAST*/)
    {
        FileSystem* fs = m_context->getModule<FileSystem>();
        if (!fs->accessible(path))
            return false;

        if (priority < m_directories.size())
            m_directories.insert(m_directories.begin() + priority, path);
        else
            m_directories.push_back(path);

        Log::infof("Added resource directory %s", path.string().c_str());

        return true;
    }

    bool ResourceCache::removeDirectory(const Path& path)
    {
        for (glm::i32 i = 0; i < m_directories.size(); i++)
        {
            if (m_directories[i] == path)
            {
                m_directories.erase(m_directories.begin() + i);
                Log::infof("Removed resource directory %s", path.string().c_str());
                return true;
            }
        }

        return false;
    }

    void ResourceCache::releaseResource(std::type_index type, const Path& path, bool force)
    {
        Resource* res = findResource(type, path);
        if (!res)
            return;

        std::lock_guard<std::mutex> lock(m_resource_mutex);
        if ((res->refs() == 1 && res->weakRefs() == 0) || force)
            m_groups[type].m_resources.erase(path);
    }

    void ResourceCache::releaseResources(std::type_index type, bool force /*= false*/)
    {
        std::lock_guard<std::mutex> lock(m_resource_mutex);
        auto group = m_groups.find(type);
        if (group != m_groups.end())
        {
            for (auto res = group->second.m_resources.begin(); res != group->second.m_resources.end();)
            {
                auto current = res++;

                if ((current->second.refs() == 1 && current->second.weakRefs() == 0) || force)
                    group->second.m_resources.erase(current);
            }
        }
    }

    void ResourceCache::releaseResources(bool force /*= false*/)
    {
        std::lock_guard<std::mutex> lock(m_resource_mutex);
        for (auto group : m_groups)
        {
            for (auto res = group.second.m_resources.begin(); res != group.second.m_resources.end();)
            {
                auto current = res++;

                if ((current->second.refs() == 1 && current->second.weakRefs() == 0) || force)
                    group.second.m_resources.erase(current);
            }
        }
    }

    Resource* ResourceCache::findResource(std::type_index type, const Path& path)
    {
        std::lock_guard<std::mutex> lock(m_resource_mutex);
        auto group = m_groups.find(type);
        if (group != m_groups.end())
        {
            auto resource = group->second.m_resources.find(path);
            if (resource != group->second.m_resources.end() && resource->second->getAsyncState() != AsyncState::FAILED)
                return resource->second.get();
        }

        return nullptr;
    }

    Path ResourceCache::findFile(const Path& name)
    {
        if (name.empty())
            return Path();

        FileSystem* fs = m_context->getModule<FileSystem>();
        for (auto dir : m_directories)
        {
            auto path = dir;
            path /= name;

            if (fs->exists(path))
                return path;
        }

        return Path();
    }

    bool ResourceCache::_loadResource(Resource* res, const Path& path, bool immediate /*= true*/)
    {
        ERIS_ASSERT(res);
        ERIS_ASSERT(!path.empty());

        if (!immediate)
        {
            m_loader->add(path, res);
            return true;
        }
        else
        {
            res->setAsyncState(AsyncState::LOADING);

            SharedPtr<File> file(new File(m_context, path));
            if (file && file->opened())
            {
                if (res->load(*file))
                {
                    res->setAsyncState(AsyncState::SUCCESS);
                    Log::infof("Successful loading %s: %s", &typeid(*res).name()[12], res->getName().string().c_str());
                    return true;
                }
                else
                {
                    res->setAsyncState(AsyncState::FAILED);
                    Log::errorf("Failed loading %s: %s", &typeid(*res).name()[12], res->getName().string().c_str());
                }

            }
            else
            {
                res->setAsyncState(AsyncState::FAILED);
                Log::errorf("Failed loading %s: %s", &typeid(*res).name()[12], res->getName().string().c_str());
            }
        }

        return false;
    }

}