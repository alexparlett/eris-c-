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

#include "Engine.h"

#include "Core/Clock.h"
#include "Core/Log.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "IO/FileSystem.h"
#include "Util/Functions.h"

namespace Eris
{
    Engine::Engine(Context* context) :
        Object(context),
        m_exitcode(EXIT_OK)
    {
        context->registerModule<Log>(new Log(context));
        context->registerModule<Engine>(this);
        context->registerModule<FileSystem>(new FileSystem(context));
        context->registerModule<Clock>(new Clock(context));
        context->registerModule<Graphics>(new Graphics(context));
        context->registerModule<Input>(new Input(context));
    }

    Engine::~Engine()
    {
    }

    void Engine::initialize()
    {
        Log* log = m_context->getModule<Log>();
        log->open("log.log");

        if (!glfwInit())
        {
            setExitCode(EXIT_GLFW_INIT_ERROR);
            return;
        }

        Graphics* graphics = m_context->getModule<Graphics>();
        graphics->setSize(800, 600);
        graphics->setFullscreen(false);
        graphics->initialize();

        Input* input = m_context->getModule<Input>();
        input->initialize();
    }

    void Engine::run()
    {
        Graphics* graphics = m_context->getModule<Graphics>();
        Clock* clock = m_context->getModule<Clock>();
        FileSystem* fs = m_context->getModule<FileSystem>();

        while (!glfwWindowShouldClose(graphics->getWindow()))
        {
            clock->beginFrame(0.f);

            glfwSwapBuffers(graphics->getWindow());

            clock->endFrame();
            m_context->resetFrameAllocator();
        }
    }

    void Engine::terminate()
    {
        Graphics* graphics = m_context->getModule<Graphics>();
        Clock* clock = m_context->getModule<Clock>();

        graphics->terminate();

        glm::f32 duration = clock->getElapsedTime() / 1000.f;
        glm::u64 frames = clock->getFrameNumber();

        glfwTerminate();

        Log::rawf("Shutting down, ran for %d frames | %.2f seconds", frames, duration);
    }

    void Engine::setExitCode(glm::i32 exitcode)
    {
        m_exitcode = exitcode;
    }
}
