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

#include "Component.h"

namespace Eris
{
    class Camera : public Component
    {
    public:
        Camera(Context* context, Node* node);
        virtual ~Camera();

        glm::f32 getFov() const { return m_fov; }
        glm::f32 getNearClip() const { return m_near_clip; }
        glm::f32 getFarClip() const { return m_far_clip; }
        glm::f32 getAspectRation() const { return m_aspect_ratio; }

        glm::mat4 getView() const;
        glm::mat4 getPerspective() const;

        void setFov(glm::f32 fov);
        void setNearClip(glm::f32 near_clip);
        void setFarClip(glm::f32 far_clip);
        void setAspectRatio( glm::f32 aspect_ratio );

    private:
        glm::f32 m_fov;
        glm::f32 m_near_clip;
        glm::f32 m_far_clip;
        glm::f32 m_aspect_ratio;
    };
}