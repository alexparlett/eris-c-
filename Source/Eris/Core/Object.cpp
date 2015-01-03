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
#include "Event.h"

#include <unordered_map>
#include <unordered_set>

namespace Eris
{
    Object::Object(Context* context) :
        m_context(context)
    {
    }

    Object::~Object()
    {
        unsubscribeFromEvents();
        m_context->removeEventSender(this);
    }

    void Object::subscribeToEvent(const StringHash& event_type, EventHandler* handler, Object* sender /*= nullptr*/)
    {
        if (!handler)
            return;

        handler->setEventType(event_type);
        handler->setSender(sender);

        auto old = m_handlers.end();
        if (!sender)
            old = findEventHandler(event_type);
        else
            old = findEventHandler(event_type, sender);

        if (old != m_handlers.end())
            m_handlers.erase(old);

        m_handlers.push_front(SharedPtr<EventHandler>(handler));
        m_context->addEventReciever(this, event_type, sender);
    }

    void Object::unsubscribeFromEvent(const StringHash& event_type)
    {
        while (true)
        {
            auto handler = findEventHandler(event_type);
            if (handler != m_handlers.end())
            {
                if ((*handler)->getSender())
                    m_context->removeEventReciever(this, event_type, (*handler)->getSender());
                else
                    m_context->removeEventReciever(this, event_type);

                m_handlers.erase(handler);
            }
            else
                break;
        }
    }

    void Object::unsubscribeFromEvent(const StringHash& event_type, Object* sender)
    {
        if (!sender)
            return;

        auto handler = findEventHandler(event_type, sender);
        if (handler != m_handlers.end())
        {
            m_context->removeEventReciever(this, event_type, (*handler)->getSender());
            m_handlers.erase(handler);
        }
    }

    void Object::unsubscribeFromEvents(Object* sender)
    {
        if (!sender)
            return;

        while (true)
        {
            auto handler = findEventHandler(sender);
            if (handler != m_handlers.end())
            {
                m_context->removeEventReciever(this, (*handler)->getEventType(), (*handler)->getSender());
                m_handlers.erase(handler);
            }
            else
                break;
        }
    }

    void Object::unsubscribeFromEvents()
    {
        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getSender())
                m_context->removeEventReciever(this, (*handler)->getEventType(), (*handler)->getSender());
            else
                m_context->removeEventReciever(this, (*handler)->getEventType());

            handler = m_handlers.erase(handler);
        }
    }

    void Object::sendEvent(const StringHash& event_type, Event* event /*= nullptr*/)
    {
        WeakPtr<Object> self(this);
        Context* context = m_context;
        std::unordered_set<Object*> processed;

        const std::unordered_set<Object*>* group = context->getEventRecievers(event_type, this);
        if (group)
        {
            for(auto iter = group->cbegin(); iter != group->cend();)
            {
                auto current = iter++;
                Object* reciever = *current;
                Object* next = nullptr;
                if (iter != group->cend())
                    next = *iter;

                std::size_t oldSize = group->size();

                reciever->onEvent(event_type, event, this);

                if (self.expired())
                    return;

                if (group->size() != oldSize)
                    iter = group->find(next);

                processed.insert(reciever);
            }
        }

        group = context->getEventRecievers(event_type);
        if (group)
        {
            for (auto iter = group->cbegin(); iter != group->cend();)
            {
                auto current = iter++;
                Object* reciever = *current;
                Object* next = nullptr;
                if (iter != group->cend())
                    next = *iter;

                if (processed.empty() || processed.find(reciever) == processed.end())
                {
                    std::size_t oldSize = group->size();

                    reciever->onEvent(event_type, event, this);

                    if (self.expired())
                        return;

                    if (group->size() != oldSize)
                        iter = group->find(next);
                }
            }
        }
    }

    void Object::onEvent(const StringHash& event_type, const Event* event /*= nullptr*/, Object* sender /*= nullptr*/)
    {
        auto specific = m_handlers.end();
        auto non_specific = m_handlers.end();

        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getEventType() == event_type)
            {
                if (!(*handler)->getSender())
                    non_specific = handler;
                else if (sender && (*handler)->getSender() == sender)
                {
                    specific = handler;
                    break;
                }
            }
            handler++;
        }

        if (specific != m_handlers.end())
        {
            (*specific)->invoke(event);
            return;
        }

        if (non_specific != m_handlers.end())
        {
            (*non_specific)->invoke(event);
        }
    }

    void Object::removeEventSender(Object* sender)
    {
        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getSender() == sender)
                handler = m_handlers.erase(handler);
            else
                handler++;
        }
    }

    Object::Iterator Object::findEventHandler(const StringHash& event_type)
    {
        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getEventType() == event_type)
                return handler;
            handler++;
        }
        return m_handlers.end();
    }

    Object::Iterator Object::findEventHandler(const StringHash& event_type, Object* sender)
    {
        if (sender == nullptr)
            return m_handlers.end();

        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getEventType() == event_type && (*handler)->getSender() == sender)
                return handler;
            handler++;
        }
        return m_handlers.end();
    }

    Object::Iterator Object::findEventHandler(Object* sender)
    {
        if (sender == nullptr)
            return m_handlers.end();

        auto handler = m_handlers.begin();
        while (handler != m_handlers.end())
        {
            if ((*handler)->getSender() == sender)
                return handler;
            handler++;
        }
        return m_handlers.end();
    }
}