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

#include "Core/Event.h"

namespace Eris
{
    struct MouseButtonReleaseEvent : public Event
    {
        EVENT(MouseButtonRelease)

    public:
        glm::int32 button;
        glm::int32 buttons;
        glm::int32 modifiers;
    };

    struct MouseButtonPressEvent : public Event
    {
        EVENT(MouseButtonPress)

    public:
        glm::int32 button;
        glm::int32 buttons;
        glm::int32 modifiers;
    };

    struct MouseScrollEvent : public Event
    {
        EVENT(MouseScroll)

    public:
        double amount;
        glm::int32 buttons;
        glm::int32 modifiers;
    };

    struct MouseMoveEvent : public Event
    {
        EVENT(MouseMove)

    public:
        glm::ivec2 position;
        glm::ivec2 relative;
        glm::int32 buttons;
        glm::int32 modifiers;
    };

    struct KeyPressEvent : public Event
    {
        EVENT(KeyPress)

    public:
        glm::int32 key;
        glm::int32 scancode;
        glm::int32 buttons;
        glm::int32 modifiers;
        bool repeat;
    };

    struct KeyReleaseEvent : public Event
    {
        EVENT(KeyRelease)

    public:
        glm::int32 key;
        glm::int32 scancode;
        glm::int32 buttons;
        glm::int32 modifiers;
    };

