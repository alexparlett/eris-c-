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

#include "Events.h"
#include "Graphics.h"
#include "Renderer.h"

#include "Core/Clock.h"
#include "Core/Log.h"

namespace Eris
{
    Renderer::Renderer(Context* context) :
        Object(context),
        m_thread_exit(false),
        m_initialized(false)
    {
        subscribeToEvent(ScreenModeEvent::getTypeStatic(), HANDLER(Renderer, handleScreenMode));
    }

    Renderer::~Renderer()
    {
        m_initialized = false;

        m_thread_exit = true;
        if (m_thread.joinable())
            m_thread.join();
    }

    void Renderer::initialize()
    {
        if (m_initialized)
            return;

        m_thread = std::thread(&Renderer::run, this);
    }

    void Renderer::run()
    {
        Log::infof("Rendered Thread started: %d", std::this_thread::get_id().hash());

        Clock* clock = m_context->getModule<Clock>();
        Graphics* graphics = m_context->getModule<Graphics>();

        GLFWwindow* window = graphics->getWindow();
        if (!initializeOpenGL(window, graphics->getWidth(), graphics->getHeight()))
            return;

        glm::f64 current_time = clock->getElapsedTime();
        while (!m_thread_exit)
        {
            glm::f64 new_time = clock->getElapsedTime();
            glm::f64 delta_time = new_time - current_time;
            current_time = new_time;

            glClearColor(0.f, 0.f, 0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //TODO rendering

            glfwSwapBuffers(window);

            if (m_viewport_dirty)
            {
                glViewport(0, 0, graphics->getWidth(), graphics->getHeight());
                glfwSwapBuffers(window);
            }
        }

        glfwMakeContextCurrent(nullptr);

        Log::infof("Renderer Thread stopped: %d", std::this_thread::get_id().hash());

    }

    void Renderer::terminate()
    {
        m_initialized = false;

        m_thread_exit = true;
        if (m_thread.joinable())
            m_thread.join();
    }

    bool Renderer::initializeOpenGL(GLFWwindow* window, glm::i32 width, glm::i32 height)
    {
        // Make current
        if (window)
            glfwMakeContextCurrent(window);
        else
        {
            Log::error("Renderer initialized and run with no window.");
            return false;
        }

        // Enable GL Flags
        glEnable(GL_MULTISAMPLE);

        // Set Viewport
        //TODO remove this to camera class
        glViewport(0, 0, width, height);

        return true;
    }

    void Renderer::handleScreenMode(const StringHash& type, const Event* event)
    {
        m_viewport_dirty = true;
    }

}