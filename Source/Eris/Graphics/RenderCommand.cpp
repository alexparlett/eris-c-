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

#include "Renderer.h"
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

    void ClearColorCommand::operator()(Renderer* renderer, const RenderKey* last_key)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void ClearCommand::operator()(Renderer* renderer, const RenderKey* last_key)
    {
        glClear(bitfield);
    }

    void EnableCommand::operator()(Renderer* renderer, const RenderKey* last_key)
    {
        glEnable(capability);
    }

    void DisableCommand::operator()(Renderer* renderer, const RenderKey* last_key)
    {
        glDisable(capability);
    }

    void Draw3DCommand::operator()(Renderer* renderer, const RenderKey* last_key)
    {
        AutoProfilerBlock profile(renderer->getContext()->getModule<Profiler>(), "Draw3DCommand");

        if ( !last_key || last_key->material != key.material )
        {
            material->getUniform( "view" )->data = renderer->getCurrentView();
            material->getUniform( "perspective" )->data = renderer->getCurrentPerspective();
            material->use();
        }

        for (auto uniform : uniforms)
            renderer->bindUniform(uniform.location, uniform.type, uniform.data);

        model->draw();
    }

    void CameraCommand::operator()( Renderer* renderer, const RenderKey* last_key )
    {
        glm::mat4 view = camera->getView();
        glm::mat4 perspective = camera->getPerspective();

        renderer->setCurrentView( view );
        renderer->setCurrentPerspective( perspective );
    }

}