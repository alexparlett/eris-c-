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

#include <cassert>
#include <memory>

namespace Eris
{

    Object::Object(Context* context) :
        context_(context)
    {
        assert(context_);
    }

    Object::~Object()
    {
        unsubscribeFromEvents();
        context_->removeEventSender(this);
    }

    void Object::subscribeToEvent(const std::string& eventType, EventHandler* handler)
    {
        if (!handler)
            return;

        handler->setEventType(eventType);

        EventHandler* oldHandler = findEventHandler(eventType);
        if (oldHandler)
        {
            eventHandlers_.remove(oldHandler);
            delete oldHandler;
        }

        eventHandlers_.push_front(handler);

        context_->addEventReceiver(this, eventType);
    }

    void Object::subscribeToEvent(Object* sender, const std::string& eventType, EventHandler* handler)
    {
        if (!sender || !handler)
        {
            if (handler)
                delete handler;

            return;
        }

        handler->setEventType(eventType);
        handler->setSender(sender);

        EventHandler* oldHandler = findSpecificEventHandler(sender, eventType);
        if (oldHandler)
        {
            eventHandlers_.remove(oldHandler);
            delete oldHandler;
        }

        eventHandlers_.push_front(handler);

        context_->addEventReceiver(this, sender, eventType);
    }

    void Object::unsubscribeFromEvent(const std::string& eventType)
    {
        for (;;)
        {
            EventHandler* previous;
            EventHandler* handler = findEventHandler(eventType, &previous);

            if (handler)
            {
                if (handler->getSender())
                    context_->removeEventReceiver(this, handler->getSender(), eventType);
                else
                    context_->removeEventReceiver(this, eventType);

                eventHandlers_.remove(handler);
                delete handler;
            }
            else
                break;
        }
    }

    void Object::unsubscribeFromEvent(Object* sender, const std::string& eventType)
    {
        if (!sender)
            return;

        EventHandler* previous;
        EventHandler* handler = findSpecificEventHandler(sender, eventType, &previous);

        if (handler)
        {
            context_->removeEventReceiver(this, handler->getSender(), eventType);
            eventHandlers_.remove(handler);
            delete handler;
        }
    }

    void Object::unsubscribeFromEvents(Object* sender)
    {
        if (!sender)
            return;

        for (;;)
        {
            EventHandler* previous;
            EventHandler* handler = findSpecificEventHandler(sender, &previous);

            if (handler)
            {
                context_->removeEventReceiver(this, handler->getSender(), handler->getEventType());
                eventHandlers_.remove(handler);
            }
            else
                break;
        }
    }

    void Object::unsubscribeFromEvents()
    {
        for (;;)
        {
            EventHandler* handler = eventHandlers_.front();

            if (handler)
            {
                if (handler->getSender())
                    context_->removeEventReceiver(this, handler->getSender(), handler->getEventType());
                else
                    context_->removeEventReceiver(this, handler->getEventType());
                eventHandlers_.remove(handler);
                delete handler;
            }
            else
                break;
        }
    }

    void Object::sendEvent(const std::string& eventType)
    {
        sendEvent(eventType, nullptr);
    }

