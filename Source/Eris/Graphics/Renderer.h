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

#include "RenderState.h"

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/Pointers.h"

namespace Eris
{
    class Renderer : public Object
    {
    public:
        Renderer(Context* context);
        virtual ~Renderer();

        void initialize();
        void run();
        void terminate();

        RenderState* getState() const { return m_state; }

    private:
        bool initializeOpenGL(GLFWwindow* window, glm::i32 width, glm::i32 height);

        void handleScreenMode(const StringHash& type, const Event* event);
        void handleRender(const StringHash& type, const Event* event);

        bool m_initialized;
        std::thread m_thread;
        std::atomic<bool> m_thread_exit;
        std::atomic<bool> m_viewport_dirty;
        SharedPtr<RenderState> m_state;
    };

    template<> inline void Context::registerModule(Renderer* module)
    {
        m_renderer = SharedPtr<Renderer>(module);
    }

    template<> inline Renderer* Context::getModule()
    {
        ERIS_ASSERT(m_renderer);
        return m_renderer.get();
    }
}
