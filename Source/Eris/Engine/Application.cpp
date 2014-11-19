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

#include "Application.h"

#include <windows.h>

namespace Eris
{

    Application::Application(Context* context) :
        Object(context),
        exitCode_(0)
    {
        ParseParameters(GetArguments());

        engine_ = new Engine(context);
    }

    void Application::ParseParameters(const Vector<String>& args)
    {
    }

    int Application::Run()
    {
        Setup();
        if (exitCode_)
            return exitCode_;

        if (!engine_->Initialize(parameters_))
            return exitCode_;

        Start();
        if (exitCode_)
            return exitCode_;

        while (!engine_->IsExiting())
            engine_->RunFrame();

        Stop();

        return exitCode_;
    }

    Vector<String> Application::GetArguments()
    {
        Vector<String> ret;
        LPWSTR* argv;
        int argc;

        argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (argv == NULL)
            return ret;

        for (int i = 0; i < argc; i++)
            ret.Push(argv[i]);

        LocalFree(argv);

        return ret;
    }

}