    void Object::sendEvent(const std::string& eventType, Event* eventData)
    {
        // Make a weak pointer to self to check for destruction during event handling
        std::weak_ptr<Object> self(std::make_shared<Object>(this));
        Context* context = context_;
        std::unordered_set<Object*> processed;

        context->beginSendEvent(this);

        // Check first the specific event receivers
        const std::unordered_set<Object*>* group = context->getEventReceivers(this, eventType);
        if (group)
        {
            for (std::unordered_set<Object*>::const_iterator i = group->begin(); i != group->end();)
            {
                std::unordered_set<Object*>::const_iterator current = i++;
                Object* receiver = *current;
                Object* next = 0;
                if (i != group->end())
                    next = *i;

                unsigned oldSize = group->size();
                receiver->onEvent(this, eventType, eventData);

                // If self has been destroyed as a result of event handling, exit
                if (self.expired())
                {
                    context->endSendEvent();
                    return;
                }

                // If group has changed size during iteration (removed/added subscribers) try to recover
               if (group->size() != oldSize)
                    i = group->find(next);

                processed.insert(receiver);
            }
        }

        // Then the non-specific receivers
        group = context->getEventReceivers(eventType);
        if (group)
        {
            if (processed.empty())
            {
                for (std::unordered_set<Object*>::const_iterator i = group->begin(); i != group->end();)
                {
                    std::unordered_set<Object*>::const_iterator current = i++;
                    Object* receiver = *current;
                    Object* next = 0;
                    if (i != group->end())
                        next = *i;

                    unsigned oldSize = group->size();
                    receiver->onEvent(this, eventType, eventData);

                    if (self.expired())
                    {
                        context->endSendEvent();
                        return;
                    }

                    if (group->size() != oldSize)
                        i = group->find(next);
                }
            }
            else
            {
                // If there were specific receivers, check that the event is not sent doubly to them
                for (std::unordered_set<Object*>::const_iterator i = group->begin(); i != group->end();)
                {
                    std::unordered_set<Object*>::const_iterator current = i++;
                    Object* receiver = *current;
                    Object* next = 0;
                    if (i != group->end())
                        next = *i;

                    if (processed.find(receiver) != processed.end())
                    {
                        unsigned oldSize = group->size();
                        receiver->onEvent(this, eventType, eventData);

                        if (self.expired())
                        {
                            context->endSendEvent();
                            return;
                        }

                        if (group->size() != oldSize)
                            i = group->find(next);
                    }
                }
            }
        }

        context->endSendEvent();
    }

    void Object::removeEventSender(Object* sender)
    {
        std::list<EventHandler*>::iterator handler = eventHandlers_.begin();
        std::list<EventHandler*>::iterator previous;

        while (handler != eventHandlers_.end())
        {
            if ((*handler)->getSender() == sender)
                handler = eventHandlers_.erase(handler);
            else
                handler++;
        }
    }

    EventHandler* Object::findEventHandler(const std::string& eventType, EventHandler** previous) const
    {
        std::list<EventHandler*>::const_iterator handler = eventHandlers_.cbegin();
        if (previous)
            *previous = nullptr;

        while (handler != eventHandlers_.cend())
        {
            if ((*handler)->getEventType() == eventType)
                return (*handler);
            if (previous)
                *previous = (*handler);
            handler++;
        }

        return nullptr;
    }

    EventHandler* Object::findSpecificEventHandler(Object* sender, EventHandler** previous) const
    {
        std::list<EventHandler*>::const_iterator handler = eventHandlers_.cbegin();
        if (previous)
            *previous = nullptr;

        while (handler != eventHandlers_.cend())
        {
            if ((*handler)->getSender() == sender)
                return (*handler);
            if (previous)
                *previous = (*handler);
            handler++;
        }

        return nullptr;
    }

    EventHandler* Object::findSpecificEventHandler(Object* sender, const std::string& eventType, EventHandler** previous) const
    {
        std::list<EventHandler*>::const_iterator handler = eventHandlers_.cbegin();
        if (previous)
            *previous = nullptr;

        while (handler != eventHandlers_.cend())
        {
            if ((*handler)->getSender() == sender && (*handler)->getEventType() == eventType)
                return *handler;
            if (previous)
                *previous = *handler;
            handler++;
        }

        return nullptr;
    }

    void Object::onEvent(Object* sender, const std::string& eventType, Event* eventData)
    {
        Context* context = context_;
        EventHandler* specific = nullptr;
        EventHandler* nonSpecific = nullptr;

        std::list<EventHandler*>::const_iterator handler = eventHandlers_.cbegin();
        while (handler != eventHandlers_.end())
        {
            if ((*handler)->getEventType() == eventType)
            {
                if (!(*handler)->getSender())
                    nonSpecific = *handler;
                else if ((*handler)->getSender() == sender)
                {
                    specific = *handler;
                    break;
                }
            }
            handler++;
        }

        if (specific)
        {
            context->setEventHandler(specific);
            specific->invoke(eventData);
            context->setEventHandler(0);
            return;
        }

        if (nonSpecific)
        {
            context->setEventHandler(nonSpecific);
            nonSpecific->invoke(eventData);
            context->setEventHandler(0);
        }
    }

}