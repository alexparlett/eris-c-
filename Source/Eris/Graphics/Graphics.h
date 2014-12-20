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

#include "Core/Object.h"

namespace Eris
{
    class Graphics : public Object
    {
    public:
        /// Ctor.
        Graphics(Context* context);
        /// Detor.
        virtual ~Graphics();

        /// Initialize graphics module. Creates window and initializes glew.
        void initialize();
        /// Terminate graphics module. Destory the window if created.
        void terminate();

        /// Maximize active window.
        void maximize();
        /// Minimize active window.
        void minimize();
        /// Restore active window.
        void restore();
        /// Hide active window.
        void hide();
        /// Show active window.
        void show();
        /// Close active window.
        void close();

        /// Set window size. Resizes window if active.
        void setSize(glm::i32 width, glm::i32 height);
        /// Set window samples. Only effective before window initialization.
        void setSamples(glm::i32 samples);
        /// Set window gamma. Changes for window if active.
        void setGamma(glm::f32 gamma);
        /// Set window title. Changes for window if active.
        void setTitle(const std::string& title);
        /// Set window fullscreen. Only effective before window initialization.
        void setFullscreen(bool fullscreen);
        /// Set window resizable. Only effective before window initialization.
        void setResizable(bool resizable);
        /// Set window borderless. Only effective before window initialization.
        void setBorderless(bool borderless);
        /// Set window vsync. Only effective before window initialization.
        void setVSync(bool vsync);

        /// Get whether fullscreen is set.
        bool getFullscreen() const { return mFullscreen; }
        /// Get whether resizable is set.
        bool getResizable() const { return mResizable; }
        /// Get whether borderless is set.
        bool getBorderless() const { return mBorderless; }
        /// Get whether vsync is set.
        bool getVSync() const { return mVSync; }
        /// Get width.
        glm::i32 getWidth() const { return mWidth; }
        /// Get height.
        glm::i32 getHeight() const { return mHeight; }
        /// Get samples.
        glm::i32 getSamples() const { return mSamples; }
        /// Get gamma.
        glm::f32 getGamma() const { return mGamma; }
        /// Get title.
        const std::string& getTitle() const { return mTitle; }
        /// Get window ptr.
        GLFWwindow* getWindow() const { return mWindow; }

        /// Get initialized.
        bool isInitialize() const { return mInitialized; }

    private:
        static void handleFramebufferCallback(GLFWwindow* window, glm::i32 width, glm::i32 height);
        static void handleCloseCallback(GLFWwindow* window);

        bool mInitialized;
        bool mFullscreen;
        bool mResizable;
        bool mBorderless;
        bool mVSync;
        glm::i32 mWidth;
        glm::i32 mHeight;
        glm::i32 mSamples;
        glm::f32 mGamma;
        std::string mTitle;
        GLFWwindow* mWindow;
    };
}
