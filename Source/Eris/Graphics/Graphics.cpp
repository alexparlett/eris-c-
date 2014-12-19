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
// LIABILITY, WindowHints::WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Context.h"
#include "Graphics.h"
#include "GraphicsEvents.h"

#include <common.hpp>

namespace Eris
{

    Graphics::Graphics(Context* context) :
        Object(context),
        window_(0)
    {

    }

    Graphics::~Graphics()
    {
        if (window_);
        {
            glfwDestroyWindow(window_);
            window_ = 0;
        }
    }

    void Graphics::initialize(const glm::ivec2& size, glm::int32 samples, const std::string& title, glm::int32 hints)
    {
        // Default Samples, Title and Hints.
        samples_ = samples;
        title_ = title;
        hints_ = hints;

        // OpenGL 3.3 Context.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Set Window Hints.
        glfwWindowHint(GLFW_DECORATED, !isHintEnabled(WindowHints::WH_BORDERLESS));
        glfwWindowHint(GLFW_RESIZABLE, isHintEnabled(WindowHints::WH_RESIZABLE));
        glfwWindowHint(GLFW_VISIBLE, isHintEnabled(WindowHints::WH_VISIBLE));
        glfwWindowHint(GLFW_SAMPLES, samples_);
        glfwWindowHint(GLFW_SRGB_CAPABLE, isHintEnabled(WindowHints::WH_SRGB));

        // Create Window.
        if (isHintEnabled(WindowHints::WH_FULLSCREEN))
            window_ = glfwCreateWindow(size.x, size.y, title_.c_str(), glfwGetPrimaryMonitor(), NULL);
        else
            window_ = glfwCreateWindow(size.x, size.y, title_.c_str(), NULL, NULL);

        if (!window_)
        {
            LOGERROR("Failed to create window with params: [%i,%i,%i,%x]", size.x_, size.y_, samples_, hints_);
            return;
        }

        // Recheck the size since it may have changed for fullscreen resolutions.
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        size_ = glm::ivec2(width, height);

        // Store the Context Ptr and make current.
        glfwSetWindowUserPointer(window_, context_);
        glfwMakeContextCurrent(window_);

        // Enable Vsync.
        if (isHintEnabled(WindowHints::WH_VSYNC))
            glfwSwapInterval(1);

        // Set Callbacks.
        glfwSetFramebufferSizeCallback(window_, &Graphics::handleFramebufferCallback);
        glfwSetWindowCloseCallback(window_, &Graphics::handleCloseCallback);

        inititalized_ = true;
    }

    void Graphics::maximize()
    {
        if (!inititalized_ || !window_)
            return;

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwRestoreWindow(window_);

        if (!isHintEnabled(WindowHints::WH_FULLSCREEN))
            glfwSetWindowSize(window_, mode->width, mode->height);
    }

    void Graphics::minimize()
    {
        if (!inititalized_ || !window_)
            return;

        glfwIconifyWindow(window_);
    }

    void Graphics::restore()
    {
        if (!inititalized_ || !window_)
            return;

        glfwRestoreWindow(window_);
    }

    void Graphics::hide()
    {
        if (!inititalized_ || !window_)
            return;

        glfwHideWindow(window_);
    }

    void Graphics::show()
    {
        if (!inititalized_ || !window_)
            return;

        glfwShowWindow(window_);
    }

    void Graphics::close()
    {
        if (!inititalized_ || !window_)
            return;

        hide();
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }


    void Graphics::toggleFullscreen()
    {
        if (!inititalized_ || !window_)
            return;

        hints_ ^= (glm::int32) WindowHints::WH_FULLSCREEN;

        setMode(size_.x, size_.y, samples_, hints_);
    }

    void Graphics::setMode(glm::int32 width, glm::int32 height, glm::int32 samples, glm::int32 hints)
    {
        if (!inititalized_ || !window_)
            return;

        if (hints_ != hints || samples_ != samples)
        {
            samples_ = samples;
            hints_ = hints;

            glfwWindowHint(GLFW_DECORATED, isHintEnabled(WindowHints::WH_BORDERLESS));
            glfwWindowHint(GLFW_RESIZABLE, isHintEnabled(WindowHints::WH_RESIZABLE));
            glfwWindowHint(GLFW_VISIBLE, isHintEnabled(WindowHints::WH_VISIBLE));
            glfwWindowHint(GLFW_SAMPLES, samples_);
            glfwWindowHint(GLFW_SRGB_CAPABLE, isHintEnabled(WindowHints::WH_SRGB));

            glfwMakeContextCurrent(NULL);
            glfwDestroyWindow(window_);

            if (isHintEnabled(WindowHints::WH_FULLSCREEN))
                window_ = glfwCreateWindow(width, height, title_.c_str(), glfwGetPrimaryMonitor(), NULL);
            else
                window_ = glfwCreateWindow(width, height, title_.c_str(), NULL, NULL);

            if (!window_)
            {
                LOGERROR("Failed to create window with params: [%i,%i,%i,%x]", width, height, samples_, hints);
                return;
            }

            glfwSetWindowUserPointer(window_, context_);
            glfwMakeContextCurrent(window_);

            int width, height;
            glfwGetFramebufferSize(window_, &width, &height);
            size_ = glm::ivec2(width, height);

            if (isHintEnabled(WindowHints::WH_VSYNC))
                glfwSwapInterval(1);

            glfwSetFramebufferSizeCallback(window_, &Graphics::handleFramebufferCallback);
            glfwSetWindowCloseCallback(window_, &Graphics::handleCloseCallback);

            sendEvent(DeviceResetEvent::getTypeStatic());
        }
        else
            setMode(width, height);
    }

    void Graphics::setMode(glm::int32 width, glm::int32 height)
    {
        if (!inititalized_ || !window_)
            return;

        glfwSetWindowSize(window_, width, height);
    }

    void Graphics::setGamma(glm::f32 gamma)
    {
        if (!inititalized_ || !window_)
            return;

        gamma_ = glm::max(gamma, 0.f);
        glfwSetGamma(glfwGetPrimaryMonitor(), gamma_);
    }

    void Graphics::setTitle(const std::string& title)
    {
        if (!inititalized_ || !window_)
            return;

        title_ = title_;
        glfwSetWindowTitle(window_, title_.c_str());
    }

    std::vector<glm::ivec2> Graphics::getResolutions() const
    {
        std::vector<glm::ivec2> ret;

        int count;
        const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
        for (int i = 0; i < count; i++)
        {
            glm::ivec2 mode(modes->width, modes->height);
            ret.push_back(mode);
        }

        return ret;
    }

    glm::ivec2 Graphics::getDesktopResolution() const
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (mode)
        {
            return glm::ivec2(mode->width, mode->height);
        }

        return glm::ivec2();
    }

    void Graphics::handleFramebufferCallback(GLFWwindow* window, glm::int32 width, glm::int32 height)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getGraphics();

        graphics->size_ = glm::ivec2(width, height);

        ScreenModeEvent* event = context->createEvent<ScreenModeEvent>();
        event->size = graphics->size_;
        graphics->sendEvent(ScreenModeEvent::getTypeStatic(), event);
    }

    void Graphics::handleCloseCallback(GLFWwindow* window)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getGraphics();

        graphics->sendEvent(ExitRequestedEvent::getTypeStatic());
    }

    bool Graphics::isHintEnabled(WindowHints hint) const
    {
        return (hints_ & (glm::int32) hint) == (glm::int32) hint;
    }
}