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

#include "Context.h"
#include "Graphics.h"

#include <glfw3.h>

namespace Eris
{

    Graphics::Graphics(Context* context) :
        Object(context),
        impl_(new GraphicsImpl())
    {

    }

    Graphics::~Graphics()
    {
        if (impl_ && impl_->GetWindow())
        {
            impl_->DestroyWindow();
        }
    }

    void Graphics::Initialize()
    {
        
    }

    void Graphics::Maximize()
    {

    }

    void Graphics::Minimize()
    {

    }

    void Graphics::Restore()
    {

    }

    void Graphics::Hide()
    {

    }

    void Graphics::Show()
    {

    }

    void Graphics::Close()
    {
    }

    void Graphics::SetMode(int width, int height, int samples, unsigned flags)
    {

    }

    void Graphics::SetMode(int width, int height)
    {

    }

    void Graphics::SetGamma(float gamma)
    {

    }

    void Graphics::SetTitle(const String& title)
    {

    }

    PODVector<IntVector2> Graphics::GetResolutions() const
    {

    }

    Eris::IntVector2 Graphics::GetDesktopResolution() const
    {

    }

    void Graphics::HandleFramebufferCallback(GLFWwindow* window, int width, int height)
    {

    }

}