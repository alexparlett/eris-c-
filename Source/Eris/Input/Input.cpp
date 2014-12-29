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

#include "Input.h"
#include "InputEvents.h"

#include "Core/CoreEvents.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsEvents.h"

namespace Eris
{

    Input::Input(Context* context) :
        Object(context),
        m_mouse_button_down(0),
        m_mouse_button_press(0),
        m_mouse_wheel_move(0),
        m_mouse_move(glm::ivec2(0,0)),
        m_cursor_mode(CursorMode::CM_NORMAL),
        m_focused(false),
        m_minimized(false),
        m_key_down(ChainAllocator<glm::i32>(0, 8 * sizeof(glm::i32), 128 * sizeof(glm::i32))),
        m_key_press(ChainAllocator<glm::i32>(0, 8 * sizeof(glm::i32), 128 * sizeof(glm::i32))),
        m_scancode_down(ChainAllocator<glm::i32>(0, 8 * sizeof(glm::i32), 128 * sizeof(glm::i32))),
        m_scancode_press(ChainAllocator<glm::i32>(0, 8 * sizeof(glm::i32), 128 * sizeof(glm::i32)))
    {
    }

    Input::~Input()
    {

    }

    void Input::update()
    {
        ERIS_ASSERT(m_initialised);
        resetState();
        glfwPollEvents();
    }

    void Input::setCursorMode(CursorMode cm)
    {
        m_cursor_mode = cm;

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
        return m_key_down.find(key) != m_key_down.end();
    }

    bool Input::keyPressed(glm::int32 key)
    {
        return m_key_press.find(key) != m_key_press.end();
    }

    bool Input::scancodeDown(glm::int32 scancode)
    {
        return m_scancode_down.find(scancode) != m_scancode_down.end();
    }

    bool Input::scancodePressed(glm::int32 scancode)
    {
        return m_scancode_press.find(scancode) != m_scancode_press.end();
    }

    bool Input::modifierDown(glm::int32 modifier)
    {
        if (modifier == MODIFIER_ALT)
            return m_key_down.find(KEY_LEFT_ALT) != m_key_down.end() || m_key_down.find(KEY_RIGHT_ALT) != m_key_down.end();
        else if (modifier == MODIFIER_CONTROL)
            return m_key_down.find(KEY_LEFT_CONTROL) != m_key_down.end() || m_key_down.find(KEY_RIGHT_CONTROL) != m_key_down.end();
        else if (modifier == MODIFIER_SHIFT)
            return m_key_down.find(KEY_LEFT_SHIFT) != m_key_down.end() || m_key_down.find(KEY_RIGHT_SHIFT) != m_key_down.end();
        else if (modifier == MODIFIER_SUPER)
            return m_key_down.find(KEY_LEFT_SUPER) != m_key_down.end() || m_key_down.find(KEY_RIGHT_SUPER) != m_key_down.end();

        return false;
    }

    bool Input::modifierPressed(glm::int32 modifier)
    {
        if (modifier == MODIFIER_ALT)
            return m_key_press.find(KEY_LEFT_ALT) != m_key_press.end() || m_key_press.find(KEY_RIGHT_ALT) != m_key_press.end();
        else if (modifier == MODIFIER_CONTROL)
            return m_key_press.find(KEY_LEFT_CONTROL) != m_key_press.end() || m_key_press.find(KEY_RIGHT_CONTROL) != m_key_press.end();
        else if (modifier == MODIFIER_SHIFT)
            return m_key_press.find(KEY_LEFT_SHIFT) != m_key_press.end() || m_key_press.find(KEY_RIGHT_SHIFT) != m_key_press.end();
        else if (modifier == MODIFIER_SUPER)
            return m_key_press.find(KEY_LEFT_SUPER) != m_key_press.end() || m_key_press.find(KEY_RIGHT_SUPER) != m_key_press.end();

        return false;
    }

    bool Input::mouseButtonDown(glm::int32 button)
    {
        return (m_mouse_button_down & button) != 0;
    }

    bool Input::mouseButtonPressed(glm::int32 button)
    {
        return (m_mouse_button_press & button) != 0;
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

        setCursorMode(m_cursor_mode);

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_FALSE);

        glfwSetWindowFocusCallback(window, &Input::handleFocusCallback);
        glfwSetWindowIconifyCallback(window, &Input::handleIconifiedCallback);
        glfwSetMouseButtonCallback(window, &Input::handleMouseButtonCallback);
        glfwSetKeyCallback(window, &Input::handleKeyCallback);
        glfwSetScrollCallback(window, &Input::handleScrollCallback);
        glfwSetCursorPosCallback(window, &Input::handleCursorPosCallback);        
        
