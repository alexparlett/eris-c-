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

#include "EventHandler.h"

namespace Eris
{
    EventHandler::EventHandler(HandlerFunctionPtr function) :
        m_sender(0),
        m_function(function),
        m_user_data(0)
    {
    }

    EventHandler::EventHandler(HandlerFunctionPtr function, void* user_data) :
        m_sender(0),
        m_function(function),
        m_user_data(user_data)
    {
    }

    EventHandler::~EventHandler() 
    {
    }

    void EventHandler::setSender(Object* sender) 
    { 
        m_sender = sender; 
    }

    void EventHandler::setEventType(StringHash event_type) 
    { 
        ERIS_ASSERT(event_type != StringHash::ZERO)
        m_event_type = event_type; 
    }

    void EventHandler::invoke(const Event* event) 
    { 
        m_function(m_event_type, event); 
    }

}

