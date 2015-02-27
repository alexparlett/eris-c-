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

#include "Event.h"
#include "Collections/StringHash.h"
#include "Memory/RefCounted.h"

namespace Eris
{
    class Object;

    class EventHandler : public RefCounted
    {
        using HandlerFunctionPtr = std::function < void(const StringHash&, const Event*) > ;

    public:
        EventHandler(HandlerFunctionPtr function);
        EventHandler(HandlerFunctionPtr function, void* user_data);

        virtual ~EventHandler();

        void setSender(Object* sender);
        void setEventType(StringHash event_type);

        Object* getSender() const { return m_sender; }
        const StringHash& getEventType() const { return m_event_type; }

        void invoke(const Event* event);

    private:
        Object* m_sender;
        StringHash m_event_type;
        HandlerFunctionPtr m_function;
        void* m_user_data;
    };

#define HANDLER(className, function) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2)))
#define HANDLER_USERDATA(className, function, userData) (new Eris::EventHandler(std::bind(&className::function, this, std::placeholders::_1, std::placeholders::_2), userData))
}