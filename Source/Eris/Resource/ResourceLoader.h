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

#include "Resource.h"

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/Pointers.h"
#include "Util/NonCopyable.h"

namespace Eris
{
    struct ResourceTask
    {
        ResourceTask(const Path& path, Resource* res) :
            m_path(path),
            m_resource(res)
        {
        }

        Path m_path;
        SharedPtr<Resource> m_resource;
    };

    class ResourceLoader : public Object
    {
    public:
        ResourceLoader(Context* context);

        void start();
        void add(const Path& path, Resource* res);
        void stop();

    private:
        void run();
        ResourceTask* poll();
        void load(ResourceTask* task);

        std::queue<ResourceTask*> m_waiting_tasks;
        std::mutex m_queue_mutex;
        std::thread m_thread;
        std::condition_variable m_queue_conditional;
        std::atomic<bool> m_thread_exit;
    };
}
