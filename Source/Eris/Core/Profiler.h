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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Core/Timer.h"
#include "Memory/RefCounted.h"

namespace Eris
{
    class AutoProfilerBlock
    {
    public:
        AutoProfilerBlock(Profiler* profile, const std::string& name);
        ~AutoProfilerBlock();

    private:
        WeakPtr<Profiler> m_profiler;
    };

    class ProfilerBlock : public RefCounted
    {
    public:
        ProfilerBlock(ProfilerBlock* parent, const std::string& name);

        void begin();
        void end();

        ProfilerBlock* getChild(const std::string& name);
        ProfilerBlock* getParent() const { return m_parent; }

    private:
        std::string m_name;
        Timer m_timer;
        glm::u64 m_count;
        glm::f64 m_total_time;
        glm::f64 m_max_time;
        WeakPtr<ProfilerBlock> m_parent;
        std::unordered_map<std::string, SharedPtr<ProfilerBlock>> m_children;
    };

    class ProfilerThreadBlock : public RefCounted
    {
    public:
        ProfilerThreadBlock();

        void beginBlock(const std::string& name);
        void endBlock();

        ProfilerBlock* getRoot() const { return m_root; }
        ProfilerBlock* getCurrent() const { return m_current; }

    private:
        SharedPtr<ProfilerBlock> m_root;
        SharedPtr<ProfilerBlock> m_current;
    };

    class Profiler : public Object
    {
    public:
        Profiler(Context* context);

        void beginBlock(const std::string& name);
        void endBlock();

        ProfilerThreadBlock* getThreadBlock(std::thread::id) const;

    private:
        std::unordered_map<std::thread::id, SharedPtr<ProfilerThreadBlock>> m_thread_blocks;
    };

    template<> inline void Context::registerModule(Profiler* module)
    {
        m_profiler = SharedPtr<Profiler>(module);
    }

    template<> inline Profiler* Context::getModule()
    {
        ERIS_ASSERT(m_profiler);
        return m_profiler.get();
    }
}

#ifdef _DEBUG
#define PROFILE(name) Eris::AutoProfilerBlock block_ ## name (m_context->getModule<Profiler>(), #name)
#else
#define PROFILE(name)
#endif