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
    Transform::Transform( Context* context ) :
        Component( context ),
        m_scale( 1.f )
    {
    }

    Transform::Transform( Context* context, Node* node ) :
        Component( context, node ),
        m_scale( 1.f )
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

    void Transform::setPosition( const glm::vec3& position )
    {
        m_position = position;
        invalidateWorldTransform();
    }

    void Transform::setRotation( const glm::quat& rotation )
    {
        m_rotation = rotation;
        invalidateWorldTransform();
    }

    void Transform::setScale( glm::f32 scalar )
    {
        m_scale = glm::vec3( scalar );
        invalidateWorldTransform();
    }

    void Transform::setScale( const glm::vec3& scale )
    {
        m_scale = scale;
        invalidateWorldTransform();
    }

    void Transform::setWorldPosition( const glm::vec3& position )
    {
        setPosition( m_node && m_node->parent() ? glm::vec3( glm::inverse( m_node->parent()->component<Transform>()->worldTransform() ) * glm::vec4( position, 0.f ) ) : position );
    }

    void Transform::setWorldRotation( const glm::quat& rotation )
    {
        setRotation( m_node && m_node->parent() ? glm::quat_cast( glm::inverse( m_node->parent()->component<Transform>()->worldTransform() ) * glm::mat4_cast( rotation ) ) : rotation );
    }

    void Transform::setWorldScale( glm::f32 scalar )
    {
        setWorldScale( glm::vec3( scalar ) );
    }

    void Transform::setWorldScale( const glm::vec3& scale )
    {
        setScale( m_node && m_node->parent() ? scale / m_node->parent()->component<Transform>()->worldScale() : scale );
    }

    glm::vec3 Transform::worldPosition() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return translation;
    }

    glm::quat Transform::worldRotation() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return rotation;
    }

    glm::vec3 Transform::worldScale() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>( m_world_transform, scale, rotation, translation, skew, perspective );
        return scale;
    }

    glm::mat4 Transform::transform() const
    {
        glm::mat4 transform = glm::mat4_cast( m_rotation );
        transform = glm::translate( transform, m_position );
        transform = glm::scale( transform, m_scale );
        return transform;
    }

    glm::vec3 Transform::forward() const
    {
        return m_rotation * glm::vec3( 1.f, 0.f, 0.f );
    }

    glm::vec3 Transform::up() const
    {
        return m_rotation * glm::vec3( 0.f, 1.f, 0.f );
    }
    glm::vec3 Transform::right() const
    {
        return m_rotation * glm::vec3( 0.f, 0.f, 1.f );
    }

    glm::vec3 Transform::worldForward() const
    {
        return worldRotation() * glm::vec3( 1.f, 0.f, 0.f );
    }

    glm::vec3 Transform::worldUp() const
    {
        return worldRotation() * glm::vec3( 0.f, 1.f, 0.f );
    }

    glm::vec3 Transform::worldRight() const
    {
        return worldRotation() * glm::vec3( 0.f, 0.f, 1.f );
    }

    void Transform::invalidateWorldTransform()
    {
        const glm::mat4 local = transform();
        m_world_transform = m_node && m_node->parent() ? m_node->parent()->component<Transform>()->m_world_transform * local : local;
    }

    glm::vec3 Transform::eulerAngles() const
    {
        return glm::eulerAngles( m_rotation );
    }

    glm::vec3 Transform::worldEulerAngles() const
    {
        return glm::eulerAngles( worldRotation() );
    }

    void Transform::setEulerAngles( const glm::vec3& eulerAngles )
    {
        setRotation( glm::quat_cast( glm::orientate4( eulerAngles ) ) );
    }

    void Transform::setWorldEulerAngles( const glm::vec3& eulerAngles )
    {
        setWorldRotation( glm::quat_cast( glm::orientate4( eulerAngles ) ) );
    }

    glm::f32 Transform::roll() const
    {
        return glm::roll( m_rotation );
    }

    glm::f32 Transform::pitch() const
    {
        return glm::pitch( m_rotation );
    }

    glm::f32 Transform::yaw() const
    {
        return glm::yaw( m_rotation );
    }

    glm::f32 Transform::worldRoll() const
    {
        return glm::roll( worldRotation() );
    }

    glm::f32 Transform::worldPitch() const
    {
        return glm::pitch( worldRotation() );
    }

    glm::f32 Transform::worldYaw() const
    {
        return glm::yaw( worldRotation() );
    }

}