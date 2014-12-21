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

namespace Eris
{
    class Object;

    class Application;
    class Graphics;

    class Context
    {
        friend class Object;

    public:
        Context();
        ~Context();

        void setExitCode(glm::i32 exitCode);
        glm::i32 getExitCode() const { return mExitCode; }

        template<class T> void registerModule(T* module) {}
        template<class T> T* getModule() { return nullptr; }

    private:
        glm::i32 mExitCode;

        Application* mApp;
        Graphics* mGraphics;
    };

    template<> inline void Context::registerModule<Graphics>(Graphics* graphics)
    {
        mGraphics = graphics;
    }

    template<> inline Graphics* Context::getModule<Graphics>()
    {
        return mGraphics;
    }

    template<> inline void Context::registerModule<Application>(Application* app)
    {
        mApp = app;
    }

    template<> inline Application* Context::getModule<Application>()
    {
        return mApp;
    }
}
