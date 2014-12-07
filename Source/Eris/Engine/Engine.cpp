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
#include "Log.h"

#include <glfw3.h>

#include "../gitversion.h"

namespace Eris
{

    Engine::Engine(Context* context) :
        Object(context),
        exitRequested_(false),
        autoExit_(true),
        audioPaused_(false),
        pauseMinimized_(true),
        initialized_(false),
        timeStep_(0.f)
    {
        context->RegisterModule(this);
        context->RegisterModule(new Time(context));
        context->RegisterModule(new Log(context));
        context->RegisterModule(new Graphics(context));
        context->RegisterModule(new Input(context));

        SubscribeToEvent(E_EXITREQUESTED, HANDLER(Engine, HandleExitRequest));
    }

    Engine::~Engine()
    {
        glfwTerminate();
    }

    bool Engine::Initialize(VariantMap parameters)
    {
        Log* log = context_->GetModule<Log>();
        Graphics* graphics = context_->GetModule<Graphics>();
        Input* input = context_->GetModule<Input>();

        log->Open(GetParameter(parameters, "Log", "Eris.log").GetString());

        if (!glfwInit())
            return false;

        IntVector2 resolution = GetParameter(parameters, "Resolution", graphics->GetDesktopResolution()).GetIntVector2();
        int samples = GetParameter(parameters, "Samples", 4).GetInt();
        String title = GetParameter(parameters, "Title", "Eris Engine").GetString();
        unsigned hints = 0;

        if (GetParameter(parameters, "Fullscreen", true).GetBool())
        {
            hints |= WH_FULLSCREEN;
        }
        else
        {
            if (GetParameter(parameters, "Resizable", false).GetBool())
                hints |= WH_RESIZABLE;
            if (GetParameter(parameters, "Borderless", false).GetBool())
                hints |= WH_BORDERLESS;
        }

        if (GetParameter(parameters, "Vsync", false).GetBool())
            hints |= WH_VSYNC;

        if (GetParameter(parameters, "SRGB", true).GetBool())
            hints |= WH_SRGB;

        if (GetParameter(parameters, "Visible", true).GetBool())
            hints |= WH_VISIBLE;

        graphics->Initialize(resolution, samples, title, hints);
        input->Initialize();

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
        return ERIS_VERSION;
    }

    void Engine::HandleExitRequest(const VariantMap& eventData, void* userData)
    {
        DoExit();
    }

    void Engine::HandleErrorCallback(int code, const char* msg)
    {
        LOGERROR("Error %i: %d", msg, code);
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

    const Variant Engine::GetParameter(const VariantMap& params, const StringHash& name, const Variant default)
    {
        VariantMap::ConstIterator iter = params.Find(name);
        if (iter != params.End())
            return iter->second_;
        return default;
    }

}