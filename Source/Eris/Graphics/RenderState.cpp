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

#include "RenderState.h"

namespace Eris
{


    RenderState::RenderState() :
        RefCounted(),
        m_render_queue(3),
        m_update_queue(0)
    {
        for (auto queue : m_queue)
            queue = new RenderQueue();
    }

    RenderState::~RenderState()
    {
        for (auto queue : m_queue)
        {
            queue.reset();
            queue = nullptr;
        }
    }

    void RenderState::swap()
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);

        m_render_queue = m_update_queue;
        if (++m_update_queue == 3)
            m_update_queue = 0;

        m_queue[m_update_queue]->clear();
    }

    RenderQueue* RenderState::getRenderable() const
    {
        std::lock_guard<const std::mutex> lock(m_queue_mutex);
        return m_queue[m_render_queue];
    }

    RenderQueue* RenderState::getUpdatable() const
    {
        std::lock_guard<const std::mutex> lock(m_queue_mutex);
        return m_queue[m_update_queue];
    }
}