    static const glm::int32 KEY_UNKNOWN = GLFW_KEY_UNKNOWN;
    static const glm::int32 KEY_SPACE = GLFW_KEY_SPACE;
    static const glm::int32 KEY_COMMA = GLFW_KEY_COMMA;
    static const glm::int32 KEY_MINUS = GLFW_KEY_MINUS;
    static const glm::int32 KEY_PERIOD = GLFW_KEY_PERIOD;
    static const glm::int32 KEY_SLASH = GLFW_KEY_SLASH;
    static const glm::int32 KEY_0 = GLFW_KEY_0;
    static const glm::int32 KEY_1 = GLFW_KEY_1;
    static const glm::int32 KEY_2 = GLFW_KEY_2;
    static const glm::int32 KEY_3 = GLFW_KEY_3;
    static const glm::int32 KEY_4 = GLFW_KEY_4;
    static const glm::int32 KEY_5 = GLFW_KEY_5;
    static const glm::int32 KEY_6 = GLFW_KEY_6;
    static const glm::int32 KEY_7 = GLFW_KEY_7;
    static const glm::int32 KEY_8 = GLFW_KEY_8;
    static const glm::int32 KEY_9 = GLFW_KEY_9;
    static const glm::int32 KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
    static const glm::int32 KEY_EQUAL = GLFW_KEY_EQUAL;
    static const glm::int32 KEY_A = GLFW_KEY_A;
    static const glm::int32 KEY_B = GLFW_KEY_B;
    static const glm::int32 KEY_C = GLFW_KEY_C;
    static const glm::int32 KEY_D = GLFW_KEY_D;
    static const glm::int32 KEY_E = GLFW_KEY_E;
    static const glm::int32 KEY_F = GLFW_KEY_F;
    static const glm::int32 KEY_G = GLFW_KEY_G;
    static const glm::int32 KEY_H = GLFW_KEY_H;
    static const glm::int32 KEY_I = GLFW_KEY_I;
    static const glm::int32 KEY_J = GLFW_KEY_J;
    static const glm::int32 KEY_K = GLFW_KEY_K;
    static const glm::int32 KEY_L = GLFW_KEY_L;
    static const glm::int32 KEY_M = GLFW_KEY_M;
    static const glm::int32 KEY_N = GLFW_KEY_N;
    static const glm::int32 KEY_O = GLFW_KEY_O;
    static const glm::int32 KEY_P = GLFW_KEY_P;
    static const glm::int32 KEY_Q = GLFW_KEY_Q;
    static const glm::int32 KEY_R = GLFW_KEY_R;
    static const glm::int32 KEY_S = GLFW_KEY_S;
    static const glm::int32 KEY_T = GLFW_KEY_T;
    static const glm::int32 KEY_U = GLFW_KEY_U;
    static const glm::int32 KEY_V = GLFW_KEY_V;
    static const glm::int32 KEY_W = GLFW_KEY_W;
    static const glm::int32 KEY_X = GLFW_KEY_X;
    static const glm::int32 KEY_Y = GLFW_KEY_Y;
    static const glm::int32 KEY_Z = GLFW_KEY_Z;
    static const glm::int32 KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
    static const glm::int32 KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
    static const glm::int32 KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
    static const glm::int32 KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
    static const glm::int32 KEY_WORLD_1 = GLFW_KEY_WORLD_1;
    static const glm::int32 KEY_WORLD_2 = GLFW_KEY_WORLD_2;
    static const glm::int32 KEY_ESCAPE = GLFW_KEY_ESCAPE;
    static const glm::int32 KEY_ENTER = GLFW_KEY_ENTER;
    static const glm::int32 KEY_TAB = GLFW_KEY_TAB;
    static const glm::int32 KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
    static const glm::int32 KEY_INSERT = GLFW_KEY_INSERT;
    static const glm::int32 KEY_DELETE = GLFW_KEY_DELETE;
    static const glm::int32 KEY_RIGHT = GLFW_KEY_RIGHT;
    static const glm::int32 KEY_LEFT = GLFW_KEY_LEFT;
    static const glm::int32 KEY_DOWN = GLFW_KEY_DOWN;
    static const glm::int32 KEY_UP = GLFW_KEY_UP;
    static const glm::int32 KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
    static const glm::int32 KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
    static const glm::int32 KEY_HOME = GLFW_KEY_HOME;
    static const glm::int32 KEY_END = GLFW_KEY_END;
    static const glm::int32 KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
    static const glm::int32 KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
    static const glm::int32 KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
    static const glm::int32 KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
    static const glm::int32 KEY_PAUSE = GLFW_KEY_PAUSE;
    static const glm::int32 KEY_F1 = GLFW_KEY_F1;
    static const glm::int32 KEY_F2 = GLFW_KEY_F2;
    static const glm::int32 KEY_F3 = GLFW_KEY_F3;
    static const glm::int32 KEY_F4 = GLFW_KEY_F4;
    static const glm::int32 KEY_F5 = GLFW_KEY_F5;
    static const glm::int32 KEY_F6 = GLFW_KEY_F6;
    static const glm::int32 KEY_F7 = GLFW_KEY_F7;
    static const glm::int32 KEY_F8 = GLFW_KEY_F8;
    static const glm::int32 KEY_F9 = GLFW_KEY_F9;
    static const glm::int32 KEY_F10 = GLFW_KEY_F10;
    static const glm::int32 KEY_F11 = GLFW_KEY_F11;
    static const glm::int32 KEY_F12 = GLFW_KEY_F12;
    static const glm::int32 KEY_F13 = GLFW_KEY_F13;
    static const glm::int32 KEY_F14 = GLFW_KEY_F14;
    static const glm::int32 KEY_F15 = GLFW_KEY_F15;
    static const glm::int32 KEY_F16 = GLFW_KEY_F16;
    static const glm::int32 KEY_F17 = GLFW_KEY_F17;
    static const glm::int32 KEY_F18 = GLFW_KEY_F18;
    static const glm::int32 KEY_F19 = GLFW_KEY_F19;
    static const glm::int32 KEY_F20 = GLFW_KEY_F20;
    static const glm::int32 KEY_F21 = GLFW_KEY_F21;
    static const glm::int32 KEY_F22 = GLFW_KEY_F22;
    static const glm::int32 KEY_F23 = GLFW_KEY_F23;
    static const glm::int32 KEY_F24 = GLFW_KEY_F24;
    static const glm::int32 KEY_F25 = GLFW_KEY_F25;
    static const glm::int32 KEY_KP_0 = GLFW_KEY_KP_0;
    static const glm::int32 KEY_KP_1 = GLFW_KEY_KP_1;
    static const glm::int32 KEY_KP_2 = GLFW_KEY_KP_2;
    static const glm::int32 KEY_KP_3 = GLFW_KEY_KP_3;
    static const glm::int32 KEY_KP_4 = GLFW_KEY_KP_4;
    static const glm::int32 KEY_KP_5 = GLFW_KEY_KP_5;
    static const glm::int32 KEY_KP_6 = GLFW_KEY_KP_6;
    static const glm::int32 KEY_KP_7 = GLFW_KEY_KP_7;
    static const glm::int32 KEY_KP_8 = GLFW_KEY_KP_8;
    static const glm::int32 KEY_KP_9 = GLFW_KEY_KP_9;
    static const glm::int32 KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
    static const glm::int32 KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
    static const glm::int32 KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
    static const glm::int32 KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
    static const glm::int32 KEY_KP_ADD = GLFW_KEY_KP_ADD;
    static const glm::int32 KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
    static const glm::int32 KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
    static const glm::int32 KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    static const glm::int32 KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    static const glm::int32 KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
    static const glm::int32 KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
    static const glm::int32 KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    static const glm::int32 KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    static const glm::int32 KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
    static const glm::int32 KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
    static const glm::int32 KEY_MENU = GLFW_KEY_MENU;

