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

#include <vector>

#include <vec2.hpp>

struct GLFWwindow;

namespace Eris
{
    enum class WindowHints : glm::int32
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
    public:
        Graphics(Context* context);
        virtual ~Graphics();

        void initialize(const glm::ivec2& size, glm::int32 samples, const std::string& title, glm::int32 hints);
        void maximize();
        void minimize();
        void restore();
        void hide();
        void show();
        void close();

        void toggleFullscreen();

        void setMode(glm::int32 width, glm::int32 height, glm::int32 samples, glm::int32 hints);
        void setMode(glm::int32 width, glm::int32 height);
        void setGamma(glm::f32 gamma);
        void setTitle(const std::string& title);

        glm::int32 getWidth() const { return size_.x; }
        glm::int32 getHeight() const { return size_.y; }
        std::vector<glm::ivec2> getResolutions() const;
        glm::ivec2 getDesktopResolution() const;
        glm::int32 getHints() const { return hints_; }
        const std::string& GetTitle() const { return title_; }
        glm::int32 getSamples() const { return samples_; }
        glm::f32 getGamma() const { return gamma_; }

        bool isInitialized() const { return inititalized_; }
        bool isDecorated() const { return isHintEnabled(WindowHints::WH_BORDERLESS); }
        bool isResizable() const { return isHintEnabled(WindowHints::WH_RESIZABLE); }
        bool isVisible() const { return isHintEnabled(WindowHints::WH_VISIBLE); }
        bool isSRGB() const { return isHintEnabled(WindowHints::WH_SRGB); }
        bool isVSync() const { return isHintEnabled(WindowHints::WH_VSYNC); }
        bool isFullscreen() const { return isHintEnabled(WindowHints::WH_FULLSCREEN); }

    private:
        static void handleFramebufferCallback(GLFWwindow* window, glm::int32 width, glm::int32 height);
        static void handleCloseCallback(GLFWwindow* window);

        bool isHintEnabled(WindowHints hint) const;

        GLFWwindow* window_;
        bool inititalized_;
        glm::ivec2 size_;
        glm::int32 hints_;
        std::string title_;
        glm::int32 samples_;
        glm::f32 gamma_;
    };
}