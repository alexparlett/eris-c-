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

#include "Renderer.h"
#include "RenderQueue.h"

#include "Core/Profiler.h"
#include "Thread/SpinLock.h"

namespace Eris
{
    struct RenderQueueSorter
    {
        bool operator () (SharedPtr<RenderCommand>& lhs, SharedPtr<RenderCommand>& rhs)
        {
            return lhs->key.key < rhs->key.key;
        }
    };

    RenderQueue::RenderQueue(Context* context) :
        Object(context)
    {
    }

    void RenderQueue::add(RenderCommand* item)
    {
        std::lock_guard<SpinLock> lock(m_lock);
        if (!item->key.key)
            item->key();

        m_commands.push_back(SharedPtr<RenderCommand>(item));
    }

    void RenderQueue::sort()
    {
        PROFILE(SortQueue);
        std::lock_guard<SpinLock> lock(m_lock);
        std::sort(m_commands.begin(), m_commands.end(), RenderQueueSorter());
    }

    void RenderQueue::process()
    {
        PROFILE(ProcessQueue);
        std::lock_guard<SpinLock> lock(m_lock);
        if (m_commands.empty())
            return;

        Renderer* renderer = m_context->getModule<Renderer>();
        RenderCommand* last = m_commands[0];
        (*last)(renderer, nullptr);

        for (auto i = 1U; i < m_commands.size(); i++)
        {
            RenderCommand* current = m_commands[i];
            (*current)(renderer, &last->key);
            last = current;
        }
    }

    void RenderQueue::clear()
    {
        std::lock_guard<SpinLock> lock(m_lock);
        m_commands.clear();
    }

}