        m_initialised = true;
    }

    void Input::resetState()
    {
        m_key_down.clear();
        m_key_press.clear();
        m_scancode_down.clear();
        m_scancode_press.clear();

        m_mouse_move = glm::ivec2(0, 0);
        m_mouse_wheel_move = 0;
        m_mouse_button_press = 0;
        m_mouse_button_down = 0;
    }

    void Input::handleScreenMode(const StringHash& event_type, const Event* event_data)
    {
        if (!m_initialised)
            initialize();
        else
            resetState();

        GLFWwindow* window = glfwGetCurrentContext();

        m_minimized = glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GL_TRUE;
    }

    void Input::handleBeginFrame(const StringHash& event_type, const Event* event_data)
    {
        update();
    }

    void Input::handleFocusCallback(GLFWwindow* window, int focused)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        input->m_focused = focused == GL_TRUE;
    }

    void Input::handleIconifiedCallback(GLFWwindow* window, int iconified)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        input->m_minimized = iconified == GL_TRUE;
    }

    void Input::handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        if (action == GLFW_PRESS)
        {
            input->m_mouse_button_down |= button;
            input->m_mouse_button_press |= button;

            MouseButtonPressEvent* event = context->getFrameAllocator().newInstance<MouseButtonPressEvent>();
            event->button = button;
            event->buttons = input->m_mouse_button_down;
            event->modifiers = mods;
            input->sendEvent(MouseButtonPressEvent::getTypeStatic(), event);
        }
        else
        {
            input->m_mouse_button_down &= ~button;

            MouseButtonReleaseEvent* event = context->getFrameAllocator().newInstance<MouseButtonReleaseEvent>();
            event->button = button;
            event->buttons = input->m_mouse_button_down;
            event->modifiers = mods;
            input->sendEvent(MouseButtonReleaseEvent::getTypeStatic(), event);
        }
    }

    void Input::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        if (action == GLFW_PRESS)
        {
            input->m_key_down.insert(key);
            input->m_key_press.insert(key);
            input->m_scancode_down.insert(scancode);
            input->m_scancode_press.insert(scancode);

            KeyPressEvent* event = context->getFrameAllocator().newInstance<KeyPressEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->m_mouse_button_down;
            event->modifiers = mods;
            event->repeat = false;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
        else if (action == GLFW_REPEAT)
        {
            input->m_key_down.insert(key);
            input->m_key_press.insert(key);
            input->m_scancode_down.insert(scancode);
            input->m_scancode_press.insert(scancode);

            KeyPressEvent* event = context->getFrameAllocator().newInstance<KeyPressEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->m_mouse_button_down;
            event->modifiers = mods;
            event->repeat = true;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
        else
        {
            input->m_key_down.erase(key);
            input->m_scancode_down.erase(scancode);

            KeyReleaseEvent* event = context->getFrameAllocator().newInstance<KeyReleaseEvent>();
            event->key = key;
            event->scancode = scancode;
            event->buttons = input->m_mouse_button_down;
            event->modifiers = mods;
            input->sendEvent(KeyPressEvent::getTypeStatic(), event);
        }
    }

    void Input::handleScrollCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        double amount = x > y ? x : -y;

        input->m_mouse_wheel_move += amount;

        MouseScrollEvent* event = context->getFrameAllocator().newInstance<MouseScrollEvent>();
        event->amount = amount;
        event->buttons = input->m_mouse_button_down;
        event->modifiers = input->modifiersDown();
        input->sendEvent(MouseScrollEvent::getTypeStatic(), event);
    }

    void Input::handleCursorPosCallback(GLFWwindow* window, double x, double y)
    {
        Context* context = static_cast<Context*>(glfwGetWindowUserPointer(window));
        Input* input = context->getModule<Input>();

        glm::ivec2 position = glm::ivec2(x, y);
        glm::ivec2 relative = input->m_mouse_last_position - position;

        input->m_mouse_last_position = position;
        input->m_mouse_move += relative;

        MouseMoveEvent* event = context->getFrameAllocator().newInstance<MouseMoveEvent>();
        event->position = position;
        event->relative = relative;
        event->buttons = input->m_mouse_button_down;
        event->modifiers = input->modifiersDown();
        input->sendEvent(MouseMoveEvent::getTypeStatic(), event);
    }

}