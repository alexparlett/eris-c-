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

namespace Eris
{
    // Forward decls
    class Object;
    class Event;
    class Context

    /// Event Handler
    class EventHandler
    {
        typedef std::function<void(const Event*)> HandlerFunctionPtr;

    public:
        EventHandler(HandlerFunctionPtr function) :
            sender_(nullptr),
            function_(function),
            userData_(nullptr)
        {
        }

        EventHandler(HandlerFunctionPtr function, void* userData) :
            sender_(nullptr),
            function_(function),
            userData_(userData)
        {
        }

        virtual ~EventHandler() {}

        void setSender(Object* sender) { sender_ = sender; }
        void setEventType(const std::string& eventType) { eventType_ = eventType; }

        Object* getSender() const { return sender_; }
        const std::string& getEventType() const { return eventType_; }

        void invoke(const Event* eventData) { function_(eventData); }

    private:
        Object* sender_;
        std::string eventType_;
        HandlerFunctionPtr function_;
        void* userData_;
    };

    /// Object
    class Object
    {
        friend class Context;

    public:
        Object(Context* context);
        virtual ~Object();

        void subscribeToEvent(const std::string& eventType, EventHandler* handler);
        void subscribeToEvent(Object* sender, const std::string& eventType, EventHandler* handler);

        void unsubscribeFromEvent(const std::string& eventType);
        void unsubscribeFromEvent(Object* sender, const std::string& eventType);
        void unsubscribeFromEvents(Object* sender);
        void unsubscribeFromEvents();

        void sendEvent(const std::string& eventType);
        void sendEvent(const std::string& eventType, Event* eventData);

        static void sendStaticEvent(const std::string& eventType);
        static void sendStaticEvent(const std::string& eventType, Event* eventData);

        Context* getContext() { return context_; }

    protected:
        virtual void onEvent(Object* sender, const std::string& eventType, Event* eventData);

        Context* context_;

    private:
        void removeEventSender(Object* sender);

        EventHandler* findEventHandler(const std::string& eventType, EventHandler** previous = nullptr) const;
        EventHandler* findSpecificEventHandler(Object* sender, EventHandler** previous = nullptr) const;
        EventHandler* findSpecificEventHandler(Object* sender, const std::string& eventType, EventHandler** previous = nullptr) const;

        std::list<EventHandler*> eventHandlers_;
    };

#define HANDLER(className, function) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2)))
#define HANDLER_USERDATA(className, function, userData) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2), userData))
}

namespace std
{
    template<>
    struct hash<Eris::Object>
    {

    };
}