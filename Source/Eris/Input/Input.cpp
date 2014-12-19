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

#include <cassert>

namespace Eris
{

    Input::Input(Context* context) :
        Object(context),
        mouseButtonDown_(0),
        mouseButtonPress_(0),
        mouseMoveWheel_(0),
        mouseMove_(glm::ivec2(0,0)),
        cursorMode_(CursorMode::CM_NORMAL),
        focused_(false),
        minimized_(false)
    {
    }

    Input::~Input()
    {

    }

    void Input::update()
    {
        assert(initialized_);
        resetState();
        glfwPollEvents();
    }

    void Input::setCursorMode(CursorMode cm)
    {
        cursorMode_ = cm;

        GLFWwindow* window = glfwGetCurrentContext();
        if (!window)
            return;

        switch (cm)
        {
        case CursorMode::CM_NORMAL:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorMode::CM_HIDDEN:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case CursorMode::CM_DISABLED:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }

    bool Input::keyDown(glm::int32 key)
    {
        return keyDown_.find(key) != keyDown_.end();
    }

    bool Input::keyPressed(glm::int32 key)
    {
        return keyPress_.find(key) != keyPress_.end();
    }

    bool Input::scancodeDown(glm::int32 scancode)
    {
        return scancodeDown_.find(scancode) != scancodeDown_.end();
    }

    bool Input::scancodePressed(glm::int32 scancode)
    {
        return scancodePress_.find(scancode) != scancodePress_.end();
    }

    bool Input::modifierDown(glm::int32 modifier)
    {
        if (modifier == MODIFIER_ALT)
            return keyDown_.find(KEY_LEFT_ALT) != keyDown_.end() || keyDown_.find(KEY_RIGHT_ALT) != keyDown_.end();
        else if (modifier == MODIFIER_CONTROL)
            return keyDown_.find(KEY_LEFT_CONTROL) != keyDown_.end() || keyDown_.find(KEY_RIGHT_CONTROL) != keyDown_.end();
        else if (modifier == MODIFIER_SHIFT)
            return keyDown_.find(KEY_LEFT_SHIFT) != keyDown_.end() || keyDown_.find(KEY_RIGHT_SHIFT) != keyDown_.end();
        else if (modifier == MODIFIER_SUPER)
            return keyDown_.find(KEY_LEFT_SUPER) != keyDown_.end() || keyDown_.find(KEY_RIGHT_SUPER) != keyDown_.end();

        return false;
    }

    bool Input::modifierPressed(glm::int32 modifier)
    {
        if (modifier == MODIFIER_ALT)
            return keyPress_.find(KEY_LEFT_ALT) != keyPress_.end() || keyPress_.find(KEY_RIGHT_ALT) != keyPress_.end();
        else if (modifier == MODIFIER_CONTROL)
            return keyPress_.find(KEY_LEFT_CONTROL) != keyPress_.end() || keyPress_.find(KEY_RIGHT_CONTROL) != keyPress_.end();
        else if (modifier == MODIFIER_SHIFT)
            return keyPress_.find(KEY_LEFT_SHIFT) != keyPress_.end() || keyPress_.find(KEY_RIGHT_SHIFT) != keyPress_.end();
        else if (modifier == MODIFIER_SUPER)
            return keyPress_.find(KEY_LEFT_SUPER) != keyPress_.end() || keyPress_.find(KEY_RIGHT_SUPER) != keyPress_.end();

        return false;
    }

    bool Input::mouseButtonDown(glm::int32 button)
    {
        return (mouseButtonDown_ & button) != 0;
    }

    bool Input::mouseButtonPressed(glm::int32 button)
    {
        return (mouseButtonPress_ & button) != 0;
    }

    glm::int32 Input::modifiersDown()
    {
        glm::int32 ret = 0;

        if (modifierDown(MODIFIER_ALT))
            ret |= MODIFIER_ALT;
        if (modifierDown(MODIFIER_CONTROL))
            ret |= MODIFIER_CONTROL;
        if (modifierDown(MODIFIER_SHIFT))
            ret |= MODIFIER_SHIFT;
        if (modifierDown(MODIFIER_SUPER))
            ret |= MODIFIER_SUPER;

        return ret;
    }

    void Input::initialize()
    {
        resetState();

        subscribeToEvent(BeginFrameEvent::getTypeStatic(), HANDLER(Input, handleBeginFrame));
        subscribeToEvent(ScreenModeEvent::getTypeStatic(), HANDLER(Input, handleScreenMode));

        GLFWwindow* window = glfwGetCurrentContext();
        if (!window)
            return;

        setCursorMode(cursorMode_);

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);

        glfwSetWindowFocusCallback(window, &Input::handleFocusCallback);
        glfwSetWindowIconifyCallback(window, &Input::handleIconifiedCallback);
        glfwSetMouseButtonCallback(window, &Input::handleMouseButtonCallback);
        glfwSetKeyCallback(window, &Input::handleKeyCallback);
        glfwSetScrollCallback(window, &Input::handleScrollCallback);
        glfwSetCursorPosCallback(window, &Input::handleCursorPosCallback);        
        
        initialized_ = true;
    }

