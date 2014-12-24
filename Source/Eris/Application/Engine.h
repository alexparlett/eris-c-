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

#include "Core/Object.h"
#include "Core/Context.h"

namespace Eris
{
    static const glm::i32 EXIT_OK = 0;
    static const glm::i32 EXIT_INITIALIZATION_FAILURE = 1;
    static const glm::i32 EXIT_GLFW_INIT_ERROR = 2;
    static const glm::i32 EXIT_GLEW_INIT_ERROR = 3;
    static const glm::i32 EXIT_WINDOW_CREATE_ERROR = 4;

    class Engine : public Object
    {
    public:
        Engine(Context* context);
        virtual ~Engine();

        void initialize();
        void run();
        void terminate();

        void setExitCode(glm::i32 exitcode);
        glm::i32 getExitCode() const { return m_exitcode; }

    private:
        glm::i32 m_exitcode;
    };

    template<> inline void Context::registerModule<Engine>(Engine* engine)
    {
        m_engine = SharedPtr<Engine>(engine);
    }

    template<> inline Engine* Context::getModule<Engine>()
    {
        return m_engine.get();
    }
}
