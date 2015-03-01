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

#include "Camera.h"
#include "Node.h"
#include "Transform.h"

namespace Eris
{
    Camera::Camera( Context* context, Node* node ) :
        Component(context, node),
        m_fov(60.f),
        m_near_clip(0.1f),
        m_far_clip(100.f)
    {
    }

    Camera::~Camera()
    {
    }

    glm::mat4 Camera::view() const
    {
        ERIS_ASSERT( m_node );

        Transform* transform = m_node->component<Transform>();

        ERIS_ASSERT( transform );

        glm::vec3 pos = transform->position();
        glm::vec3 forward = transform->forward();
        glm::vec3 up = transform->up();

        return glm::lookAt( pos, forward, up );
    }

    void Camera::setFov( glm::f32 fov )
    {
        m_fov = glm::max( fov, 1.f );
    }

    void Camera::setNearClip( glm::f32 near_clip)
    {
        m_near_clip = glm::max( near_clip, 0.f );
    }

    void Camera::setFarClip( glm::f32 far_clip )
    {
        m_far_clip = glm::max( far_clip, m_near_clip );
    }

}