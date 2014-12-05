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
#include "Engine.h"
#include "Graphics.h"
#include "EngineEvents.h"
#include "Input.h"

#include <glfw3.h>

#include "../gitversion.h"

namespace Eris
{

    Engine::Engine(Context* context) :
        Object(context),
        exitRequested_(false)
    {
        context->RegisterModule(this);
        context->RegisterModule(new Input(context));
        context->RegisterModule(new Time(context));
        context->RegisterModule(new Graphics(context));

        SubscribeToEvent(E_EXITREQUESTED, HANDLER(Engine, HandleExitRequest));
    }

    Engine::~Engine()
    {
        glfwTerminate();
    }

    bool Engine::Initialize(VariantMap parameters)
    {
        if (!glfwInit())
            return false;

        initialized_ = true;
        return true;
    }

    void Engine::RunFrame()
    {
        assert(initialized_);

        if (exitRequested_)
            return;

        Time* time = context_->GetModule<Time>();
        Input* input = context_->GetModule<Input>();

        time->BeginFrame(timeStep_);

        if (pauseMinimized_ && input->IsMinimized())
        {

        }
        else
        {
            if (audioPaused_)
            {
                audioPaused_ = false;
            }

            Update();
        }

        Render();
        ApplyFrameLimit();

        time->EndFrame();
    }

    void Engine::Update()
    {

    }

    void Engine::Render()
    {

    }

    void Engine::ApplyFrameLimit()
    {

    }

    void Engine::Exit()
    {
        DoExit();
    }

    void Engine::SetNextTimeStep(float seconds)
    {
        timeStep_ = Max(seconds, 0.0f);
    }

    void Engine::SetMinFPS(unsigned fps)
    {
        minFps_ = Max(fps, 0);
    }

    void Engine::SetMaxFPS(unsigned fps)
    {
        maxFps_ = Max(fps, 0);
    }

    void Engine::SetMaxInactiveFPS(unsigned fps)
    {
        maxInactiveFps_ = Max(fps, 0);
    }

    void Engine::SetAutoExit(bool enabled)
    {
        autoExit_ = enabled;
    }

    void Engine::SetPauseMinimized(bool enabled)
    {
        pauseMinimized_ = enabled;
    }

    const char* Engine::GetVersion() const
    {
        return version;
    }

    void Engine::HandleExitRequest(const VariantMap& eventData, void* userData)
    {
        DoExit();
    }

    void Engine::DoExit()
    {
        if (GetAutoExit())
        {
            Eris::Graphics* graphics = context_->GetModule<Graphics>();
            graphics->Close();

            exitRequested_ = true;
        }
    }

}