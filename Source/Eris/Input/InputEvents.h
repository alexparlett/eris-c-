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

#include <glfw3.h>

EVENT(E_MOUSE_BUTTON_RELEASED, MouseButtonReleased)
{
    PARAM(P_BUTTON, Button);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
}

EVENT(E_MOUSE_BUTTON_PRESSED, MouseButtonPressed)
{
    PARAM(P_BUTTON, Button);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
}

EVENT(E_MOUSE_SCROLL, MouseScroll)
{
    PARAM(P_AMOUNT, Amount);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
}

EVENT(E_MOUSE_MOVE, MouseMove)
{
    PARAM(P_POSITION, Position);
    PARAM(P_RELATIVE, Relative);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
}

EVENT(E_KEY_PRESSED, KeyPressed)
{
    PARAM(P_KEY, Key);
    PARAM(P_SCANCODE, Scancode);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
    PARAM(P_REPEAT, Repeat);
}


EVENT(E_KEY_RELEASED, KeyReleased)
{
    PARAM(P_KEY, Key);
    PARAM(P_SCANCODE, Scancode);
    PARAM(P_BUTTONS, Buttons);
    PARAM(P_MODIFIERS, Modifiers);
}

namespace Eris
{
    static const int KEY_UNKNOWN = GLFW_KEY_UNKNOWN;
    static const int KEY_SPACE = GLFW_KEY_SPACE;
    static const int KEY_COMMA = GLFW_KEY_COMMA;
    static const int KEY_MINUS = GLFW_KEY_MINUS;
    static const int KEY_PERIOD = GLFW_KEY_PERIOD;
    static const int KEY_SLASH = GLFW_KEY_SLASH;
    static const int KEY_0 = GLFW_KEY_0;
    static const int KEY_1 = GLFW_KEY_1;
    static const int KEY_2 = GLFW_KEY_2;
    static const int KEY_3 = GLFW_KEY_3;
    static const int KEY_4 = GLFW_KEY_4;
    static const int KEY_5 = GLFW_KEY_5;
    static const int KEY_6 = GLFW_KEY_6;
    static const int KEY_7 = GLFW_KEY_7;
    static const int KEY_8 = GLFW_KEY_8;
    static const int KEY_9 = GLFW_KEY_9;
    static const int KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
    static const int KEY_EQUAL = GLFW_KEY_EQUAL;
    static const int KEY_A = GLFW_KEY_A;
    static const int KEY_B = GLFW_KEY_B;
    static const int KEY_C = GLFW_KEY_C;
    static const int KEY_D = GLFW_KEY_D;
    static const int KEY_E = GLFW_KEY_E;
    static const int KEY_F = GLFW_KEY_F;
    static const int KEY_G = GLFW_KEY_G;
    static const int KEY_H = GLFW_KEY_H;
    static const int KEY_I = GLFW_KEY_I;
    static const int KEY_J = GLFW_KEY_J;
    static const int KEY_K = GLFW_KEY_K;
    static const int KEY_L = GLFW_KEY_L;
    static const int KEY_M = GLFW_KEY_M;
    static const int KEY_N = GLFW_KEY_N;
    static const int KEY_O = GLFW_KEY_O;
    static const int KEY_P = GLFW_KEY_P;
    static const int KEY_Q = GLFW_KEY_Q;
    static const int KEY_R = GLFW_KEY_R;
    static const int KEY_S = GLFW_KEY_S;
    static const int KEY_T = GLFW_KEY_T;
    static const int KEY_U = GLFW_KEY_U;
    static const int KEY_V = GLFW_KEY_V;
    static const int KEY_W = GLFW_KEY_W;
    static const int KEY_X = GLFW_KEY_X;
    static const int KEY_Y = GLFW_KEY_Y;
    static const int KEY_Z = GLFW_KEY_Z;
    static const int KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
    static const int KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
    static const int KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
    static const int KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
    static const int KEY_WORLD_1 = GLFW_KEY_WORLD_1;
    static const int KEY_WORLD_2 = GLFW_KEY_WORLD_2;
    static const int KEY_ESCAPE = GLFW_KEY_ESCAPE;
    static const int KEY_ENTER = GLFW_KEY_ENTER;
    static const int KEY_TAB = GLFW_KEY_TAB;
    static const int KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
    static const int KEY_INSERT = GLFW_KEY_INSERT;
    static const int KEY_DELETE = GLFW_KEY_DELETE;
    static const int KEY_RIGHT = GLFW_KEY_RIGHT;
    static const int KEY_LEFT = GLFW_KEY_LEFT;
    static const int KEY_DOWN = GLFW_KEY_DOWN;
    static const int KEY_UP = GLFW_KEY_UP;
    static const int KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
    static const int KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
    static const int KEY_HOME = GLFW_KEY_HOME;
    static const int KEY_END = GLFW_KEY_END;
    static const int KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
    static const int KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
    static const int KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
    static const int KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
    static const int KEY_PAUSE = GLFW_KEY_PAUSE;
    static const int KEY_F1 = GLFW_KEY_F1;
    static const int KEY_F2 = GLFW_KEY_F2;
    static const int KEY_F3 = GLFW_KEY_F3;
    static const int KEY_F4 = GLFW_KEY_F4;
    static const int KEY_F5 = GLFW_KEY_F5;
    static const int KEY_F6 = GLFW_KEY_F6;
    static const int KEY_F7 = GLFW_KEY_F7;
    static const int KEY_F8 = GLFW_KEY_F8;
    static const int KEY_F9 = GLFW_KEY_F9;
    static const int KEY_F10 = GLFW_KEY_F10;
    static const int KEY_F11 = GLFW_KEY_F11;
    static const int KEY_F12 = GLFW_KEY_F12;
    static const int KEY_F13 = GLFW_KEY_F13;
    static const int KEY_F14 = GLFW_KEY_F14;
    static const int KEY_F15 = GLFW_KEY_F15;
    static const int KEY_F16 = GLFW_KEY_F16;
    static const int KEY_F17 = GLFW_KEY_F17;
    static const int KEY_F18 = GLFW_KEY_F18;
    static const int KEY_F19 = GLFW_KEY_F19;
    static const int KEY_F20 = GLFW_KEY_F20;
    static const int KEY_F21 = GLFW_KEY_F21;
    static const int KEY_F22 = GLFW_KEY_F22;
    static const int KEY_F23 = GLFW_KEY_F23;
    static const int KEY_F24 = GLFW_KEY_F24;
    static const int KEY_F25 = GLFW_KEY_F25;
    static const int KEY_KP_0 = GLFW_KEY_KP_0;
    static const int KEY_KP_1 = GLFW_KEY_KP_1;
    static const int KEY_KP_2 = GLFW_KEY_KP_2;
    static const int KEY_KP_3 = GLFW_KEY_KP_3;
    static const int KEY_KP_4 = GLFW_KEY_KP_4;
    static const int KEY_KP_5 = GLFW_KEY_KP_5;
    static const int KEY_KP_6 = GLFW_KEY_KP_6;
    static const int KEY_KP_7 = GLFW_KEY_KP_7;
    static const int KEY_KP_8 = GLFW_KEY_KP_8;
    static const int KEY_KP_9 = GLFW_KEY_KP_9;
    static const int KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
    static const int KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
    static const int KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
    static const int KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
    static const int KEY_KP_ADD = GLFW_KEY_KP_ADD;
    static const int KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
    static const int KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
    static const int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    static const int KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    static const int KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
    static const int KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
    static const int KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    static const int KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    static const int KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
    static const int KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
    static const int KEY_MENU = GLFW_KEY_MENU;