    void Input::resetState()
    {
        keyDown_.clear();
        keyPress_.clear();
        scancodeDown_.clear();
        scancodePress_.clear();

        mouseMove_ = glm::ivec2(0, 0);
        mouseMoveWheel_ = 0;
        mouseButtonPress_ = 0;
        mouseButtonDown_ = 0;
    }

    void Input::handleScreenMode(const Event* eventData)
    {
        if (!initialized_)
            initialize();
        else
            resetState();

        GLFWwindow* window = glfwGetCurrentContext();

        minimized_ = glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GL_TRUE;
    }

    void Input::handleBeginFrame(const Event* eventData)
    {
        update();
    }

    void Input::handleFocusCallback(GLFWwindow* window, int focused)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        input->focused_ = focused == GL_TRUE;
    }

    void Input::handleIconifiedCallback(GLFWwindow* window, int iconified)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        input->minimized_ = iconified == GL_TRUE;
    }

    void Input::handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        if (action == GLFW_PRESS)
        {
            input->mouseButtonDown_ |= button;
            input->mouseButtonPress_ |= button;

            MouseButtonPressEvent* event = context->createEvent<MouseButtonPressEvent>();
            event->button = button;
            event->buttons = input->mouseButtonDown_;
            event->modifiers = mods;
            input->sendEvent(MouseButtonPressEvent::getTypeStatic(), event);
        }
        else
        {
            input->mouseButtonDown_ &= ~button;

            MouseButtonReleaseEvent* event = context->createEvent<MouseButtonReleaseEvent>();
            event->button = button;
            event->buttons = input->mouseButtonDown_;
            event->modifiers = mods;
            input->sendEvent(MouseButtonReleaseEvent::getTypeStatic(), event);
        }
    }

    void Input::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        if (action == GLFW_PRESS)
        {
            input->keyDown_.insert(key);
            input->keyPress_.insert(key);
            input->scancodeDown_.insert(scancode);
            input->scancodePress_.insert(scancode);

            KeyPressEvent* event = context->createEvent<KeyPressEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->mouseButtonDown_;
            event->modifiers = mods;
            event->repeat = false;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
        else if (action == GLFW_REPEAT)
        {
            input->keyDown_.insert(key);
            input->keyPress_.insert(key);
            input->scancodeDown_.insert(scancode);
            input->scancodePress_.insert(scancode);

            KeyPressEvent* event = context->createEvent<KeyPressEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->mouseButtonDown_;
            event->modifiers = mods;
            event->repeat = true;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
        else
        {
            input->keyDown_.erase(key);
            input->scancodeDown_.erase(scancode);

            KeyReleaseEvent* event = context->createEvent<KeyReleaseEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->mouseButtonDown_;
            event->modifiers = mods;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
    }

    void Input::handleScrollCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        double amount = x > y ? x : -y;

        input->mouseMoveWheel_ += amount;

        MouseScrollEvent* event = context->createEvent<MouseScrollEvent>();
        event->amount = amount;
        event->buttons = input->mouseButtonDown_;
        event->modifiers = input->modifiersDown();
        input->sendEvent(MouseScrollEvent::getTypeStatic(), event);
    }

    void Input::handleCursorPosCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getInput();

        glm::ivec2 position = glm::ivec2(x, y);
        glm::ivec2 relative = input->lastMousePosition_ - position;

        input->lastMousePosition_ = position;
        input->mouseMove_ += relative;

        MouseMoveEvent* event = context->createEvent<MouseMoveEvent>();
        event->position = position;
        event->relative = relative;
        event->buttons = input->mouseButtonDown_;
        event->modifiers = input->modifiersDown();
        input->sendEvent(MouseMoveEvent::getTypeStatic(), event);
    }

}