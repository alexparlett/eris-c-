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

#include "ResourceCache.h"

#include "IO/File.h"
#include "IO/FileSystem.h"

namespace Eris
{


    BackgroundLoader::BackgroundLoader(Context* context) :
        Object(context),
        m_thread(&BackgroundLoader::run, this)
    {
    }

    void BackgroundLoader::add(const Path& path, Resource* res)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        BackgroundTask task;
        task.m_path = path;
        task.m_resource = res;
        m_waiting.push(task);

        m_conditional.notify_all();
    }

    void BackgroundLoader::run()
    {
        while (m_control)
        {
            if (poll())
                load();
            else
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_conditional.wait(lock);
            }
        }
    }

    void BackgroundLoader::stop()
    {
        m_control = false;
        m_conditional.notify_all();
        m_thread.join();
    }

    bool BackgroundLoader::poll()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_waiting.size() > 0)
        {
            m_current = m_waiting.front();
            m_waiting.pop();
            return true;
        }

        return false;
    }

    void BackgroundLoader::load()
    {
        if (m_current.m_resource && !m_current.m_path.empty())
        {
            m_current.m_resource->setAsyncState(AsyncState::LOADING);

            SharedPtr<File> file(new File(m_context, m_current.m_path));
            if (file && file->isOpen())
            {
                if (m_current.m_resource->load(*file))
                    m_current.m_resource->setAsyncState(AsyncState::SUCCESS);
                else
                    m_current.m_resource->setAsyncState(AsyncState::FAILED);
            }
            else
            {
                m_current.m_resource->setAsyncState(AsyncState::FAILED);
            }
        }
    }

    ResourceCache::ResourceCache(Context* context) :
        Object(context),
        m_loader(new BackgroundLoader(context))
    {

    }

    ResourceCache::~ResourceCache()
    {
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

        return m_directories.insert(m_directories.begin() + priority, path) != m_directories.end();
    }

    bool ResourceCache::removeDirectory(const Path& path)
    {
        for (glm::i32 i = 0; i < m_directories.size(); i++)
        {
            if (m_directories[i] == path)
                return m_directories.erase(m_directories.begin() + i) != m_directories.end();
        }

        return false;
    }

    void ResourceCache::releaseResources(std::type_index type, bool force /*= false*/)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
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
        std::lock_guard<std::mutex> lock(m_mutex);
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
        std::lock_guard<std::mutex> lock(m_mutex);
        auto group = m_groups.find(type);
        if (group != m_groups.end())
        {
            auto resource = group->second.m_resources.find(path);
            if (resource != group->second.m_resources.end() && resource->second->getAsyncState() == AsyncState::SUCCESS)
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

            if (fs->accessible(dir) && fs->exists(path))
                return path;
        }

        return Path();
    }
}