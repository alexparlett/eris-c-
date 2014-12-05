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

#include "HashMap.h"
#include "HashSet.h"
#include "Ptr.h"
#include "RefCounted.h"
#include "StringHash.h"
#include "Variant.h"

namespace Eris
{
    class Object;
    class EventHandler;

    class Context : public RefCounted
    {
        friend class Object;

    public:
        Context();
        virtual ~Context();

        void RegisterModule(Object* object);

        template<typename T> void RemoveModule();

        VariantMap& GetEventDataMap();
        Object* GetEventSender() const;
        EventHandler* GetEventHandler() const { return eventHandler_; }
        template<typename T> T* GetModule() const;

    private:
        void AddEventReceiver(Object* reciever, StringHash eventType);
        void AddEventReceiver(Object* reciever, Object* sender, StringHash eventType);

        void RemoveEventReceiver(Object* reciever, StringHash eventType);
        void RemoveEventReceiver(Object* reciever, Object* sender, StringHash eventType);
        void RemoveEventSender(Object* sender);

        HashSet<Object*>* GetEventReceivers(StringHash eventType);
        HashSet<Object*>* GetEventReceivers(Object* sender, StringHash eventType);
       
        void SetEventHandler(EventHandler* handler) { eventHandler_ = handler; }
        void BeginSendEvent(Object* sender) { eventSenders_.Push(sender); }
        void EndSendEvent() { eventSenders_.Pop(); }

        HashMap<StringHash, SharedPtr<Object>> modules_;
        HashMap<StringHash, HashSet<Object*>> eventRecievers_;
        HashMap<Object*, HashMap<StringHash, HashSet<Object*>>> specificEventRecievers_;
        PODVector<Object*> eventSenders_;
        PODVector<VariantMap*> eventDataMaps_;
        EventHandler* eventHandler_;
    };

    template<typename T> void Context::RemoveModule() 
    { 
        modules_.Erase(T::GetTypeStatic());
    }

    template<typename T> T* Context::GetModule() const 
    { 
        HashMap<StringHash, SharedPtr<Object>>::ConstIterator iter = modules_.Find(T::GetTypeStatic());
        if (iter != modules_.End())
            return static_cast<T*>(iter->second_.Get());
        return 0;
    }
}