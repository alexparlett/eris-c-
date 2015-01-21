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

#include "Profiler.h"

namespace Eris
{
    AutoProfilerBlock::AutoProfilerBlock(Profiler* profiler, const std::string& name) :
        m_profiler(profiler)
    {
        profiler->beginBlock(name);
    }

    AutoProfilerBlock::~AutoProfilerBlock()
    {
        m_profiler->endBlock();
    }

    ProfilerBlock::ProfilerBlock(ProfilerBlock* parent, const std::string& name) :
        m_name(name),
        m_parent(parent),
        m_count(0),
        m_total_time(0),
        m_max_time(0)
    {
    }

    void ProfilerBlock::begin()
    {
        m_timer.reset();
        m_count++;
    }
    
    void ProfilerBlock::end()
    {
        glm::f64 elapsed = m_timer.elapsed();
        if (elapsed > m_max_time)
            m_max_time = elapsed;
        m_total_time += elapsed;
    }

    ProfilerBlock* ProfilerBlock::getChild(const std::string& name)
    {
        auto find = m_children.find(name);
        if (find != m_children.end())
            return find->second;

        SharedPtr<ProfilerBlock> child(new ProfilerBlock(this, name));
        m_children[name] = child;   
        return child;
    }

    ProfilerThreadBlock::ProfilerThreadBlock() :
        m_root(new ProfilerBlock(nullptr, "Root")),
        m_current(m_root)
    {
    }

    void ProfilerThreadBlock::beginBlock(const std::string& name)
    {
        m_current = m_current->getChild(name);
        m_current->begin();
    }

    void ProfilerThreadBlock::endBlock()
    {
        if (m_current != m_root)
        {
            m_current->end();
            m_current = m_current->getParent();
        }
    }

    Profiler::Profiler(Context* context) :
        Object(context)
    {
    }

    void Profiler::beginBlock(const std::string& name)
    {
        auto thread_block = getThreadBlock(std::this_thread::get_id());
        if (thread_block)
            thread_block->beginBlock(name);

        SharedPtr<ProfilerThreadBlock> new_thread_block(new ProfilerThreadBlock());
        m_thread_blocks[std::this_thread::get_id()] = new_thread_block;
        new_thread_block->beginBlock(name);
    }

    void Profiler::endBlock()
    {
        auto thread_block = getThreadBlock(std::this_thread::get_id());
        if (thread_block)
            thread_block->endBlock();
    }

    ProfilerThreadBlock* Profiler::getThreadBlock(std::thread::id thread_id) const
    {
        auto find = m_thread_blocks.find(thread_id);
        if (find != m_thread_blocks.end())
            return find->second;
        return nullptr;
    }
}