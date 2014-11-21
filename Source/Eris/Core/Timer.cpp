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
#include <glfw3.h>

namespace Eris
{

    Time::Time(Context* context) :
        Object(context),
        frameNumber_(0),
        timeStep_(0)
    {

    }

    void Time::BeginFrame(float timeStep)
    {
        frameNumber_++;
        if (!frameNumber_)
            frameNumber_++;

        timeStep_ = timeStep;

        using namespace BeginFrame;

        VariantMap& eventData = context_->GetEventDataMap();
        eventData[P_FRAME] = frameNumber_;
        eventData[P_TIMESTEP] = timeStep_;
        SendEvent(E_BEGINFRAME, eventData);
    }

    void Time::EndFrame()
    {
        SendEvent(E_ENDFRAME);
    }

    float Time::GetElapsedTime() const
    {
        return glfwGetTime() * 1000.f;
    }

    unsigned Time::GetSystemTime()
    {
        return timeGetTime();
    }

    unsigned Time::GetTimeSinceEpoch()
    {
        return (unsigned) time(NULL);
    }

    String Time::GetTimestamp()
    {
        time_t sysTime;
        time(&sysTime);
        char dateTime[32];

        ctime_s(dateTime, 32, &sysTime);

        return String(dateTime).Replaced("\n", "").Trimmed();
    }


    Timer::Timer() :
        startTime_(0)
    {
    }

    long long Timer::GetElapsedTime(bool reset)
    {
        long long currentTime = glfwGetTime();
        long long elapsedTime = currentTime - startTime_;

        if (elapsedTime < 0)
            elapsedTime = 0;

        if (reset)
            startTime_ = currentTime;

        return (elapsedTime * 1000000LL) / 1000LL;
    }

    void Timer::Reset()
    {
        startTime_ = glfwGetTime();
    }

}