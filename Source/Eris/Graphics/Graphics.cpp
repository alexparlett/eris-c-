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
#include "EngineEvents.h"
#include "Graphics.h"
#include "GraphicsEvents.h"
#include "Log.h"

#include <glfw3.h>

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

    void Graphics::Initialize()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_DECORATED, IsHintEnabled(WH_DECORATED));
        glfwWindowHint(GLFW_RESIZABLE, IsHintEnabled(WH_RESIZABLE));
        glfwWindowHint(GLFW_VISIBLE, IsHintEnabled(WH_VISIBLE));
        glfwWindowHint(GLFW_SAMPLES, samples_);
        glfwWindowHint(GLFW_SRGB_CAPABLE, IsHintEnabled(WH_SRGB));

        if (IsHintEnabled(WH_FULLSCREEN))
            window_ = glfwCreateWindow(size_.x_, size_.y_, title_.CString(), glfwGetPrimaryMonitor(), NULL);
        else
            window_ = glfwCreateWindow(size_.x_, size_.y_, title_.CString(), NULL, NULL);

        if (!window_)
            return;

        glfwSetWindowUserPointer(window_, GetContext());

        glfwMakeContextCurrent(window_);

        if (IsHintEnabled(WH_VSYNC))
            glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(window_, &Graphics::HandleFramebufferCallback);
        glfwSetWindowCloseCallback(window_, &Graphics::HandleCloseCallback);

        inititalized_ = true;
    }

    void Graphics::Maximize()
    {
        if (!inititalized_ || !window_)
            return;

       const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

       glfwRestoreWindow(window_);
       glfwSetWindowSize(window_, mode->width, mode->height);
    }

    void Graphics::Minimize()
    {
        if (!inititalized_ || !window_)
            return;

        glfwIconifyWindow(window_);
    }

    void Graphics::Restore()
    {
        if (!inititalized_ || !window_)
            return;

        glfwRestoreWindow(window_);
    }

    void Graphics::Hide()
    {
        if (!inititalized_ || !window_)
            return;

        glfwHideWindow(window_);
    } 

    void Graphics::Show()
    {
        if (!inititalized_ || !window_)
            return;

        glfwShowWindow(window_);
    }

    void Graphics::Close()
    {
        if (!inititalized_ || !window_)
            return;

        Hide();
        glfwSetWindowShouldClose(window_, GL_TRUE);
    }


    void Graphics::ToggleFullscreen()
    {
        if (!inititalized_ || !window_)
            return;

        hints_ ^= WH_FULLSCREEN;

        SetMode(size_.x_, size_.y_, samples_, hints_);
    }

    void Graphics::SetMode(int width, int height, int samples, unsigned hints)
    {
        if (!inititalized_ || !window_)
            return;

        if (hints_ != hints || samples_ != samples)
        {
            samples_ = samples;
            hints_ = hints;

            glfwWindowHint(GLFW_DECORATED, IsHintEnabled(WH_DECORATED));
            glfwWindowHint(GLFW_RESIZABLE, IsHintEnabled(WH_RESIZABLE));
            glfwWindowHint(GLFW_VISIBLE, IsHintEnabled(WH_VISIBLE));
            glfwWindowHint(GLFW_SAMPLES, samples_);
            glfwWindowHint(GLFW_SRGB_CAPABLE, IsHintEnabled(WH_SRGB));

            glfwMakeContextCurrent(NULL);
            glfwDestroyWindow(window_);

            if (IsHintEnabled(WH_FULLSCREEN))
                window_ = glfwCreateWindow(width, height, title_.CString(), glfwGetPrimaryMonitor(), NULL);
            else
                window_ = glfwCreateWindow(width, height, title_.CString(), NULL, NULL);

            if (!window_)
            {
                LOGERROR("Failed to create window with params: [%i,%i,%i,%x]", width, height, samples_, hints);
                return;
            }

            glfwMakeContextCurrent(window_);
            SendEvent(E_DEVICERESET);
        }
        else
            SetMode(width, height);
    }

    void Graphics::SetMode(int width, int height)
    {
        if (!inititalized_ || !window_)
            return;

        glfwSetWindowSize(window_, width, height);
    }

    void Graphics::SetGamma(float gamma)
    {
        if (!inititalized_ || !window_)
            return;

        gamma_ = Max(gamma, 0.f);

        glfwSetGamma(glfwGetPrimaryMonitor(), gamma_);
    }

    void Graphics::SetTitle(const String& title)
    {
        if (!inititalized_ || !window_)
            return;

        title_ = title_;
        glfwSetWindowTitle(window_, title_.CString());
    }

    PODVector<IntVector2> Graphics::GetResolutions() const
    {
        PODVector<IntVector2> ret;

        int count;
        const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
        for (int i = 0; i < count; i++)
        {
            IntVector2 mode(modes->width, modes->height);
            ret.Push(mode);
        }

        return ret;
    }

    Eris::IntVector2 Graphics::GetDesktopResolution() const
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (mode)
        {
            return IntVector2(mode->width, mode->height);
        }

        return IntVector2::ZERO;
    }

    void Graphics::HandleFramebufferCallback(GLFWwindow* window, int width, int height)
    {
        using namespace ScreenMode;

        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->GetModule<Graphics>();

        graphics->size_ = IntVector2(width, height);

        VariantMap& eventData = context->GetEventDataMap();
        eventData[P_SIZE] = graphics->size_;
        graphics->SendEvent(E_SCREENMODE, eventData);
    }

    void Graphics::HandleCloseCallback(GLFWwindow* window)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->GetModule<Graphics>();

        graphics->SendEvent(E_EXITREQUESTED);
    }

    bool Graphics::IsHintEnabled(int hint) const
    {
        return (hints_ & hint) == hint;
    }
}