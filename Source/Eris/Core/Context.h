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

#include "Event.h"

#include "Collections/StringHash.h"
#include "Memory/RefCounted.h"
#include "Memory/Pointers.h"
#include "Memory/Allocator.h"
#include "Util/NonCopyable.h"

#include <unordered_map>
#include <unordered_set>

namespace Eris
{
    class Clock;
    class Engine;
    class FileSystem;
    class Graphics;
    class Input;
    class Locale;
    class Log;
    class ResourceCache;
    class Settings;

    class Context : public RefCounted, public NonCopyable
    {
        friend class Object;

    public:
        Context();

        template<typename T> void registerModule(T* module) {}
        template<typename T> T* getModule() { return nullptr; }

        void addEventReciever(Object* reciever, const StringHash& event_type, Object* sender);
        void removeEventSender(Object* sender);
        void removeEventReciever(Object* reciever, const StringHash& event_type, Object* sender = nullptr);
        const std::unordered_set<Object*>* getEventRecievers(const StringHash& event_type, Object* sender = nullptr);

        StackAllocator<glm::u8>& getFrameAllocator() { return m_frame_allocator; }
        void resetFrameAllocator();

    private:
        std::unordered_set<Object*>* _getEventRecievers(const StringHash& event_type, Object* sender = nullptr);

        std::unordered_map<StringHash, std::unordered_set<Object*>> m_recievers;
        std::unordered_map<Object*, std::unordered_map<StringHash, std::unordered_set<Object*>>> m_specific_recievers;

        StackAllocator<glm::u8> m_frame_allocator;

        SharedPtr<Clock> m_clock;
        SharedPtr<Engine> m_engine;
        SharedPtr<FileSystem> m_fs;
        SharedPtr<Graphics> m_graphics;
        SharedPtr<Input> m_input;
        SharedPtr<Locale> m_locale;
        SharedPtr<Log> m_log;
        SharedPtr<ResourceCache> m_cache;
        SharedPtr<Settings> m_settings;
    };
}