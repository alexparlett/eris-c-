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

#include "ResourceLoader.h"

#include "IO/File.h"
#include "Core/Log.h"

namespace Eris
{
    ResourceLoader::ResourceLoader(Context* context) : 
        Object(context),
        m_thread_exit(false)
    {
    }

    void ResourceLoader::start()
    {
        for (glm::uint i = 0; i < std::thread::hardware_concurrency(); i++)
            m_threads.push_back(std::thread(&ResourceLoader::run, this));
    }

    void ResourceLoader::add(const Path& path, Resource* res)
    {

        if (res->getAsyncState() == AsyncState::QUEUED || res->getAsyncState() == AsyncState::LOADING)
            return;

        res->setAsyncState(AsyncState::QUEUED);

        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_waiting_tasks.push(new ResourceTask(path, res));

        m_queue_conditional.notify_all();
    }

    void ResourceLoader::stop()
    {
        m_thread_exit = true;
        m_queue_conditional.notify_all();

        for (glm::uint i = 0; i < m_threads.size(); i++)
        {
            if (m_threads[i].joinable())
                m_threads[i].join();
        }
    }

    void ResourceLoader::run()
    {
        Log::infof("Resource Thread started: %d", std::this_thread::get_id().hash());
        while (!m_thread_exit)
        {
            ResourceTask* task = poll();
            if (task)
                load(task);
            else
            {
                std::unique_lock<std::mutex> lock(m_queue_mutex);
                m_queue_conditional.wait(lock);
            }
        }
        Log::infof("Resource Thread stopped: %d", std::this_thread::get_id().hash());
    }

    ResourceTask* ResourceLoader::poll()
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        while (m_waiting_tasks.size() > 0)
        {
            ResourceTask* task = m_waiting_tasks.front();
            m_waiting_tasks.pop();

            if (task->m_resource->getAsyncState() != AsyncState::QUEUED)
            {
                delete task;
                continue;
            }

            return task;
        }

        return nullptr;
    }

    void ResourceLoader::load(ResourceTask* task)
    {
        if (task->m_resource && !task->m_path.empty())
        {
            task->m_resource->setAsyncState(AsyncState::LOADING);

            SharedPtr<File> file(new File(m_context, task->m_path));
            if (file && file->opened())
            {
                if (task->m_resource->load(*file))
                {
                    task->m_resource->setAsyncState(AsyncState::SUCCESS);
                    Log::infof("Successful loading resource %s", task->m_resource->getName());
                }
                else
                {
                    task->m_resource->setAsyncState(AsyncState::FAILED);
                    Log::errorf("Failed loading resource %s", task->m_resource->getName());
                }
            }
            else
            {
                task->m_resource->setAsyncState(AsyncState::FAILED);
                Log::errorf("Failed loading resource %s", task->m_resource->getName());
            }
        }
        
        delete task;
    }
}