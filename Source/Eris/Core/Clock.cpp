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

#include "Clock.h"
#include "Events.h"

#include "Collections/Functions.h"

#include <ctime>

namespace Eris
{


    Clock::Clock(Context* context) :
        Object(context),
        m_time_step(0.f),
        m_frame_number(0)
    {

    }

    void Clock::beginFrame(glm::f32 time_step)
    {
        m_frame_number++;
        m_time_step = time_step;

        BeginFrameEvent* event = m_context->getFrameAllocator().newInstance<BeginFrameEvent>();
        event->frame_number = m_frame_number;
        event->time_step = m_time_step;

        sendEvent(BeginFrameEvent::getTypeStatic(), event);
    }

    void Clock::endFrame()
    {
        sendEvent(EndFrameEvent::getTypeStatic());
    }

    glm::f32 Clock::getElapsedTime() const
    {
        return glfwGetTime() * 1000.f;
    }

    glm::f32 Clock::getTimeStep() const
    {
        return m_time_step;
    }

    glm::u64 Clock::getFrameNumber() const
    {
        return m_frame_number;
    }

    std::string Clock::getTimestamp() const
    {
        time_t sysTime;
        time(&sysTime);
        char dateTime[32];

        ctime_s(dateTime, 32, &sysTime);

        return std::string_replace(dateTime, "\n", "");
    }

}