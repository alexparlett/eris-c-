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

#include "Application/Engine.h"
#include "Core/Context.h"
#include "Graphics.h"

namespace Eris
{
    Graphics::Graphics(Context* context) :
        Object(context),
        m_initialized(false),
        m_fullscreen(true),
        m_borderless(false),
        m_resizable(false),
        m_vsync(true),
        m_width(0),
        m_height(0),
        m_samples(4),
        m_gamma(1.0f),
        m_title("Eris"),
        m_window(nullptr)
    {
    }

    Graphics::~Graphics()
    {
    }

    void Graphics::initialize()
    {
        if (m_initialized)
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

        glfwWindowHint(GLFW_DECORATED, !m_borderless);
        glfwWindowHint(GLFW_RESIZABLE, m_resizable);
        glfwWindowHint(GLFW_SAMPLES, m_samples);

        if (m_width <= 0 || m_height <= 0)
        {
            const GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
            m_width = desktop->width;
            m_height = desktop->height;
        }

        if (m_fullscreen)
            m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), nullptr);
        else
            m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_window)
        {
            m_context->getModule<Engine>()->setExitCode(EXIT_WINDOW_CREATE_ERROR);
            return;
        }
        
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, m_context);

        glm::i32 width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        m_width = width;
        m_height = height;

        if (m_vsync)
            glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            const GLubyte* msg = glewGetErrorString(err);

            glfwDestroyWindow(m_window);
            m_window = nullptr;

            m_context->getModule<Engine>()->setExitCode(EXIT_GLEW_INIT_ERROR);
            return;
        }

        glfwSetFramebufferSizeCallback(m_window, &Graphics::handleFramebufferCallback);
        glfwSetWindowCloseCallback(m_window, &Graphics::handleCloseCallback);

        m_initialized = true;
    }

    void Graphics::terminate()
    {
        m_initialized = false;

        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void Graphics::maximize()
    {
        if (!m_initialized || !m_window)
            return;

        if (!m_fullscreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowSize(m_window, mode->width, mode->height);
        }

        glfwRestoreWindow(m_window);
    }

    void Graphics::minimize()
    {
        if (!m_initialized || !m_window)
            return;

        glfwIconifyWindow(m_window);
    }

    void Graphics::restore()
    {
        if (!m_initialized || !m_window)
            return;

        glfwRestoreWindow(m_window);
    }

    void Graphics::hide()
    {
        if (!m_initialized || !m_window)
            return;

        glfwHideWindow(m_window);
    }

    void Graphics::show()
    {
        if (!m_initialized || !m_window)
            return;

        glfwShowWindow(m_window);
    }

    void Graphics::close()
    {
        if (!m_initialized || !m_window)
            return;

        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }

    void Graphics::setSize(glm::i32 width, glm::i32 height)
    {
        m_width = width;
        m_height = height;

        if (!m_initialized || !m_window)
            return;

        glfwSetWindowSize(m_window, m_width, m_height);
    }

    void Graphics::setSamples(glm::i32 samples)
    {
        m_samples = samples;
    }

    void Graphics::setGamma(glm::f32 gamma)
    {
        m_gamma = gamma;

        if (!m_initialized || !m_window)
            return;

        glfwSetGamma(glfwGetWindowMonitor(m_window), m_gamma);
    }

    void Graphics::setTitle(const std::string& title)
    {
        m_title = title;

        if (!m_initialized || !m_window)
            return;

        glfwSetWindowTitle(m_window, m_title.c_str());
    }

    void Graphics::setFullscreen(bool fullscreen)
    {
        m_fullscreen = fullscreen;
    }

    void Graphics::setResizable(bool resizable)
    {
        m_resizable = !m_fullscreen && resizable;
    }

    void Graphics::setBorderless(bool borderless)
    {
        m_borderless = !m_fullscreen && borderless;
    }

    void Graphics::setVSync(bool vsync)
    {
        m_vsync = vsync;
    }

    void Graphics::handleFramebufferCallback(GLFWwindow* window, glm::i32 width, glm::i32 height)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getModule<Graphics>();
        graphics->m_width = width;
        graphics->m_height = height;
    }

    void Graphics::handleCloseCallback(GLFWwindow* window)
    {

    }
}
