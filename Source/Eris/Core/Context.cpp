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

#include "Context.h"
#include "Object.h"

namespace Eris
{

    void Context::RegisterModule(Object* object)
    {
        if (!object)
            return;

        modules_[object->GetType()] = object;
    }

    VariantMap& Context::GetEventDataMap()
    {
        unsigned nestingLevel = eventSenders_.Size();
        while (eventDataMaps_.Size() < nestingLevel + 1)
            eventDataMaps_.Push(new VariantMap());

        VariantMap& ret = *eventDataMaps_[nestingLevel];
        ret.Clear();
        return ret;
    }

    Object* Context::GetEventSender() const
    {
        if (!eventSenders_.Empty())
            return eventSenders_.Back();
        else
            return 0;
    }

    void Context::AddEventReceiver(Object* reciever, StringHash eventType)
    {
        eventRecievers_[eventType].Insert(reciever);
    }

    void Context::AddEventReceiver(Object* reciever, Object* sender, StringHash eventType)
    {
        specificEventRecievers_[sender][eventType].Insert(reciever);
    }

    void Context::RemoveEventReceiver(Object* reciever, StringHash eventType)
    {
        HashSet<Object*>* group = GetEventReceivers(eventType);
        if (group)
            group->Erase(reciever);
    }

    void Context::RemoveEventReceiver(Object* reciever, Object* sender, StringHash eventType)
    {
        HashSet<Object*>* group = GetEventReceivers(sender, eventType);
        if (group)
            group->Erase(reciever);
    }

    void Context::RemoveEventSender(Object* sender)
    {
        HashMap<Object*, HashMap<StringHash, HashSet<Object*> > >::Iterator i = specificEventRecievers_.Find(sender);
        if (i != specificEventRecievers_.End())
        {
            for (HashMap<StringHash, HashSet<Object*> >::Iterator j = i->second_.Begin(); j != i->second_.End(); ++j)
            {
                for (HashSet<Object*>::Iterator k = j->second_.Begin(); k != j->second_.End(); ++k)
                    (*k)->RemoveEventSender(sender);
            }
            specificEventRecievers_.Erase(i);
        }
    }

    HashSet<Object*>* Context::GetEventReceivers(StringHash eventType)
    {
        HashMap<StringHash, HashSet<Object*>>::Iterator iter = eventRecievers_.Find(eventType);
        if (iter != eventRecievers_.End())
            return &iter->second_;
        return 0;
    }

    HashSet<Object*>* Context::GetEventReceivers(Object* sender, StringHash eventType)
    {
        HashMap<Object*, HashMap<StringHash, HashSet<Object*>>>::Iterator si = specificEventRecievers_.Find(sender);
        if (si != specificEventRecievers_.End())
        {
            HashMap<StringHash, HashSet<Object*>>::Iterator iter = si->second_.Find(eventType);
            if (iter != eventRecievers_.End())
                return &iter->second_;
        }
        return 0;
    }

}