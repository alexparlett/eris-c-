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

#include "Node.h"
#include "Transform.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Eris
{
    Transform::Transform( Context* context, Node* node ) :
        Component( context, node ),
        m_local_scale( 1.f )
    {
    }

    Transform::~Transform()
    {
    }

    void Transform::load( const JsonElement* src ) const
    {
        ERIS_ASSERT( src );
    }

    void Transform::save( JsonElement* dest ) const
    {
        ERIS_ASSERT( dest );
    }

    void Transform::setLocalPosition( const glm::vec3& position )
    {
        m_local_position = position;
        invalidateTransform();
    }

    void Transform::setLocalRotation( const glm::quat& rotation )
    {
        m_local_rotation = rotation;
        invalidateTransform();
    }

    void Transform::setLocalScale( glm::f32 scalar )
    {
        m_local_scale = glm::vec3( scalar );
        invalidateTransform();
    }

    void Transform::setLocalScale( const glm::vec3& scale )
    {
        m_local_scale = scale;
        invalidateTransform();
    }

    void Transform::setPosition( const glm::vec3& position )
    {
        setLocalPosition( m_node && m_node->getParent() ? glm::vec3( glm::inverse( m_node->getParent()->getComponent<Transform>()->getTransform() ) * glm::vec4( position, 0.f ) ) : position );
    }

    void Transform::setRotation( const glm::quat& rotation )
    {
        setLocalRotation( m_node && m_node->getParent() ? glm::quat_cast( glm::inverse( m_node->getParent()->getComponent<Transform>()->getTransform() ) * glm::mat4_cast( rotation ) ) : rotation );
    }

    void Transform::setScale( glm::f32 scalar )
    {
        setScale( glm::vec3( scalar ) );
    }

    void Transform::setScale( const glm::vec3& scale )
    {
        setLocalScale( m_node && m_node->getParent() ? scale / m_node->getParent()->getComponent<Transform>()->getScale() : scale );
    }

    glm::vec3 Transform::getPosition() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return translation;
    }

    glm::quat Transform::getRotation() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return rotation;
    }

    glm::vec3 Transform::getScale() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return scale;
    }

    glm::mat4 Transform::getLocalTransform() const
    {
        glm::mat4 transform = glm::mat4_cast( m_local_rotation );
        transform = glm::translate( transform, m_local_position );
        transform = glm::scale( transform, m_local_scale );
        return transform;
    }

    glm::vec3 Transform::getLocalForward() const
    {
        return m_local_rotation * glm::vec3( 0.f, 0.f, -1.f );
    }

    glm::vec3 Transform::getLocalUp() const
    {
        return m_local_rotation * glm::vec3( 0.f, 1.f, 0.f );
    }
    glm::vec3 Transform::getLocalRight() const
    {
        return m_local_rotation * glm::vec3( 1.f, 0.f, 0.f );
    }

    glm::vec3 Transform::getForward() const
    {
        return getRotation() * glm::vec3( 0.f, 0.f, -1.f );
    }

    glm::vec3 Transform::getUp() const
    {
        return getRotation() * glm::vec3( 0.f, 1.f, 0.f );
    }

    glm::vec3 Transform::getRight() const
    {
        return getRotation() * glm::vec3( 1.f, 0.f, 0.f );
    }

    void Transform::invalidateTransform()
    {
        const glm::mat4 local = getLocalTransform();
        m_world_transform = m_node && m_node->getParent() ? m_node->getParent()->getComponent<Transform>()->m_world_transform * local : local;
    }

    glm::vec3 Transform::getLocalEulerAngles() const
    {
        return glm::eulerAngles( m_local_rotation );
    }

    glm::vec3 Transform::getEulerAngles() const
    {
        return glm::eulerAngles( getRotation() );
    }

    void Transform::setLocalEulerAngles( const glm::vec3& eulerAngles )
    {
        setLocalRotation( glm::quat_cast( glm::orientate4( eulerAngles ) ) );
    }

    void Transform::setEulerAngles( const glm::vec3& eulerAngles )
    {
        setRotation( glm::quat_cast( glm::orientate4( eulerAngles ) ) );
    }

    glm::f32 Transform::getLocalRoll() const
    {
        return glm::roll( m_local_rotation );
    }

    glm::f32 Transform::getLocalPitch() const
    {
        return glm::pitch( m_local_rotation );
    }

    glm::f32 Transform::getLocalYaw() const
    {
        return glm::yaw( m_local_rotation );
    }

    glm::f32 Transform::getRoll() const
    {
        return glm::roll( getRotation() );
    }

    glm::f32 Transform::getPitch() const
    {
        return glm::pitch( getRotation() );
    }

    glm::f32 Transform::getYaw() const
    {
        return glm::yaw( getRotation() );
    }

}