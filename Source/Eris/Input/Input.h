//
// Copyright (c) 2013-2015 the Eris project.
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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/Allocator.h"

#include <unordered_set>

namespace Eris
{
    class Graphics;

    enum class CursorMode : glm::u8
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
        CursorMode getCursorMode() const { return m_cursor_mode; }

        bool keyDown(glm::int32 key);
        bool keyPressed(glm::int32 key);
        bool scancodeDown(glm::int32 scancode);
        bool scancodePressed(glm::int32 scancode);
        bool modifierDown(glm::int32 modifier);
        bool modifierPressed(glm::int32 modifier);
        bool mouseButtonDown(glm::int32 button);
        bool mouseButtonPressed(glm::int32 button);
        glm::int32 modifiersDown();

        bool isFocused() const { return m_focused; }
        bool isMinimized() const { return m_minimized; }

    private:
        void handleBeginFrame(const StringHash& event_type, const Event* event_data);
        void handleScreenMode(const StringHash& event_type, const Event* event_data);

        static void handleFocusCallback(GLFWwindow* window, int focused);
        static void handleIconifiedCallback(GLFWwindow* window, int iconified);
        static void handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void handleScrollCallback(GLFWwindow* window, double x, double y);
        static void handleCursorPosCallback(GLFWwindow* window, double x, double y);

        bool m_initialised;
        bool m_focused;
        bool m_minimized; 
        CursorMode m_cursor_mode;       
        glm::int32 m_mouse_button_down;
        glm::int32 m_mouse_button_press;
        double m_mouse_wheel_move;        
        glm::ivec2 m_mouse_move;
        glm::ivec2 m_mouse_last_position;
        std::unordered_set<glm::int32, std::hash<glm::int32>, std::equal_to<glm::i32>, ChainAllocator<glm::i32>> m_key_down;
        std::unordered_set<glm::int32, std::hash<glm::int32>, std::equal_to<glm::i32>, ChainAllocator<glm::i32>> m_key_press;
        std::unordered_set<glm::int32, std::hash<glm::int32>, std::equal_to<glm::i32>, ChainAllocator<glm::i32>> m_scancode_down;
        std::unordered_set<glm::int32, std::hash<glm::int32>, std::equal_to<glm::i32>, ChainAllocator<glm::i32>> m_scancode_press;
    };

    template<> inline void Context::registerModule(Input* module)
    {
        m_input = SharedPtr<Input>(module);
    }

    template<> inline Input* Context::getModule()
    {
        ERIS_ASSERT(m_input);
        return m_input.get();
    }
}