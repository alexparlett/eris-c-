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

#include <functional>
#include <list>

#include "Collections/StringHash.h"
#include "Memory/RefCounted.h"
#include "Memory/Pointers.h"

namespace Eris
{
    class Object;
    class Event;

    class EventHandler : public RefCounted
    {
        using HandlerFunctionPtr = std::function<void(const StringHash&, const Event*)>;

    public:
        EventHandler(HandlerFunctionPtr function) :
            m_sender(0),
            m_function(function),
            m_user_data(0)
        {
        }

        EventHandler(HandlerFunctionPtr function, void* user_data) :
            m_sender(0),
            m_function(function),
            m_user_data(user_data)
        {
        }

        virtual ~EventHandler() {}

        void setSender(Object* sender) { m_sender = sender; }
        void setEventType(StringHash event_type) { m_event_type = event_type; }

        Object* getSender() const { return m_sender; }
        const StringHash& getEventType() const { return m_event_type; }

        void invoke(const Event* event) { m_function(m_event_type, event); }

    private:
        Object* m_sender;
        StringHash m_event_type;
        HandlerFunctionPtr m_function;
        void* m_user_data;
    };

	class Object : public RefCounted
	{
        friend class Context;

        using Iterator = std::list<SharedPtr<EventHandler>>::iterator;

	public:
	    Object(Context* context);
	    virtual ~Object();

        void subscribeToEvent(const StringHash& event_type, EventHandler* handler, Object* sender = nullptr);

        void unsubscribeFromEvent(const StringHash& event_type);
        void unsubscribeFromEvent(const StringHash& event_type, Object* sender);
        void unsubscribeFromEvents(Object* sender);
        void unsubscribeFromEvents();

        void sendEvent(const StringHash& event_type, Event* event = nullptr);

        Context* getContext() const { return m_context; }

    protected:
        virtual void onEvent(const StringHash& event_type, const Event* event = nullptr, Object* sender = nullptr);

        Context* m_context;

    private:
        void removeEventSender(Object* sender);

        Iterator findEventHandler(const StringHash& event_type);
        Iterator findEventHandler(const StringHash& event_type, Object* sender);
        Iterator findEventHandler(Object* sender);

        std::list<SharedPtr<EventHandler>> m_handlers;
	};

#define HANDLER(className, function) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2)))
#define HANDLER_USERDATA(className, function, userData) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2), userData))

}
