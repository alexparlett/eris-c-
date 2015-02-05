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

#include <glm/gtx/matrix_decompose.hpp>

namespace Eris
{
    Node::Node(Context* context) :
        Object(context),
        m_parent(nullptr),
        m_scale(1.f)
    {
    }

    Node::~Node()
    {
        if (m_parent)
            m_parent->removeChild(const_cast<Node*>(this));
        m_children.clear();
    }

    Node* Node::createChild()
    {
        static glm::u64 next_node_id = 0ULL;

        SharedPtr<Node> node(new Node(m_context));
        node->m_parent = WeakPtr<Node>(this);
        node->invalidateWorldTransform();
        m_children.push_back(node);
        return node;
    }

    void Node::addChild(Node* child)
    {
        child->m_parent = WeakPtr<Node>(this);
        child->invalidateWorldTransform();
        m_children.push_back(SharedPtr<Node>(child));
    }

    void Node::removeChild(Node* child)
    {
        for (auto c = m_children.begin(); c != m_children.end(); c++)
        {
            if (c->get() == child)
            {
                c->get()->m_parent.reset();
                c->get()->invalidateWorldTransform();
                m_children.erase(c);
                return;
            }
        }
    }

    void Node::setPosition(const glm::vec3& position)
    {
        m_position = position;
        invalidateWorldTransform();
    }

    void Node::setRotation(const glm::quat& rotation)
    {
        m_rotation = rotation;
        invalidateWorldTransform();
    }

    void Node::setScale(glm::f32 scalar)
    {
        m_scale = glm::vec3(scalar);
        invalidateWorldTransform();
    }

    void Node::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
        invalidateWorldTransform();
    }

    void Node::setWorldPosition(const glm::vec3& position)
    {
         setPosition(m_parent ? glm::vec3(glm::inverse(m_parent->worldTransform()) * glm::vec4(position,0.f)) : position);
    }

    void Node::setWorldRotation(const glm::quat& rotation)
    {
        setRotation(m_parent ? glm::quat_cast(glm::inverse(m_parent->worldTransform()) * glm::mat4_cast(rotation)) : rotation);
    }

    void Node::setWorldScale(glm::f32 scalar)
    {
        setWorldScale(glm::vec3(scalar));
    }

    void Node::setWorldScale(const glm::vec3& scale)
    {
        setScale(m_parent ? scale / m_parent->worldScale() : scale);
    }

    glm::vec3 Node::worldPosition() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>(m_world_transform, scale, rotation, translation, skew, perspective);
        return translation;
    }

    glm::quat Node::worldRotation() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>(m_world_transform, scale, rotation, translation, skew, perspective);
        return rotation;
    }

    glm::vec3 Node::worldScale() const
    {
        glm::vec3 scale, translation, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose<glm::f32, glm::highp>(m_world_transform, scale, rotation, translation, skew, perspective);
        return scale;
    }

    glm::mat4 Node::transform() const
    {
        glm::mat4 transform = glm::mat4_cast(m_rotation);
        transform = glm::scale(transform, m_scale);
        transform = glm::translate(transform, m_position);
        return transform;
    }

    void Node::invalidateWorldTransform()
    {
        const glm::mat4 local = transform();
        m_world_transform = m_parent ? m_parent->m_world_transform * local : local;
    }
}