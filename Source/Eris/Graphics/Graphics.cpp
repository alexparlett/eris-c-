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

#include "Core/Context.h"
#include "Graphics.h"

namespace Eris
{
    Graphics::Graphics(Context* context) :
        Object(context),
        mInitialized(false),
        mFullscreen(true),
        mBorderless(false),
        mResizable(false),
        mVSync(true),
        mWidth(0),
        mHeight(0),
        mSamples(4),
        mGamma(1.0f),
        mTitle("Eris"),
        mWindow(nullptr)
    {
    }

    Graphics::~Graphics()
    {
    }

    void Graphics::initialize()
    {
        if (mInitialized)
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

        glfwWindowHint(GLFW_DECORATED, !mBorderless);
        glfwWindowHint(GLFW_RESIZABLE, mResizable);
        glfwWindowHint(GLFW_SAMPLES, mSamples);

        if (mFullscreen)
            mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
        else
            mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);

        if (!mWindow)
        {
            mContext->setExitCode(EXIT_WINDOW_CREATE_ERROR);
            return;
        }
        
        glfwMakeContextCurrent(mWindow);
        glfwSetWindowUserPointer(mWindow, mContext);

        glm::i32 width, height;
        glfwGetFramebufferSize(mWindow, &width, &height);
        mWidth = width;
        mHeight = height;

        if (mVSync)
            glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            const GLubyte* msg = glewGetErrorString(err);

            glfwDestroyWindow(mWindow);
            mWindow = nullptr;

            mContext->setExitCode(EXIT_GLEW_INIT_ERROR);
            return;
        }

        glfwSetFramebufferSizeCallback(mWindow, &Graphics::handleFramebufferCallback);
        glfwSetWindowCloseCallback(mWindow, &Graphics::handleCloseCallback);

        mInitialized = true;
    }

    void Graphics::terminate()
    {
        mInitialized = false;

        if (mWindow)
        {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
        }
    }

    void Graphics::maximize()
    {
        if (!mInitialized || !mWindow)
            return;

        if (!mFullscreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowSize(mWindow, mode->width, mode->height);
        }

        glfwRestoreWindow(mWindow);
    }

    void Graphics::minimize()
    {
        if (!mInitialized || !mWindow)
            return;

        glfwIconifyWindow(mWindow);
    }

    void Graphics::restore()
    {
        if (!mInitialized || !mWindow)
            return;

        glfwRestoreWindow(mWindow);
    }

    void Graphics::hide()
    {
        if (!mInitialized || !mWindow)
            return;

        glfwHideWindow(mWindow);
    }

    void Graphics::show()
    {
        if (!mInitialized || !mWindow)
            return;

        glfwShowWindow(mWindow);
    }

    void Graphics::close()
    {
        if (!mInitialized || !mWindow)
            return;

        glfwSetWindowShouldClose(mWindow, GL_TRUE);
    }

    void Graphics::setSize(glm::i32 width, glm::i32 height)
    {
        mWidth = width;
        mHeight = height;

        if (!mInitialized || !mWindow)
            return;

        glfwSetWindowSize(mWindow, mWidth, mHeight);
    }

    void Graphics::setSamples(glm::i32 samples)
    {
        mSamples = samples;
    }

    void Graphics::setGamma(glm::f32 gamma)
    {
        mGamma = gamma;

        if (!mInitialized || !mWindow)
            return;

        glfwSetGamma(glfwGetWindowMonitor(mWindow), mGamma);
    }

    void Graphics::setTitle(const std::string& title)
    {
        mTitle = title;

        if (!mInitialized || !mWindow)
            return;

        glfwSetWindowTitle(mWindow, mTitle.c_str());
    }

    void Graphics::setFullscreen(bool fullscreen)
    {
        mFullscreen = fullscreen;
    }

    void Graphics::setResizable(bool resizable)
    {
        mResizable = !mFullscreen && resizable;
    }

    void Graphics::setBorderless(bool borderless)
    {
        mBorderless = !mFullscreen && borderless;
    }

    void Graphics::setVSync(bool vsync)
    {
        mVSync = vsync;
    }

    void Graphics::handleFramebufferCallback(GLFWwindow* window, glm::i32 width, glm::i32 height)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Graphics* graphics = context->getModule<Graphics>();
        graphics->mWidth = width;
        graphics->mHeight = height;
    }

    void Graphics::handleCloseCallback(GLFWwindow* window)
    {

    }
}
