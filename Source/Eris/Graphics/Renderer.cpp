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
#include "RenderCommand.h"

#include "Engine/Events.h"
#include "Core/Clock.h"
#include "Core/Log.h"
#include "Core/Profiler.h"

namespace Eris
{
    Renderer::Renderer(Context* context) :
        Object(context),
        m_thread_exit(false),
        m_initialized(false),
        m_state(new RenderState(context))
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

        subscribeToEvent(RenderEvent::getTypeStatic(), HANDLER(Renderer, handleRender));
        m_thread = std::thread(&Renderer::run, this);
    }

    void Renderer::run()
    {
        Log::infof("Rendered Thread started: %d", std::this_thread::get_id().hash());

        Clock* clock = m_context->getModule<Clock>();
        Graphics* graphics = m_context->getModule<Graphics>();
        ResourceCache* rc = m_context->getModule<ResourceCache>();

        GLFWwindow* window = graphics->getWindow();
        if (!initializeOpenGL(window, graphics->getWidth(), graphics->getHeight()))
            return;

        while (!m_thread_exit)
        {
            m_state->process();
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
        ERIS_ASSERT(window);
        ERIS_ASSERT(width > 0);
        ERIS_ASSERT(height > 0);

        // Make current
        if (window)
            glfwMakeContextCurrent(window);
        else
        {
            Log::error("Renderer initialized and run with no window.");
            return false;
        }

        ERIS_ASSERT(glfwGetCurrentContext() == window);

        // Enable GL Flags
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        // Set Viewport
        //TODO remove this to camera class
        glViewport(0, 0, width, height);

        return true;
    }

    void Renderer::handleScreenMode(const StringHash& type, const Event* event)
    {
        m_viewport_dirty = true;
    }

    void Renderer::handleRender(const StringHash& type, const Event* event)
    {
        ClearColorCommand* clear_color_command = new ClearColorCommand();
        clear_color_command->key.command = 0;
        clear_color_command->key.depth = 0;
        clear_color_command->key.extra = 0;
        clear_color_command->key.material = 0;
        clear_color_command->key.target = 0;
        clear_color_command->key.target_layer = 0;
        clear_color_command->key.transparency = 0;
        clear_color_command->color = glm::vec4(0.0f, 0.0f, 0.0f, 1.f);

        ClearCommand* clear_command = new ClearCommand();
        clear_command->key.command = 0;
        clear_command->key.depth = 0;
        clear_command->key.extra = 0;
        clear_command->key.material = 0;
        clear_command->key.target = 0;
        clear_command->key.target_layer = 0;
        clear_command->key.transparency = 0;
        clear_command->bitfield = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        
        EnableCommand* enable_depth_command = new EnableCommand();
        enable_depth_command->key.command = 1;
        enable_depth_command->key.depth = 0;
        enable_depth_command->key.extra = 0;
        enable_depth_command->key.material = 0;
        enable_depth_command->key.target = 0;
        enable_depth_command->key.target_layer = 1;
        enable_depth_command->key.transparency = 0;
        enable_depth_command->capability = GL_DEPTH_TEST;

        EnableCommand* enable_cull_command = new EnableCommand();
        enable_cull_command->key.command = 1;
        enable_cull_command->key.depth = 0;
        enable_cull_command->key.extra = 0;
        enable_cull_command->key.material = 0;
        enable_cull_command->key.target = 0;
        enable_cull_command->key.target_layer = 1;
        enable_cull_command->key.transparency = 0;
        enable_cull_command->capability = GL_CULL_FACE;

        auto state = getState();
        state->add(clear_color_command);
        state->add(clear_command);
        state->add(enable_cull_command);
        state->add(enable_depth_command);
    }

}