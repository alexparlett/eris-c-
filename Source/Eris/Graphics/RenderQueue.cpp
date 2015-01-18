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

#include "RenderQueue.h"

namespace Eris
{
    struct RenderQueueSorter
    {
        bool operator () (SharedPtr<RenderQueueItem>& lhs, SharedPtr<RenderQueueItem>& rhs)
        {

        }
    };

    RenderQueue::RenderQueue() :
        RefCounted()
    {
    }

    void RenderQueue::add(RenderQueueItem* item)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue_items.push_back(SharedPtr<RenderQueueItem>(item));
    }

    void RenderQueue::sort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue_items.sort(RenderQueueSorter());
    }

    void RenderQueue::process()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
    }

    void RenderQueue::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue_items.clear();
    }

}