    static const int SCANCODE_UNKNOWN = 0;
    static const int SCANCODE_A = 4;
    static const int SCANCODE_B = 5;
    static const int SCANCODE_C = 6;
    static const int SCANCODE_D = 7;
    static const int SCANCODE_E = 8;
    static const int SCANCODE_F = 9;
    static const int SCANCODE_G = 10;
    static const int SCANCODE_H = 11;
    static const int SCANCODE_I = 12;
    static const int SCANCODE_J = 13;
    static const int SCANCODE_K = 14;
    static const int SCANCODE_L = 15;
    static const int SCANCODE_M = 16;
    static const int SCANCODE_N = 17;
    static const int SCANCODE_O = 18;
    static const int SCANCODE_P = 19;
    static const int SCANCODE_Q = 20;
    static const int SCANCODE_R = 21;
    static const int SCANCODE_S = 22;
    static const int SCANCODE_T = 23;
    static const int SCANCODE_U = 24;
    static const int SCANCODE_V = 25;
    static const int SCANCODE_W = 26;
    static const int SCANCODE_X = 27;
    static const int SCANCODE_Y = 28;
    static const int SCANCODE_Z = 29;
    static const int SCANCODE_1 = 30;
    static const int SCANCODE_2 = 31;
    static const int SCANCODE_3 = 32;
    static const int SCANCODE_4 = 33;
    static const int SCANCODE_5 = 34;
    static const int SCANCODE_6 = 35;
    static const int SCANCODE_7 = 36;
    static const int SCANCODE_8 = 37;
    static const int SCANCODE_9 = 38;
    static const int SCANCODE_0 = 39;
    static const int SCANCODE_RETURN = 40;
    static const int SCANCODE_ESCAPE = 41;
    static const int SCANCODE_BACKSPACE = 42;
    static const int SCANCODE_TAB = 43;
    static const int SCANCODE_SPACE = 44;
    static const int SCANCODE_MINUS = 45;
    static const int SCANCODE_EQUALS = 46;
    static const int SCANCODE_LEFTBRACKET = 47;
    static const int SCANCODE_RIGHTBRACKET = 48;
    static const int SCANCODE_BACKSLASH = 49;
    static const int SCANCODE_NONUSHASH = 50;
    static const int SCANCODE_SEMICOLON = 51;
    static const int SCANCODE_APOSTROPHE = 52;
    static const int SCANCODE_GRAVE = 53;
    static const int SCANCODE_COMMA = 54;
    static const int SCANCODE_PERIOD = 55;
    static const int SCANCODE_SLASH = 56;
    static const int SCANCODE_CAPSLOCK = 57;
    static const int SCANCODE_F1 = 58;
    static const int SCANCODE_F2 = 59;
    static const int SCANCODE_F3 = 60;
    static const int SCANCODE_F4 = 61;
    static const int SCANCODE_F5 = 62;
    static const int SCANCODE_F6 = 63;
    static const int SCANCODE_F7 = 64;
    static const int SCANCODE_F8 = 65;
    static const int SCANCODE_F9 = 66;
    static const int SCANCODE_F10 = 67;
    static const int SCANCODE_F11 = 68;
    static const int SCANCODE_F12 = 69;
    static const int SCANCODE_PRINTSCREEN = 70;
    static const int SCANCODE_SCROLLLOCK = 71;
    static const int SCANCODE_PAUSE = 72;
    static const int SCANCODE_INSERT = 73;
    static const int SCANCODE_HOME = 74;
    static const int SCANCODE_PAGEUP = 75;
    static const int SCANCODE_DELETE = 76;
    static const int SCANCODE_END = 77;
    static const int SCANCODE_PAGEDOWN = 78;
    static const int SCANCODE_RIGHT = 79;
    static const int SCANCODE_LEFT = 80;
    static const int SCANCODE_DOWN = 81;
    static const int SCANCODE_UP = 82;
    static const int SCANCODE_NUMLOCKCLEAR = 83;
    static const int SCANCODE_KP_DIVIDE = 84;
    static const int SCANCODE_KP_MULTIPLY = 85;
    static const int SCANCODE_KP_MINUS = 86;
    static const int SCANCODE_KP_PLUS = 87;
    static const int SCANCODE_KP_ENTER = 88;
    static const int SCANCODE_KP_1 = 89;
    static const int SCANCODE_KP_2 = 90;
    static const int SCANCODE_KP_3 = 91;
    static const int SCANCODE_KP_4 = 92;
    static const int SCANCODE_KP_5 = 93;
    static const int SCANCODE_KP_6 = 94;
    static const int SCANCODE_KP_7 = 95;
    static const int SCANCODE_KP_8 = 96;
    static const int SCANCODE_KP_9 = 97;
    static const int SCANCODE_KP_0 = 98;
    static const int SCANCODE_KP_PERIOD = 99;
    static const int SCANCODE_NONUSBACKSLASH = 100;
    static const int SCANCODE_APPLICATION = 101;
    static const int SCANCODE_POWER = 102;
    static const int SCANCODE_KP_EQUALS = 103;
    static const int SCANCODE_F13 = 104;
    static const int SCANCODE_F14 = 105;
    static const int SCANCODE_F15 = 106;
    static const int SCANCODE_F16 = 107;
    static const int SCANCODE_F17 = 108;
    static const int SCANCODE_F18 = 109;
    static const int SCANCODE_F19 = 110;
    static const int SCANCODE_F20 = 111;
    static const int SCANCODE_F21 = 112;
    static const int SCANCODE_F22 = 113;
    static const int SCANCODE_F23 = 114;
    static const int SCANCODE_F24 = 115;
    static const int SCANCODE_EXECUTE = 116;
    static const int SCANCODE_HELP = 117;
    static const int SCANCODE_MENU = 118;
    static const int SCANCODE_SELECT = 119;
    static const int SCANCODE_STOP = 120;
    static const int SCANCODE_AGAIN = 121;
    static const int SCANCODE_UNDO = 122;
    static const int SCANCODE_CUT = 123;
    static const int SCANCODE_COPY = 124;
    static const int SCANCODE_PASTE = 125;
    static const int SCANCODE_FIND = 126;
    static const int SCANCODE_MUTE = 127;
    static const int SCANCODE_VOLUMEUP = 128;
    static const int SCANCODE_VOLUMEDOWN = 129;
    static const int SCANCODE_LOCKINGCAPSLOCK = 130;
    static const int SCANCODE_LOCKINGNUMLOCK = 131;
    static const int SCANCODE_LOCKINGSCROLLLOCK = 132;
    static const int SCANCODE_KP_COMMA = 133;
    static const int SCANCODE_KP_EQUALSAS400 = 134;
    static const int SCANCODE_INTERNATIONAL1 = 135;
    static const int SCANCODE_INTERNATIONAL2 = 136;
    static const int SCANCODE_INTERNATIONAL3 = 137;
    static const int SCANCODE_INTERNATIONAL4 = 138;
    static const int SCANCODE_INTERNATIONAL5 = 139;
    static const int SCANCODE_INTERNATIONAL6 = 140;
    static const int SCANCODE_INTERNATIONAL7 = 141;
    static const int SCANCODE_INTERNATIONAL8 = 142;
    static const int SCANCODE_INTERNATIONAL9 = 143;
    static const int SCANCODE_LANG1 = 144;
    static const int SCANCODE_LANG2 = 145;
    static const int SCANCODE_LANG3 = 146;
    static const int SCANCODE_LANG4 = 147;
    static const int SCANCODE_LANG5 = 148;
    static const int SCANCODE_LANG6 = 149;
    static const int SCANCODE_LANG7 = 150;
    static const int SCANCODE_LANG8 = 151;
    static const int SCANCODE_LANG9 = 152;
    static const int SCANCODE_ALTERASE = 153;
    static const int SCANCODE_SYSREQ = 154;
    static const int SCANCODE_CANCEL = 155;
    static const int SCANCODE_CLEAR = 156;
    static const int SCANCODE_PRIOR = 157;
    static const int SCANCODE_RETURN2 = 158;
    static const int SCANCODE_SEPARATOR = 159;
    static const int SCANCODE_OUT = 160;
    static const int SCANCODE_OPER = 161;
    static const int SCANCODE_CLEARAGAIN = 162;
    static const int SCANCODE_CRSEL = 163;
    static const int SCANCODE_EXSEL = 164;
    static const int SCANCODE_KP_00 = 176;
    static const int SCANCODE_KP_000 = 177;
    static const int SCANCODE_THOUSANDSSEPARATOR = 178;
    static const int SCANCODE_DECIMALSEPARATOR = 179;
    static const int SCANCODE_CURRENCYUNIT = 180;
    static const int SCANCODE_CURRENCYSUBUNIT = 181;
    static const int SCANCODE_KP_LEFTPAREN = 182;
    static const int SCANCODE_KP_RIGHTPAREN = 183;
    static const int SCANCODE_KP_LEFTBRACE = 184;
    static const int SCANCODE_KP_RIGHTBRACE = 185;
    static const int SCANCODE_KP_TAB = 186;
    static const int SCANCODE_KP_BACKSPACE = 187;
    static const int SCANCODE_KP_A = 188;
    static const int SCANCODE_KP_B = 189;
    static const int SCANCODE_KP_C = 190;
    static const int SCANCODE_KP_D = 191;
    static const int SCANCODE_KP_E = 192;
    static const int SCANCODE_KP_F = 193;
    static const int SCANCODE_KP_XOR = 194;
    static const int SCANCODE_KP_POWER = 195;
    static const int SCANCODE_KP_PERCENT = 196;
    static const int SCANCODE_KP_LESS = 197;
    static const int SCANCODE_KP_GREATER = 198;
    static const int SCANCODE_KP_AMPERSAND = 199;
    static const int SCANCODE_KP_DBLAMPERSAND = 200;
    static const int SCANCODE_KP_VERTICALBAR = 201;
    static const int SCANCODE_KP_DBLVERTICALBAR = 202;
    static const int SCANCODE_KP_COLON = 203;
    static const int SCANCODE_KP_HASH = 204;
    static const int SCANCODE_KP_SPACE = 205;
    static const int SCANCODE_KP_AT = 206;
    static const int SCANCODE_KP_EXCLAM = 207;
    static const int SCANCODE_KP_MEMSTORE = 208;
    static const int SCANCODE_KP_MEMRECALL = 209;
    static const int SCANCODE_KP_MEMCLEAR = 210;
    static const int SCANCODE_KP_MEMADD = 211;
    static const int SCANCODE_KP_MEMSUBTRACT = 212;
    static const int SCANCODE_KP_MEMMULTIPLY = 213;
    static const int SCANCODE_KP_MEMDIVIDE = 214;
    static const int SCANCODE_KP_PLUSMINUS = 215;
    static const int SCANCODE_KP_CLEAR = 216;
    static const int SCANCODE_KP_CLEARENTRY = 217;
    static const int SCANCODE_KP_BINARY = 218;
    static const int SCANCODE_KP_OCTAL = 219;
    static const int SCANCODE_KP_DECIMAL = 220;
    static const int SCANCODE_KP_HEXADECIMAL = 221;
    static const int SCANCODE_LCTRL = 224;
    static const int SCANCODE_LSHIFT = 225;
    static const int SCANCODE_LALT = 226;
    static const int SCANCODE_LGUI = 227;
    static const int SCANCODE_RCTRL = 228;
    static const int SCANCODE_RSHIFT = 229;
    static const int SCANCODE_RALT = 230;
    static const int SCANCODE_RGUI = 231;
    static const int SCANCODE_MODE = 257;
    static const int SCANCODE_AUDIONEXT = 258;
    static const int SCANCODE_AUDIOPREV = 259;
    static const int SCANCODE_AUDIOSTOP = 260;
    static const int SCANCODE_AUDIOPLAY = 261;
    static const int SCANCODE_AUDIOMUTE = 262;
    static const int SCANCODE_MEDIASELECT = 263;
    static const int SCANCODE_WWW = 264;
    static const int SCANCODE_MAIL = 265;
    static const int SCANCODE_CALCULATOR = 266;
    static const int SCANCODE_COMPUTER = 267;
    static const int SCANCODE_AC_SEARCH = 268;
    static const int SCANCODE_AC_HOME = 269;
    static const int SCANCODE_AC_BACK = 270;
    static const int SCANCODE_AC_FORWARD = 271;
    static const int SCANCODE_AC_STOP = 272;
    static const int SCANCODE_AC_REFRESH = 273;
    static const int SCANCODE_AC_BOOKMARKS = 274;
    static const int SCANCODE_BRIGHTNESSDOWN = 275;
    static const int SCANCODE_BRIGHTNESSUP = 276;
    static const int SCANCODE_DISPLAYSWITCH = 277;
    static const int SCANCODE_KBDILLUMTOGGLE = 278;
    static const int SCANCODE_KBDILLUMDOWN = 279;
    static const int SCANCODE_KBDILLUMUP = 280;
    static const int SCANCODE_EJECT = 281;
    static const int SCANCODE_SLEEP = 282;
    static const int SCANCODE_APP1 = 283;
    static const int SCANCODE_APP2 = 284;

    static const int MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
    static const int MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
    static const int MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
    static const int MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
    static const int MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
    static const int MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
    static const int MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
    static const int MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
    static const int MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
    static const int MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
    static const int MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;

    static const int MOD_SHIFT = GLFW_MOD_SHIFT;
    static const int MOD_CONTROL = GLFW_MOD_CONTROL;
    static const int MOD_ALT = GLFW_MOD_ALT;
    static const int MOD_SUPER = GLFW_MOD_SUPER;
}