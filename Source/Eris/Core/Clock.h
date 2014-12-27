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

#include "Context.h"
#include "Object.h"

namespace Eris
{
    class Clock : public Object
    {
    public:
        Clock(Context* context);

        void beginFrame(glm::f32 time_step);
        void endFrame();

        glm::f32 getElapsedTime() const;
        glm::f32 getTimeStep() const;
        glm::u64 getFrameNumber() const;
        std::string getTimestamp() const;

    private:
        glm::u64 m_frame_number;
        glm::f32 m_time_step;
    };

    template<> inline void Context::registerModule(Clock* engine)
    {
        m_clock = SharedPtr<Clock>(engine);
    }

    template<> inline Clock* Context::getModule()
    {
        ERIS_ASSERT(m_clock);
        return m_clock.get();
    }
}