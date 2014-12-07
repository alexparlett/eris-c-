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
#include "Ptr.h"

struct GLFWwindow;

namespace Eris
{
    enum WindowHints
    {
        WH_BORDERLESS = 1,
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

        void Initialize(const IntVector2& size, int samples, const String& title, unsigned hints);
        void Maximize();
        void Minimize();
        void Restore();
        void Hide();
        void Show();
        void Close();

        void ToggleFullscreen();

        void SetMode(int width, int height, int samples, unsigned hints);
        void SetMode(int width, int height);
        void SetGamma(float gamma);
        void SetTitle(const String& title);

        int GetWidth() const { return size_.x_; }
        int GetHeight() const { return size_.y_; }
        PODVector<IntVector2> GetResolutions() const;
        IntVector2 GetDesktopResolution() const;
        unsigned GetHints() const { return hints_; }
        String GetTitle() const { return title_; }
        int GetSamples() const { return samples_; }
        float GetGamma() const { return gamma_; }

        bool IsInitialized() const { return inititalized_; }
        bool IsDecorated() const { return IsHintEnabled(WH_BORDERLESS); }
        bool IsResizable() const { return IsHintEnabled(WH_RESIZABLE); }
        bool IsVisible() const { return IsHintEnabled(WH_VISIBLE); }
        bool IsSRGB() const { return IsHintEnabled(WH_SRGB); }
        bool IsVSync() const { return IsHintEnabled(WH_VSYNC); }
        bool IsFullscreen() const { return IsHintEnabled(WH_FULLSCREEN); }

    private:
        static void HandleFramebufferCallback(GLFWwindow* window, int width, int height);
        static void HandleCloseCallback(GLFWwindow* window);

        bool IsHintEnabled(int hint) const;

        GLFWwindow* window_;
        bool inititalized_;
        IntVector2 size_;
        unsigned hints_;
        String title_;
        int samples_;
        float gamma_;
    };
}