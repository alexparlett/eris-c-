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

#include "CoreEvents.h"
#include "Context.h"
#include "Graphics.h"
#include "GraphicsEvents.h"
#include "Input.h"
#include "InputEvents.h"

#include <glfw3.h>

namespace Eris
{

    Input::Input(Context* context) :
        Object(context),
        mouseButtonDown_(0),
        mouseButtonPress_(0),
        mouseMoveWheel_(0),
        mouseMove_(IntVector2::ZERO),
        cursorMode_(CM_NORMAL),
        focused_(false),
        minimized_(false)
    {
        SubscribeToEvent(E_SCREENMODE, HANDLER(Input, HandleScreenMode));
        Initialize();
    }

    Input::~Input()
    {

    }

    void Input::Update()
    {
        assert(initialized_);

        ResetState();

        glfwPollEvents();
    }

    void Input::SetCursorMode(CursorMode cm)
    {
        cursorMode_ = cm;

        if (!graphics_ || !graphics_->IsInitialized())
            return;

        GLFWwindow* window = graphics_->GetImpl()->GetWindow();

        switch (cm)
        {
        case Eris::CM_NORMAL:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case Eris::CM_HIDDEN:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case Eris::CM_DISABLED:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }

    bool Input::KeyDown(int key)
    {
        keyDown_.Contains(key);
    }

    bool Input::KeyPressed(int key)
    {
        keyPress_.Contains(key);
    }

    bool Input::ScancodeDown(int scancode)
    {
        scancodeDown_.Contains(scancode);
    }

    bool Input::ScancodePressed(int scancode)
    {
        scancodePress_.Contains(scancode);
    }

    bool Input::ModifierDown(int modifier)
    {
        if (modifier == MOD_ALT)
            return keyDown_.Contains(KEY_LEFT_ALT) || keyDown_.Contains(KEY_RIGHT_ALT);
        else if (modifier == MOD_CONTROL)
            return keyDown_.Contains(KEY_LEFT_CONTROL) || keyDown_.Contains(KEY_RIGHT_CONTROL);
        else if (modifier == MOD_SHIFT)
            return keyDown_.Contains(KEY_LEFT_SHIFT) || keyDown_.Contains(KEY_RIGHT_SHIFT);
        else if (modifier == MOD_SUPER)
            return keyDown_.Contains(KEY_LEFT_SUPER) || keyDown_.Contains(KEY_RIGHT_SUPER);
    }

    bool Input::ModifierPressed(int modifier)
    {
        if (modifier == MOD_ALT)
            return keyPress_.Contains(KEY_LEFT_ALT) || keyPress_.Contains(KEY_RIGHT_ALT);
        else if (modifier == MOD_CONTROL)
            return keyPress_.Contains(KEY_LEFT_CONTROL) || keyPress_.Contains(KEY_RIGHT_CONTROL);
        else if (modifier == MOD_SHIFT)
            return keyPress_.Contains(KEY_LEFT_SHIFT) || keyPress_.Contains(KEY_RIGHT_SHIFT);
        else if (modifier == MOD_SUPER)
            return keyPress_.Contains(KEY_LEFT_SUPER) || keyPress_.Contains(KEY_RIGHT_SUPER);
    }

    bool Input::MouseButtonDown(int button)
    {
        return (mouseButtonDown_ & button) != 0;
    }

    bool Input::MouseButtonPressed(int button)
    {
        return (mouseButtonPress_ & button) != 0;
    }

    int Input::ModifiersDown()
    {
        int ret = 0;

        if (ModifierDown(MOD_ALT))
            ret |= MOD_ALT;
        if (ModifierDown(MOD_CONTROL))
            ret |= MOD_CONTROL;
        if (ModifierDown(MOD_SHIFT))
            ret |= MOD_SHIFT;
        if (ModifierDown(MOD_SUPER))
            ret |= MOD_SUPER;

        return ret;
    }

    void Input::Initialize()
    {
        Graphics* graphics = context_->GetModule<Graphics>();
        if (!graphics || !graphics->IsInitialized())
            return;

        graphics_ = graphics;

        ResetState();

        SubscribeToEvent(E_BEGINFRAME, HANDLER(Input, HandleBeginFrame));

        GLFWwindow* window = graphics_->GetImpl()->GetWindow();

        SetCursorMode(cursorMode_);

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);

        glfwSetWindowFocusCallback(window, &Input::HandleFocusCallback);
        glfwSetWindowIconifyCallback(window, &Input::HandleIconifiedCallback);
        glfwSetMouseButtonCallback(window, &Input::HandleMouseButtonCallback);
        glfwSetKeyCallback(window, &Input::HandleKeyCallback);
        glfwSetScrollCallback(window, &Input::HandleScrollCallback);
        glfwSetCursorPosCallback(window, &Input::HandleCursorPosCallback);        
        
        initialized_ = true;
    }

