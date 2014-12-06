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

#pragma once

#include "Object.h"
#include "Variant.h"
#include "Vector.h"
#include "Timer.h"

namespace Eris
{

    class Engine : public Object
    {
        OBJECT(Engine)

    public:
        Engine(Context* context);
        virtual ~Engine();

        bool Initialize(VariantMap parameters);
        void RunFrame();
        void Update();
        void Render();
        void ApplyFrameLimit();
        void Exit();

        void SetNextTimeStep(float seconds);
        void SetMinFPS(unsigned fps);
        void SetMaxFPS(unsigned fps);
        void SetMaxInactiveFPS(unsigned fps);
        void SetAutoExit(bool enabled);
        void SetPauseMinimized(bool enabled);

        float GetNextTimeStep() const { return timeStep_; }
        unsigned GetMinFPS() const { return minFps_; }
        unsigned GetMaxFPS() const { return maxFps_; }
        unsigned GetMaxInactiveFPS() const { return maxInactiveFps_; }
        bool GetAutoExit() const { return autoExit_; }
        bool GetPauseMinimized() const { return pauseMinimized_; }
        const char* GetVersion() const;

        bool IsExiting() const { return exitRequested_; }
        bool IsInitialized() const { return initialized_; }

    private:
        void HandleExitRequest(const VariantMap& eventData, void* userData);

        static void HandleErrorCallback(int code, const char* msg);

        void DoExit();

        Timer frameTimer_;
        PODVector<float> lastTimeSteps_;
        bool exitRequested_;
        bool initialized_;
        bool pauseMinimized_;
        bool autoExit_;
        bool audioPaused_;
        float timeStep_;
        unsigned timeStepSmoothing_;
        unsigned minFps_;
        unsigned maxFps_;
        unsigned maxInactiveFps_;
    };

}