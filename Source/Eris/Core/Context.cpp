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

#include "Graphics.h"
#include "Input.h"
#include "Timer.h"

namespace Eris
{
    Context::Context() : 
        eventHandler_(nullptr),
        graphics(nullptr),
        input(nullptr),
        time(nullptr)
    {

    }

    Context::~Context()
    {
        delete graphics;
        delete input;
        delete time;
    }

    Object* Context::getEventSender() const
    {
        if (!eventSenders_.empty())
            return eventSenders_.back();
        else
            return nullptr;
    }

    void Context::addEventReceiver(Object* reciever, const std::string& eventType)
    {
        eventRecievers_[eventType].insert(reciever);
    }

    void Context::addEventReceiver(Object* reciever, Object* sender, const std::string& eventType)
    {
        specificEventRecievers_[sender][eventType].insert(reciever);
    }

    void Context::removeEventReceiver(Object* reciever, const std::string& eventType)
    {
        std::unordered_set<Object*>* group = getEventReceivers(eventType);
        if (group)
            group->erase(reciever);
    }

    void Context::removeEventReceiver(Object* reciever, Object* sender, const std::string& eventType)
    {
        std::unordered_set<Object*>* group = getEventReceivers(sender, eventType);
        if (group)
            group->erase(reciever);
    }

    void Context::removeEventSender(Object* sender)
    {
        std::unordered_map<Object*, std::unordered_map<std::string, std::unordered_set<Object*> > >::iterator i = specificEventRecievers_.find(sender);
        if (i != specificEventRecievers_.end())
        {
            for (std::unordered_map<std::string, std::unordered_set<Object*> >::iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                for (std::unordered_set<Object*>::iterator k = j->second.begin(); k != j->second.end(); ++k)
                    (*k)->removeEventSender(sender);
            }
            specificEventRecievers_.erase(i);
        }
    }

    std::unordered_set<Object*>* Context::getEventReceivers(const std::string& eventType)
    {
        std::unordered_map<std::string, std::unordered_set<Object*>>::iterator iter = eventRecievers_.find(eventType);
        if (iter != eventRecievers_.end())
            return &iter->second;
        return nullptr;
    }

    std::unordered_set<Object*>* Context::getEventReceivers(Object* sender, const std::string& eventType)
    {
        std::unordered_map<Object*, std::unordered_map<std::string, std::unordered_set<Object*>>>::iterator si = specificEventRecievers_.find(sender);
        if (si != specificEventRecievers_.end())
        {
            std::unordered_map<std::string, std::unordered_set<Object*>>::iterator iter = si->second.find(eventType);
            if (iter != eventRecievers_.end())
                return &iter->second;
        }
        return nullptr;
    }
}