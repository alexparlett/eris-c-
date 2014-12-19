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
#include "CoreEvents.h"
#include "Timer.h"

#include <windows.h>
#include <mmsystem.h>
#include <ctime>
#include <algorithm>

namespace Eris
{

    Time::Time(Context* context) :
        Object(context),
        frameNumber_(0),
        timeStep_(0)
    {

    }

    void Time::beginFrame(double timeStep)
    {
        frameNumber_++;
        if (!frameNumber_)
            frameNumber_++;

        timeStep_ = timeStep;


        BeginFrameEvent* event = context_->createEvent<BeginFrameEvent>();
        event->frame = frameNumber_;
        event->timeStep = timeStep_;
        sendEvent(BeginFrameEvent::getTypeStatic(), event);
    }

    void Time::endFrame()
    {
        sendEvent(EndFrameEvent::getTypeStatic());
    }

    double Time::getElapsedTime() const
    {
        return glfwGetTime() * 1000.0;
    }

    glm::uint Time::getSystemTime()
    {
        return timeGetTime();
    }

    glm::uint Time::getTimeSinceEpoch()
    {
        return (glm::uint) time(NULL);
    }

    std::string Time::getTimestamp()
    {
        time_t sysTime;
        time(&sysTime);
        char dateTime[32];

        ctime_s(dateTime, 32, &sysTime);

        std::string dateString(dateTime);

        std::string::size_type pos = 0; // Must initialize
        while ((pos = dateString.find("\r\n", pos)) != std::string::npos)
        {
            dateString.erase(pos, 2);
        }

        return dateString;
    }


    Timer::Timer() :
        startTime_(0)
    {
    }

    double Timer::getElapsedTime(bool reset)
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - startTime_;

        if (elapsedTime < 0)
            elapsedTime = 0;

        if (reset)
            startTime_ = currentTime;

        return elapsedTime * 1000.0;
    }

    void Timer::reset()
    {
        startTime_ = glfwGetTime();
    }

}