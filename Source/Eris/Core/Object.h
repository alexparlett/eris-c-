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

#include "LinkedList.h"
#include "Ptr.h"
#include "RefCounted.h"
#include "Variant.h"
#include "StringHash.h"

#include <functional>

namespace Eris
{
    class Object;

    /// Event Handler
    class EventHandler : public LinkedListNode
    {
        typedef std::function<void(const VariantMap&, void*)> HandlerFunctionPtr;

    public:
        EventHandler(HandlerFunctionPtr function) :
            sender_(0),
            function_(function),
            userData_(0)
        {
        }

        EventHandler(HandlerFunctionPtr function, void* userData) :
            sender_(0),
            function_(function),
            userData_(userData)
        {
        }

        virtual ~EventHandler() {}

        void SetSender(Object* sender) { sender_ = sender; }
        void SetEventType(StringHash eventType) { eventType_ = eventType; }

        Object* GetSender() const { return sender_; }
        const StringHash& GetEventType() const { return eventType_; }

        void Invoke(const VariantMap& eventData) { function_(eventData, userData_); }

    private:
        Object* sender_;
        StringHash eventType_;
        HandlerFunctionPtr function_;
        void* userData_;
    };

#define OBJECT(typeName) \
public: \
    virtual const Eris::StringHash& GetType() const { return GetTypeStatic(); } \
    virtual const Eris::StringHash& GetBaseType() const { return GetBaseTypeStatic(); } \
    virtual const Eris::String& GetTypeName() const { return GetTypeNameStatic(); } \
    static const Eris::StringHash& GetTypeStatic() { static const Eris::StringHash typeStatic(#typeName); return typeStatic; } \
    static const Eris::String& GetTypeNameStatic() { static const Eris::String typeNameStatic(#typeName); return typeNameStatic; } \

#define BASEOBJECT(typeName) \
public: \
    static const Eris::StringHash& GetBaseTypeStatic() { static const Eris::StringHash baseTypeStatic(#typeName); return baseTypeStatic; } \

    /// Object
    class Object : public RefCounted
    {
        friend class Context;

        BASEOBJECT(Object)

    public:
        Object(Context* context);
        virtual ~Object();

        void SubscribeToEvent(StringHash eventType, EventHandler* handler);
        void SubscribeToEvent(Object* sender, StringHash eventType, EventHandler* handler);

        void UnsubscribeFromEvent(StringHash eventType);
        void UnsubscribeFromEvent(Object* sender, StringHash eventType);
        void UnsubscribeFromEvents(Object* sender);
        void UnsubscribeFromEvents();

        void SendEvent(StringHash eventType);
        void SendEvent(StringHash eventType, const VariantMap& eventData);

        virtual const StringHash& GetType() const = 0;
        virtual const StringHash& GetBaseType() const = 0;
        virtual const String& GetTypeName() const = 0;
        Context* GetContext() { return context_; }

    protected:
        virtual void OnEvent(Object* sender, StringHash eventType, const VariantMap& eventData);

        Context* context_;

    private:
        void RemoveEventSender(Object* sender);

        EventHandler* FindEventHandler(StringHash eventType, EventHandler** previous = 0) const;
        EventHandler* FindSpecificEventHandler(Object* sender, EventHandler** previous = 0) const;
        EventHandler* FindSpecificEventHandler(Object* sender, StringHash eventType, EventHandler** previous = 0) const;

        LinkedList<EventHandler> eventHandlers_;
    };

#define EVENT(eventID, eventName) static const Eris::StringHash eventID(#eventName); namespace eventName
#define PARAM(paramID, paramName) static const Eris::StringHash paramID(#paramName)
#define HANDLER(className, function) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2)))
#define HANDLER_USERDATA(className, function, userData) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2), userData))
}