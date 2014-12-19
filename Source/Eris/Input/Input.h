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

#include <unordered_set>
#include <memory>

#include <vec2.hpp>

#include "Object.h"

struct GLFWwindow;

namespace Eris
{
    class Graphics;

    enum class CursorMode : glm::int8_t
    {
        CM_NORMAL,
        CM_HIDDEN,
        CM_DISABLED
    };

    class Input : public Object
    {
    public:
        Input(Context* context);
        virtual ~Input();

        void initialize();
        void update();
        void resetState();

        void setCursorMode(CursorMode cm);
        CursorMode getCursorMode() const { return cursorMode_; }

        bool keyDown(glm::int32 key);
        bool keyPressed(glm::int32 key);
        bool scancodeDown(glm::int32 scancode);
        bool scancodePressed(glm::int32 scancode);
        bool modifierDown(glm::int32 modifier);
        bool modifierPressed(glm::int32 modifier);
        bool mouseButtonDown(glm::int32 button);
        bool mouseButtonPressed(glm::int32 button);
        glm::int32 modifiersDown();

        bool isFocused() const { return focused_; }
        bool isMinimized() const { return minimized_; }

    private:
        void handleBeginFrame(const Event* eventData);
        void handleScreenMode(const Event* eventData);

        static void handleFocusCallback(GLFWwindow* window, int focused);
        static void handleIconifiedCallback(GLFWwindow* window, int iconified);
        static void handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handleScrollCallback(GLFWwindow* window, double x, double y);
        static void handleCursorPosCallback(GLFWwindow* window, double x, double y);

        std::unordered_set<glm::int32> keyDown_;
        std::unordered_set<glm::int32> keyPress_;
        std::unordered_set<glm::int32> scancodeDown_;
        std::unordered_set<glm::int32> scancodePress_;
        glm::int32 mouseButtonDown_;
        glm::int32 mouseButtonPress_;
        double mouseMoveWheel_;
        glm::ivec2 mouseMove_;
        glm::ivec2 lastMousePosition_;
        CursorMode cursorMode_;
        bool initialized_;
        bool focused_;
        bool minimized_;
    };
}