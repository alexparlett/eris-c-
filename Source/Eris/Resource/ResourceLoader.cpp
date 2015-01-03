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

#include "ResourceLoader.h"

#include "IO/File.h"

namespace Eris
{
    void ResourceThread::run()
    {
        while (m_loader->m_thread_control)
        {
            if (poll())
                load();
            else
            {
                std::unique_lock<std::mutex> lock(m_loader->m_queue_mutex);
                m_loader->m_queue_conditional.wait(lock);
            }
        }
    }

    bool ResourceThread::poll()
    {
        std::lock_guard<std::mutex> lock(m_loader->m_queue_mutex);
        while (m_loader->m_waiting_tasks.size() > 0)
        {
            m_task = m_loader->m_waiting_tasks.front();
            m_loader->m_waiting_tasks.pop();

            if (m_task->m_resource->getAsyncState() != AsyncState::QUEUED)
                continue;

            return true;
        }

        return false;
    }

    void ResourceThread::load()
    {
        if (m_task->m_resource && !m_task->m_path.empty())
        {
            m_task->m_resource->setAsyncState(AsyncState::LOADING);

            SharedPtr<File> file(new File(m_loader->m_context, m_task->m_path));
            if (file && file->isOpen())
            {
                if (m_task->m_resource->load(*file))
                    m_task->m_resource->setAsyncState(AsyncState::SUCCESS);
                else
                    m_task->m_resource->setAsyncState(AsyncState::FAILED);
            }
            else
            {
                m_task->m_resource->setAsyncState(AsyncState::FAILED);
            }
        }
    }

    ResourceLoader::ResourceLoader(Context* context) : 
        Object(context),
        m_threads(0)
    {
        for (glm::uint i = 0; i < std::thread::hardware_concurrency(); i++)
            m_threads.push_back(SharedPtr<ResourceThread>(new ResourceThread(this)));
    }

    ResourceLoader::~ResourceLoader()
    {
        stop();
    }

    void ResourceLoader::add(const Path& path, Resource* res)
    {
        if (res->getAsyncState() == AsyncState::QUEUED || res->getAsyncState() == AsyncState::LOADING)
            return;

        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_waiting_tasks.push(SharedPtr<ResourceTask>(new ResourceTask(path, res)));

        m_queue_conditional.notify_all();
    }

    void ResourceLoader::stop()
    {
        m_thread_control = false;
        m_queue_conditional.notify_all();
        for (glm::uint i = 0; i < m_threads.size(); i++)
            m_threads[i]->m_thread.join();
    }
}