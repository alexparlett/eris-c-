//
// Copyright (c) 2013-2015 the Eris project.
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

#include "Graphics.h"
#include "Events.h"

#include "Engine/Engine.h"
#include "Engine/Events.h"
#include "Core/Log.h"

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
        m_title(ERIS_APP),
        m_icon("icon.ico"),
        m_main_window(nullptr),
        m_resource_window(nullptr)
    {
    }

    void Graphics::initialize()
    {
        if (m_initialized)
            return;

        if (!initializeMainWindow() || !initializeResourceWindow())
            return;

        m_initialized = true;

        setIcon(m_icon);
    }

    void Graphics::terminate()
    {
        m_initialized = false;

        if (m_main_window)
        {
            glfwDestroyWindow(m_main_window);
            m_main_window = nullptr;
        }

        if (m_resource_window)
        {
            glfwDestroyWindow(m_resource_window);
            m_resource_window = nullptr;
        }
    }

    void Graphics::maximize()
    {
        if (!m_initialized || !m_main_window)
            return;

        if (!m_fullscreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowSize(m_main_window, mode->width, mode->height);
        }

        glfwRestoreWindow(m_main_window);
    }

    void Graphics::minimize()
    {
        if (!m_initialized || !m_main_window)
            return;

        glfwIconifyWindow(m_main_window);
    }

    void Graphics::restore()
    {
        if (!m_initialized || !m_main_window)
            return;

        glfwRestoreWindow(m_main_window);
    }

    void Graphics::hide()
    {
        if (!m_initialized || !m_main_window)
            return;

        glfwHideWindow(m_main_window);
    }

    void Graphics::show()
    {
        if (!m_initialized || !m_main_window)
            return;

        glfwShowWindow(m_main_window);
    }

    void Graphics::close()
    {
        if (!m_initialized || !m_main_window)
            return;

        glfwSetWindowShouldClose(m_main_window, GL_TRUE);
    }

    void Graphics::setSize(glm::i32 width, glm::i32 height)
    {
        m_width = width;
        m_height = height;

        if (!m_initialized || !m_main_window)
            return;

        glfwSetWindowSize(m_main_window, m_width, m_height);
    }

    void Graphics::setSamples(glm::i32 samples)
    {
        m_samples = samples;
    }

    void Graphics::setGamma(glm::f32 gamma)
    {
        m_gamma = glm::max(gamma, 1.0f);

        if (!m_initialized || !m_main_window)
            return;

        glfwSetGamma(glfwGetWindowMonitor(m_main_window), m_gamma);
    }

    void Graphics::setTitle(const std::string& title)
    {
        m_title = title;

        if (!m_initialized || !m_main_window)
            return;

        glfwSetWindowTitle(m_main_window, m_title.c_str());
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

    void Graphics::setIcon(const std::string& icon)
    {
        m_icon = icon;

        if (!m_initialized || !m_main_window || !m_resource_window)
            return;

        HWND main_hwnd = glfwGetWin32Window(m_main_window);
        HWND res_hwnd = glfwGetWin32Window(m_resource_window);
        HANDLE hicon = (HICON) LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
        if (hicon)
        {
            SendMessage(main_hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hicon);
            SendMessage(main_hwnd, WM_SETICON, ICON_BIG, (LPARAM) hicon);

            SendMessage(res_hwnd, WM_SETICON, ICON_SMALL, (LPARAM) hicon);
            SendMessage(res_hwnd, WM_SETICON, ICON_BIG, (LPARAM) hicon);
        }
        else
            Log::errorf("Failed setting Window Icon: %d", GetLastError());
    }

    std::vector<glm::ivec2> Graphics::getResolutions() const
    {
        glm::i32 count;
        const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

        if (count > 0 && modes)
        {
            std::vector<glm::ivec2> out;
            for (glm::i32 i = 0; i < count; i++)
                out.push_back(glm::ivec2(modes[i].width, modes[i].height));
            return out;
        }

        return std::vector<glm::ivec2>();
    }

    glm::ivec2 Graphics::getResolution() const
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (mode)
            return glm::ivec2(mode->width, mode->height);
        return glm::ivec2();
    }

    void Graphics::handleFramebufferCallback(GLFWwindow* window, glm::i32 width, glm::i32 height)
    {
        Log::infof("Window resized width: %d height:%d", width, height);

        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getModule<Graphics>();
        graphics->m_width = width;
        graphics->m_height = height;

        ScreenModeEvent* event = context->getFrameAllocator().newInstance<ScreenModeEvent>();
        event->width = graphics->m_width;
        event->height = graphics->m_height;
        graphics->sendEvent(ScreenModeEvent::getTypeStatic(), event);
    }

    void Graphics::handleCloseCallback(GLFWwindow* window)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getModule<Graphics>();

        graphics->sendEvent(ExitRequestedEvent::getTypeStatic());
    }

    bool Graphics::initializeMainWindow()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
            m_main_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), nullptr);
        else
            m_main_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_main_window)
        {
            m_context->getModule<Engine>()->setExitCode(EXIT_WINDOW_CREATE_ERROR);
            return false;
        }

        glfwMakeContextCurrent(m_main_window);
        glfwSetWindowUserPointer(m_main_window, m_context);

        glm::i32 width, height;
        glfwGetFramebufferSize(m_main_window, &width, &height);
        m_width = width;
        m_height = height;

        if (m_vsync)
            glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            const GLubyte* msg = glewGetErrorString(err);
            Log::errorf("GLEW error: %s", msg);

            glfwDestroyWindow(m_main_window);
            m_main_window = nullptr;

            m_context->getModule<Engine>()->setExitCode(EXIT_GLEW_INIT_ERROR);
            return false;
        }

        glfwSetFramebufferSizeCallback(m_main_window, &Graphics::handleFramebufferCallback);
        glfwSetWindowCloseCallback(m_main_window, &Graphics::handleCloseCallback);

        Log::raw("Initializing Graphics...");
        Log::rawf("\tAdapter: %s %s", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
        Log::rawf("\tOpen GL: %s", glGetString(GL_VERSION));
        Log::rawf("\tGLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        Log::rawf("\tGLEW: %s", glewGetString(GLEW_VERSION));

        glfwMakeContextCurrent(nullptr);

        return true;
    }

    bool Graphics::initializeResourceWindow()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

        m_resource_window = glfwCreateWindow(1, 1, m_title.c_str(), nullptr, m_main_window);

        if (!m_resource_window)
        {
            m_context->getModule<Engine>()->setExitCode(EXIT_WINDOW_CREATE_ERROR);
            return false;
        }

        glfwMakeContextCurrent(m_resource_window);
        glfwSetWindowUserPointer(m_resource_window, m_context);

        if (m_vsync)
            glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            const GLubyte* msg = glewGetErrorString(err);
            Log::errorf("GLEW error: %s", msg);

            glfwDestroyWindow(m_resource_window);
            m_resource_window = nullptr;

            m_context->getModule<Engine>()->setExitCode(EXIT_GLEW_INIT_ERROR);
            return false;
        }

        glfwSetWindowCloseCallback(m_resource_window, &Graphics::handleCloseCallback);
        glfwMakeContextCurrent(nullptr);

        return true;
    }
}