    void Input::ResetState()
    {
        keyDown_.Clear();
        keyPress_.Clear();
        scancodeDown_.Clear();
        scancodePress_.Clear();

        mouseMove_ = IntVector2::ZERO;
        mouseMoveWheel_ = 0;
        mouseButtonPress_ = 0;
        mouseButtonDown_ = 0;
    }

    void Input::HandleScreenMode(const VariantMap& eventData, void* userData)
    {
        if (!initialized_)
            Initialize();
        else
            ResetState();

        GLFWwindow* window = graphics_->GetImpl()->GetWindow();

        minimized_ = glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GL_TRUE;
    }

    void Input::HandleBeginFrame(const VariantMap& eventData, void* userData)
    {
        Update();
    }

    void Input::HandleFocusCallback(GLFWwindow* window, int focused)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        input->focused_ = focused == GL_TRUE;
    }

    void Input::HandleIconifiedCallback(GLFWwindow* window, int iconified)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        input->minimized_ = iconified == GL_TRUE;
    }

    void Input::HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        if (action == GLFW_PRESS)
        {
            input->mouseButtonDown_ |= button;
            input->mouseButtonPress_ |= button;

            using namespace MouseButtonPressed;

            VariantMap& eventData = context->GetEventDataMap();
            eventData[P_BUTTON] = button;
            eventData[P_BUTTONS] = input->mouseButtonDown_;
            eventData[P_MODIFIERS] = mods;
            input->SendEvent(E_MOUSE_BUTTON_PRESSED, eventData);
        }
        else
        {
            input->mouseButtonDown_ &= ~button;

            using namespace MouseButtonReleased;

            VariantMap& eventData = context->GetEventDataMap();
            eventData[P_BUTTON] = button;
            eventData[P_BUTTONS] = input->mouseButtonDown_;
            eventData[P_MODIFIERS] = mods;
            input->SendEvent(E_MOUSE_BUTTON_RELEASED, eventData);
        }
    }

    void Input::HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        if (action == GLFW_PRESS)
        {
            input->keyDown_.Insert(key);
            input->keyPress_.Insert(key);
            input->scancodeDown_.Insert(scancode);
            input->scancodePress_.Insert(scancode);

            using namespace KeyPressed;

            VariantMap& eventData = context->GetEventDataMap();
            eventData[P_KEY] = key;
            eventData[P_SCANCODE] = scancode;
            eventData[P_BUTTONS] = input->mouseButtonDown_;
            eventData[P_MODIFIERS] = mods;
            eventData[P_REPEAT] = false;
            input->SendEvent(E_KEY_PRESSED, eventData);
        }
        else if (action == GLFW_REPEAT)
        {
            input->keyDown_.Insert(key);
            input->keyPress_.Insert(key);
            input->scancodeDown_.Insert(scancode);
            input->scancodePress_.Insert(scancode);

            using namespace KeyPressed;

            VariantMap& eventData = context->GetEventDataMap();
            eventData[P_KEY] = key;
            eventData[P_SCANCODE] = scancode;
            eventData[P_BUTTONS] = input->mouseButtonDown_;
            eventData[P_MODIFIERS] = mods;
            eventData[P_REPEAT] = true;
            input->SendEvent(E_KEY_PRESSED, eventData);
        }
        else
        {
            input->keyDown_.Erase(key);
            input->scancodeDown_.Erase(scancode);

            using namespace KeyReleased;

            VariantMap& eventData = context->GetEventDataMap();
            eventData[P_KEY] = key;
            eventData[P_SCANCODE] = scancode;
            eventData[P_BUTTONS] = input->mouseButtonDown_;
            eventData[P_MODIFIERS] = mods;
            input->SendEvent(E_KEY_RELEASED, eventData);
        }
    }

    void Input::HandleScrollCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        double amount = x > y ? x : -y;

        input->mouseMoveWheel_ += amount;

        using namespace MouseScroll;

        VariantMap& eventData = context->GetEventDataMap();
        eventData[P_AMOUNT] = amount;
        eventData[P_BUTTONS] = input->mouseButtonDown_;
        eventData[P_MODIFIERS] = input->ModifiersDown();
        input->SendEvent(E_MOUSE_SCROLL, eventData);
    }

    void Input::HandleCursorPosCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->GetModule<Input>();

        IntVector2 position = IntVector2(x, y);
        IntVector2 relative = input->lastMousePosition_ - position;

        input->lastMousePosition_ = position;
        input->mouseMove_ += relative;

        using namespace MouseMove;

        VariantMap& eventData = context->GetEventDataMap();
        eventData[P_POSITION] = position;
        eventData[P_RELATIVE] = relative;
        eventData[P_BUTTONS] = input->mouseButtonDown_;
        eventData[P_MODIFIERS] = input->ModifiersDown();
        input->SendEvent(E_MOUSE_MOVE, eventData);
    }

}