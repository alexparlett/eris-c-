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

#include "Engine.h"
#include "Events.h"

#include "Core/Clock.h"
#include "Core/Log.h"
#include "Collections/Functions.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "IO/FileSystem.h"
#include "Resource/ResourceCache.h"

#include "../gitversion.h"

namespace Eris
{
    Engine::Engine(Context* context) :
        Object(context),
        m_exitcode(EXIT_OK),
        m_exiting(false)
    {        
        context->registerModule<Clock>(new Clock(context));
        context->registerModule<Engine>(this);
        context->registerModule<FileSystem>(new FileSystem(context));
        context->registerModule<Graphics>(new Graphics(context));
        context->registerModule<Input>(new Input(context));
        context->registerModule<Log>(new Log(context));
        context->registerModule<ResourceCache>(new ResourceCache(context));

        subscribeToEvent(ExitRequestedEvent::getTypeStatic(), HANDLER(Engine, HandleExitRequest));
    }

    Engine::~Engine()
    {
    }

    void Engine::initialize()
    {
        FileSystem* fs = m_context->getModule<FileSystem>();
        Log* log = m_context->getModule<Log>();
        log->open(fs->getApplicationPreferencesDir() /= "log.log");

        Log::rawf("Initializing - Version %s.", std::string_upper(getVersion()).c_str());

        fs->addPath(fs->getApplicationPreferencesDir());
        fs->addPath(fs->getDocumentsDir());
        fs->addPath(fs->getProgramDir());

        ResourceCache* rc = m_context->getModule<ResourceCache>();
        rc->addDirectory(fs->getProgramDir() /= "Data");

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
        Input* input = m_context->getModule<Input>();

        glm::f32 current_time = clock->getElapsedTime();

        while (!m_exiting)
        {
            glm::f32 new_time = clock->getElapsedTime();
            glm::f32 elapsed = new_time - current_time;
            current_time = new_time;

            clock->beginFrame(elapsed);

            UpdateEvent* event = m_context->getFrameAllocator().newInstance<UpdateEvent>();
            event->time_step = elapsed;

            sendEvent(UpdateEvent::getTypeStatic(), event);
            sendEvent(PostUpdateEvent::getTypeStatic());

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

        Log::rawf("Terminating - Ran for %d frames over %.2f seconds.", frames, duration);
    }

    const char* Engine::getVersion() const
    {
        return ERIS_VERSION;
    }

    void Engine::setExitCode(glm::i32 exitcode)
    {
        m_exitcode = exitcode;
    }

    void Engine::HandleExitRequest(const StringHash& type, const Event* event)
    {
        m_exiting = true;
    }

}
