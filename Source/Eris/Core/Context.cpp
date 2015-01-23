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

#include "Context.h"
#include "Object.h"

// Include all module definitions
#include "Clock.h"
#include "Log.h"
#include "Profiler.h"
#include "Application/Engine.h"
#include "Application/Locale.h"
#include "Application/Settings.h"
#include "Graphics/Graphics.h"
#include "Graphics/Renderer.h"
#include "Input/Input.h"
#include "IO/FileSystem.h"
#include "Resource/ResourceCache.h"

namespace Eris
{
    Context::Context() :
        m_frame_allocator(0, 4096),
        m_clock(nullptr),
        m_engine(nullptr),
        m_graphics(nullptr),
        m_input(nullptr),
        m_locale(nullptr),
        m_log(nullptr),
        m_fs(nullptr),
        m_cache(nullptr),
        m_settings(nullptr),
        m_renderer(nullptr),
        m_profiler(nullptr)
    {
    }

    void Context::addEventReciever(Object* reciever, const StringHash& event_type, Object* sender)
    {
        if (sender)
            m_specific_recievers[sender][event_type].insert(reciever);
        else
            m_recievers[event_type].insert(reciever);
    }

    void Context::removeEventSender(Object* sender)
    {
        auto iter = m_specific_recievers.find(sender);
        if (iter != m_specific_recievers.end())
        {
            for (auto i : iter->second)
            {
                for (auto j : i.second)
                    (*j).removeEventSender(sender);
            }
            m_specific_recievers.erase(iter);
        }
    }

    void Context::removeEventReciever(Object* reciever, const StringHash& event_type, Object* sender)
    {
        std::unordered_set<Object*>* group = _getEventRecievers(event_type, sender);
        if (group)
            group->erase(reciever);
    }

    const std::unordered_set<Object*>* Context::getEventRecievers(const StringHash& event_type, Object* sender)
    {
        return _getEventRecievers(event_type, sender);
    }

    void Context::resetFrameAllocator()
    {
        m_frame_allocator.getMemoryPool().reset();
    }

    std::unordered_set<Object*>* Context::_getEventRecievers(const StringHash& event_type, Object* sender /*= nullptr*/)
    {
        if (sender)
        {
            auto sm = m_specific_recievers.find(sender);
            if (sm != m_specific_recievers.end())
            {
                auto iter = sm->second.find(event_type);
                if (iter != m_recievers.end())
                    return &iter->second;
            }
        }
        else
        {
            auto iter = m_recievers.find(event_type);
            if (iter != m_recievers.end())
                return &iter->second;
        }

        return nullptr;
    }
}