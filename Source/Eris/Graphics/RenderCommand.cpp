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

#include "Graphics.h"
#include "RenderCommand.h"

#include "Core/Profiler.h"

namespace Eris
{
    glm::u64 RenderKey::operator()()
    {
        if (transparency)
        {
            key = target << 62
                | target_layer << 60
                | transparency << 58
                | command << 57
                | extra << 54
                | material << 24
                | depth;
        }
        else
        {
            key = target << 62
                | target_layer << 60
                | transparency << 58
                | command << 57
                | extra << 54
                | depth << 32
                | material;
        }

        return key;
    }

    void ClearColorCommand::operator()(Graphics* graphics, const RenderKey* last_key)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void ClearCommand::operator()(Graphics* graphics, const RenderKey* last_key)
    {
        glClear(bitfield);
    }

    void EnableCommand::operator()(Graphics* graphics, const RenderKey* last_key)
    {
        glEnable(capability);
    }

    void DisableCommand::operator()(Graphics* graphics, const RenderKey* last_key)
    {
        glDisable(capability);
    }

    void Draw3DCommand::operator()(Graphics* graphics, const RenderKey* last_key)
    {
        AutoProfilerBlock profile(graphics->getContext()->getModule<Profiler>(), "Draw3DCommand");

        if (!last_key || last_key->material != key.material)
            material->use();

        for (auto uniform : uniforms)
            graphics->bindUniform(uniform.location, uniform.type, uniform.data);

        model->draw();
    }

    void CameraCommand::operator()( Graphics* graphics, const RenderKey* last_key )
    {
        glm::f32 aspect = graphics->getWidth() / graphics->getHeight();
        glm::mat4 view = camera->view();
        glm::mat4 perspective = glm::perspective( camera->fov(), aspect, camera->nearClip(), camera->farClip() );
    }

}