    static const glm::int32 SCANCODE_UNKNOWN = 0;
    static const glm::int32 SCANCODE_A = 4;
    static const glm::int32 SCANCODE_B = 5;
    static const glm::int32 SCANCODE_C = 6;
    static const glm::int32 SCANCODE_D = 7;
    static const glm::int32 SCANCODE_E = 8;
    static const glm::int32 SCANCODE_F = 9;
    static const glm::int32 SCANCODE_G = 10;
    static const glm::int32 SCANCODE_H = 11;
    static const glm::int32 SCANCODE_I = 12;
    static const glm::int32 SCANCODE_J = 13;
    static const glm::int32 SCANCODE_K = 14;
    static const glm::int32 SCANCODE_L = 15;
    static const glm::int32 SCANCODE_M = 16;
    static const glm::int32 SCANCODE_N = 17;
    static const glm::int32 SCANCODE_O = 18;
    static const glm::int32 SCANCODE_P = 19;
    static const glm::int32 SCANCODE_Q = 20;
    static const glm::int32 SCANCODE_R = 21;
    static const glm::int32 SCANCODE_S = 22;
    static const glm::int32 SCANCODE_T = 23;
    static const glm::int32 SCANCODE_U = 24;
    static const glm::int32 SCANCODE_V = 25;
    static const glm::int32 SCANCODE_W = 26;
    static const glm::int32 SCANCODE_X = 27;
    static const glm::int32 SCANCODE_Y = 28;
    static const glm::int32 SCANCODE_Z = 29;
    static const glm::int32 SCANCODE_1 = 30;
    static const glm::int32 SCANCODE_2 = 31;
    static const glm::int32 SCANCODE_3 = 32;
    static const glm::int32 SCANCODE_4 = 33;
    static const glm::int32 SCANCODE_5 = 34;
    static const glm::int32 SCANCODE_6 = 35;
    static const glm::int32 SCANCODE_7 = 36;
    static const glm::int32 SCANCODE_8 = 37;
    static const glm::int32 SCANCODE_9 = 38;
    static const glm::int32 SCANCODE_0 = 39;
    static const glm::int32 SCANCODE_RETURN = 40;
    static const glm::int32 SCANCODE_ESCAPE = 41;
    static const glm::int32 SCANCODE_BACKSPACE = 42;
    static const glm::int32 SCANCODE_TAB = 43;
    static const glm::int32 SCANCODE_SPACE = 44;
    static const glm::int32 SCANCODE_MINUS = 45;
    static const glm::int32 SCANCODE_EQUALS = 46;
    static const glm::int32 SCANCODE_LEFTBRACKET = 47;
    static const glm::int32 SCANCODE_RIGHTBRACKET = 48;
    static const glm::int32 SCANCODE_BACKSLASH = 49;
    static const glm::int32 SCANCODE_NONUSHASH = 50;
    static const glm::int32 SCANCODE_SEMICOLON = 51;
    static const glm::int32 SCANCODE_APOSTROPHE = 52;
    static const glm::int32 SCANCODE_GRAVE = 53;
    static const glm::int32 SCANCODE_COMMA = 54;
    static const glm::int32 SCANCODE_PERIOD = 55;
    static const glm::int32 SCANCODE_SLASH = 56;
    static const glm::int32 SCANCODE_CAPSLOCK = 57;
    static const glm::int32 SCANCODE_F1 = 58;
    static const glm::int32 SCANCODE_F2 = 59;
    static const glm::int32 SCANCODE_F3 = 60;
    static const glm::int32 SCANCODE_F4 = 61;
    static const glm::int32 SCANCODE_F5 = 62;
    static const glm::int32 SCANCODE_F6 = 63;
    static const glm::int32 SCANCODE_F7 = 64;
    static const glm::int32 SCANCODE_F8 = 65;
    static const glm::int32 SCANCODE_F9 = 66;
    static const glm::int32 SCANCODE_F10 = 67;
    static const glm::int32 SCANCODE_F11 = 68;
    static const glm::int32 SCANCODE_F12 = 69;
    static const glm::int32 SCANCODE_PRINTSCREEN = 70;
    static const glm::int32 SCANCODE_SCROLLLOCK = 71;
    static const glm::int32 SCANCODE_PAUSE = 72;
    static const glm::int32 SCANCODE_INSERT = 73;
    static const glm::int32 SCANCODE_HOME = 74;
    static const glm::int32 SCANCODE_PAGEUP = 75;
    static const glm::int32 SCANCODE_DELETE = 76;
    static const glm::int32 SCANCODE_END = 77;
    static const glm::int32 SCANCODE_PAGEDOWN = 78;
    static const glm::int32 SCANCODE_RIGHT = 79;
    static const glm::int32 SCANCODE_LEFT = 80;
    static const glm::int32 SCANCODE_DOWN = 81;
    static const glm::int32 SCANCODE_UP = 82;
    static const glm::int32 SCANCODE_NUMLOCKCLEAR = 83;
    static const glm::int32 SCANCODE_KP_DIVIDE = 84;
    static const glm::int32 SCANCODE_KP_MULTIPLY = 85;
    static const glm::int32 SCANCODE_KP_MINUS = 86;
    static const glm::int32 SCANCODE_KP_PLUS = 87;
    static const glm::int32 SCANCODE_KP_ENTER = 88;
    static const glm::int32 SCANCODE_KP_1 = 89;
    static const glm::int32 SCANCODE_KP_2 = 90;
    static const glm::int32 SCANCODE_KP_3 = 91;
    static const glm::int32 SCANCODE_KP_4 = 92;
    static const glm::int32 SCANCODE_KP_5 = 93;
    static const glm::int32 SCANCODE_KP_6 = 94;
    static const glm::int32 SCANCODE_KP_7 = 95;
    static const glm::int32 SCANCODE_KP_8 = 96;
    static const glm::int32 SCANCODE_KP_9 = 97;
    static const glm::int32 SCANCODE_KP_0 = 98;
    static const glm::int32 SCANCODE_KP_PERIOD = 99;
    static const glm::int32 SCANCODE_NONUSBACKSLASH = 100;
    static const glm::int32 SCANCODE_APPLICATION = 101;
    static const glm::int32 SCANCODE_POWER = 102;
    static const glm::int32 SCANCODE_KP_EQUALS = 103;
    static const glm::int32 SCANCODE_F13 = 104;
    static const glm::int32 SCANCODE_F14 = 105;
    static const glm::int32 SCANCODE_F15 = 106;
    static const glm::int32 SCANCODE_F16 = 107;
    static const glm::int32 SCANCODE_F17 = 108;
    static const glm::int32 SCANCODE_F18 = 109;
    static const glm::int32 SCANCODE_F19 = 110;
    static const glm::int32 SCANCODE_F20 = 111;
    static const glm::int32 SCANCODE_F21 = 112;
    static const glm::int32 SCANCODE_F22 = 113;
    static const glm::int32 SCANCODE_F23 = 114;
    static const glm::int32 SCANCODE_F24 = 115;
    static const glm::int32 SCANCODE_EXECUTE = 116;
    static const glm::int32 SCANCODE_HELP = 117;
    static const glm::int32 SCANCODE_MENU = 118;
    static const glm::int32 SCANCODE_SELECT = 119;
    static const glm::int32 SCANCODE_STOP = 120;
    static const glm::int32 SCANCODE_AGAIN = 121;
    static const glm::int32 SCANCODE_UNDO = 122;
    static const glm::int32 SCANCODE_CUT = 123;
    static const glm::int32 SCANCODE_COPY = 124;
    static const glm::int32 SCANCODE_PASTE = 125;
    static const glm::int32 SCANCODE_FIND = 126;
    static const glm::int32 SCANCODE_MUTE = 127;
    static const glm::int32 SCANCODE_VOLUMEUP = 128;
    static const glm::int32 SCANCODE_VOLUMEDOWN = 129;
    static const glm::int32 SCANCODE_LOCKINGCAPSLOCK = 130;
    static const glm::int32 SCANCODE_LOCKINGNUMLOCK = 131;
    static const glm::int32 SCANCODE_LOCKINGSCROLLLOCK = 132;
    static const glm::int32 SCANCODE_KP_COMMA = 133;
    static const glm::int32 SCANCODE_KP_EQUALSAS400 = 134;
    static const glm::int32 SCANCODE_INTERNATIONAL1 = 135;
    static const glm::int32 SCANCODE_INTERNATIONAL2 = 136;
    static const glm::int32 SCANCODE_INTERNATIONAL3 = 137;
    static const glm::int32 SCANCODE_INTERNATIONAL4 = 138;
    static const glm::int32 SCANCODE_INTERNATIONAL5 = 139;
    static const glm::int32 SCANCODE_INTERNATIONAL6 = 140;
    static const glm::int32 SCANCODE_INTERNATIONAL7 = 141;
    static const glm::int32 SCANCODE_INTERNATIONAL8 = 142;
    static const glm::int32 SCANCODE_INTERNATIONAL9 = 143;
    static const glm::int32 SCANCODE_LANG1 = 144;
    static const glm::int32 SCANCODE_LANG2 = 145;
    static const glm::int32 SCANCODE_LANG3 = 146;
    static const glm::int32 SCANCODE_LANG4 = 147;
    static const glm::int32 SCANCODE_LANG5 = 148;
    static const glm::int32 SCANCODE_LANG6 = 149;
    static const glm::int32 SCANCODE_LANG7 = 150;
    static const glm::int32 SCANCODE_LANG8 = 151;
    static const glm::int32 SCANCODE_LANG9 = 152;
    static const glm::int32 SCANCODE_ALTERASE = 153;
    static const glm::int32 SCANCODE_SYSREQ = 154;
    static const glm::int32 SCANCODE_CANCEL = 155;
    static const glm::int32 SCANCODE_CLEAR = 156;
    static const glm::int32 SCANCODE_PRIOR = 157;
    static const glm::int32 SCANCODE_RETURN2 = 158;
    static const glm::int32 SCANCODE_SEPARATOR = 159;
    static const glm::int32 SCANCODE_OUT = 160;
    static const glm::int32 SCANCODE_OPER = 161;
    static const glm::int32 SCANCODE_CLEARAGAIN = 162;
    static const glm::int32 SCANCODE_CRSEL = 163;
    static const glm::int32 SCANCODE_EXSEL = 164;
    static const glm::int32 SCANCODE_KP_00 = 176;
    static const glm::int32 SCANCODE_KP_000 = 177;
    static const glm::int32 SCANCODE_THOUSANDSSEPARATOR = 178;
    static const glm::int32 SCANCODE_DECIMALSEPARATOR = 179;
    static const glm::int32 SCANCODE_CURRENCYUNIT = 180;
    static const glm::int32 SCANCODE_CURRENCYSUBUNIT = 181;
    static const glm::int32 SCANCODE_KP_LEFTPAREN = 182;
    static const glm::int32 SCANCODE_KP_RIGHTPAREN = 183;
    static const glm::int32 SCANCODE_KP_LEFTBRACE = 184;
    static const glm::int32 SCANCODE_KP_RIGHTBRACE = 185;
    static const glm::int32 SCANCODE_KP_TAB = 186;
    static const glm::int32 SCANCODE_KP_BACKSPACE = 187;
    static const glm::int32 SCANCODE_KP_A = 188;
    static const glm::int32 SCANCODE_KP_B = 189;
    static const glm::int32 SCANCODE_KP_C = 190;
    static const glm::int32 SCANCODE_KP_D = 191;
    static const glm::int32 SCANCODE_KP_E = 192;
    static const glm::int32 SCANCODE_KP_F = 193;
    static const glm::int32 SCANCODE_KP_XOR = 194;
    static const glm::int32 SCANCODE_KP_POWER = 195;
    static const glm::int32 SCANCODE_KP_PERCENT = 196;
    static const glm::int32 SCANCODE_KP_LESS = 197;
    static const glm::int32 SCANCODE_KP_GREATER = 198;
    static const glm::int32 SCANCODE_KP_AMPERSAND = 199;
    static const glm::int32 SCANCODE_KP_DBLAMPERSAND = 200;
    static const glm::int32 SCANCODE_KP_VERTICALBAR = 201;
    static const glm::int32 SCANCODE_KP_DBLVERTICALBAR = 202;
    static const glm::int32 SCANCODE_KP_COLON = 203;
    static const glm::int32 SCANCODE_KP_HASH = 204;
    static const glm::int32 SCANCODE_KP_SPACE = 205;
    static const glm::int32 SCANCODE_KP_AT = 206;
    static const glm::int32 SCANCODE_KP_EXCLAM = 207;
    static const glm::int32 SCANCODE_KP_MEMSTORE = 208;
    static const glm::int32 SCANCODE_KP_MEMRECALL = 209;
    static const glm::int32 SCANCODE_KP_MEMCLEAR = 210;
    static const glm::int32 SCANCODE_KP_MEMADD = 211;
    static const glm::int32 SCANCODE_KP_MEMSUBTRACT = 212;
    static const glm::int32 SCANCODE_KP_MEMMULTIPLY = 213;
    static const glm::int32 SCANCODE_KP_MEMDIVIDE = 214;
    static const glm::int32 SCANCODE_KP_PLUSMINUS = 215;
    static const glm::int32 SCANCODE_KP_CLEAR = 216;
    static const glm::int32 SCANCODE_KP_CLEARENTRY = 217;
    static const glm::int32 SCANCODE_KP_BINARY = 218;
    static const glm::int32 SCANCODE_KP_OCTAL = 219;
    static const glm::int32 SCANCODE_KP_DECIMAL = 220;
    static const glm::int32 SCANCODE_KP_HEXADECIMAL = 221;
    static const glm::int32 SCANCODE_LCTRL = 224;
    static const glm::int32 SCANCODE_LSHIFT = 225;
    static const glm::int32 SCANCODE_LALT = 226;
    static const glm::int32 SCANCODE_LGUI = 227;
    static const glm::int32 SCANCODE_RCTRL = 228;
    static const glm::int32 SCANCODE_RSHIFT = 229;
    static const glm::int32 SCANCODE_RALT = 230;
    static const glm::int32 SCANCODE_RGUI = 231;
    static const glm::int32 SCANCODE_MODE = 257;
    static const glm::int32 SCANCODE_AUDIONEXT = 258;
    static const glm::int32 SCANCODE_AUDIOPREV = 259;
    static const glm::int32 SCANCODE_AUDIOSTOP = 260;
    static const glm::int32 SCANCODE_AUDIOPLAY = 261;
    static const glm::int32 SCANCODE_AUDIOMUTE = 262;
    static const glm::int32 SCANCODE_MEDIASELECT = 263;
    static const glm::int32 SCANCODE_WWW = 264;
    static const glm::int32 SCANCODE_MAIL = 265;
    static const glm::int32 SCANCODE_CALCULATOR = 266;
    static const glm::int32 SCANCODE_COMPUTER = 267;
    static const glm::int32 SCANCODE_AC_SEARCH = 268;
    static const glm::int32 SCANCODE_AC_HOME = 269;
    static const glm::int32 SCANCODE_AC_BACK = 270;
    static const glm::int32 SCANCODE_AC_FORWARD = 271;
    static const glm::int32 SCANCODE_AC_STOP = 272;
    static const glm::int32 SCANCODE_AC_REFRESH = 273;
    static const glm::int32 SCANCODE_AC_BOOKMARKS = 274;
    static const glm::int32 SCANCODE_BRIGHTNESSDOWN = 275;
    static const glm::int32 SCANCODE_BRIGHTNESSUP = 276;
    static const glm::int32 SCANCODE_DISPLAYSWITCH = 277;
    static const glm::int32 SCANCODE_KBDILLUMTOGGLE = 278;
    static const glm::int32 SCANCODE_KBDILLUMDOWN = 279;
    static const glm::int32 SCANCODE_KBDILLUMUP = 280;
    static const glm::int32 SCANCODE_EJECT = 281;
    static const glm::int32 SCANCODE_SLEEP = 282;
    static const glm::int32 SCANCODE_APP1 = 283;
    static const glm::int32 SCANCODE_APP2 = 284;

    static const glm::int32 MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
    static const glm::int32 MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
    static const glm::int32 MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
    static const glm::int32 MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
    static const glm::int32 MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
    static const glm::int32 MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
    static const glm::int32 MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
    static const glm::int32 MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
    static const glm::int32 MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
    static const glm::int32 MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
    static const glm::int32 MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;

    static const glm::int32 MODIFIER_SHIFT = GLFW_MOD_SHIFT;
    static const glm::int32 MODIFIER_CONTROL = GLFW_MOD_CONTROL;
    static const glm::int32 MODIFIER_ALT = GLFW_MOD_ALT;
    static const glm::int32 MODIFIER_SUPER = GLFW_MOD_SUPER;
}