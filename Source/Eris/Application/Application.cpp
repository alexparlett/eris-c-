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

#include "Application.h"

#include "Core/Context.h"
#include "Graphics/Graphics.h"

namespace Eris
{
    Application::Application(Context* context) :
        Object(context)
    {
        context->registerModule<Application>(this);
        context->registerModule<Graphics>(new Graphics(context));
    }

    Application::~Application()
    {
    }

    void Application::initialize()
    {
        if (!glfwInit())
        {
            mContext->setExitCode(-1);
            return;
        }

        Graphics* graphics = mContext->getModule<Graphics>();
        graphics->setSize(800, 600);
        graphics->setFullscreen(false);
        graphics->initialize();
    }

    void Application::run()
    {
        Graphics* graphics = mContext->getModule<Graphics>();

        while (!glfwWindowShouldClose(graphics->getWindow()))
        {
            glfwPollEvents();
            glfwSwapBuffers(graphics->getWindow());
        }
    }

    void Application::terminate()
    {
        Graphics* graphics = mContext->getModule<Graphics>();

        graphics->terminate();

        glfwTerminate();
    }
}
