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

#include "HashSet.h"
#include "Object.h"
#include "Ptr.h"

struct GLFWWindow;

namespace Eris
{
    class Graphics;

    enum CursorMode
    {
        CM_NORMAL,
        CM_HIDDEN,
        CM_DISABLED
    };

    class Input : public Object
    {
        OBJECT(Input)

    public:
        Input(Context* context);
        virtual ~Input();

        void Initialize();
        void Update();
        void ResetState();

        void SetCursorMode(CursorMode cm);

        CursorMode GetCursorMode() const { return cursorMode_; }

        bool KeyDown(int key);
        bool KeyPressed(int key);
        bool ScancodeDown(int scancode);
        bool ScancodePressed(int scancode);
        bool ModifierDown(int modifier);
        bool ModifierPressed(int modifier);
        bool MouseButtonDown(int button);
        bool MouseButtonPressed(int button);
        int ModifiersDown();

        bool IsFocused() const { return focused_; }
        bool IsMinimized() const { return minimized_; }

    private:
        void HandleBeginFrame(const VariantMap& eventData, void* userData);
        void HandleScreenMode(const VariantMap& eventData, void* userData);

        static void HandleFocusCallback(GLFWwindow* window, int focused);
        static void HandleIconifiedCallback(GLFWwindow* window, int iconified);
        static void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void HandleScrollCallback(GLFWwindow* window, double x, double y);
        static void HandleCursorPosCallback(GLFWwindow* window, double x, double y);

        WeakPtr<Graphics> graphics_;
        HashSet<int> keyDown_;
        HashSet<int> keyPress_;
        HashSet<int> scancodeDown_;
        HashSet<int> scancodePress_;
        unsigned mouseButtonDown_;
        unsigned mouseButtonPress_;
        double mouseMoveWheel_;
        IntVector2 mouseMove_;
        IntVector2 lastMousePosition_;
        CursorMode cursorMode_;
        bool initialized_;
        bool focused_;
        bool minimized_;
    };
}