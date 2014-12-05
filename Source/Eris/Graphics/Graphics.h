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

#include "GraphicsImpl.h"
#include "Object.h"

namespace Eris
{
    enum WindowHints
    {
        WH_DECORATED = 1,
        WH_RESIZABLE = 2,
        WH_VISIBLE = 4,
        WH_SRGB = 8,
        WH_VSYNC = 16,
        WH_FULLSCREEN = 32
    };

    class Graphics : public Object
    {
        OBJECT(Graphics)

    public:
        Graphics(Context* context);
        virtual ~Graphics();

        void Initialize();
        void Maximize();
        void Minimize();
        void Restore();
        void Hide();
        void Show();
        void Close();

        void SetMode(int width, int height, int samples, unsigned flags);
        void SetMode(int width, int height);

        void SetGamma(float gamma);
        void SetTitle(const String& title);

        GraphicsImpl* GetImpl() const { return impl_; }
        int GetWidth() const { return size_.x_; }
        int GetHeight() const { return size_.y_; }
        PODVector<IntVector2> GetResolutions() const;
        IntVector2 GetDesktopResolution() const;
        unsigned GetHints() const { return hints_; }
        String GetTitle() const { return title_; }
        int GetSamples() const { return samples_; }
        float GetGamma() const { return gamma_; }

        bool IsInitialized() const { return inititalized_; }
        bool IsDecorated() const { return (hints_ & WH_DECORATED) != 0; }
        bool IsResizable() const { return (hints_ & WH_RESIZABLE) != 0; }
        bool IsVisible() const { return (hints_ & WH_VISIBLE) != 0; }
        bool IsSRGB() const { return (hints_ & WH_SRGB) != 0; }
        bool IsVSync() const { return (hints_ & WH_VSYNC) != 0; }
        bool IsFullscreen() const { return (hints_ & WH_FULLSCREEN) != 0; }

    private:
        static void HandleFramebufferCallback(GLFWwindow* window, int width, int height);

        UniquePtr<GraphicsImpl> impl_;
        bool inititalized_;
        IntVector2 size_;
        unsigned hints_;
        String title_;
        int samples_;
        float gamma_;
    };
}