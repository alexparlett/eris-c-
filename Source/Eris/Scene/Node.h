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

#include "Core/Context.h"
#include "Core/Object.h"
#include "Memory/Pointers.h"

namespace Eris
{
    class Node : public Object
    {
    public:
        Node(Context* context);
        virtual ~Node();

        glm::u64 id() const { return m_id; }
        Node* parent() const { return m_parent; }
        const std::list<SharedPtr<Node>>& children() const { return m_children; }

        void addChild(Node* child);
        void removeChild(Node* child);
        void removeChildren() { m_children.clear(); }
        Node* firstChild() const { return m_children.front(); }
        Node* child(glm::u64 id) const;
        Node* lastChild() const { return m_children.back(); }
        std::size_t childCount() const { return m_children.size(); }

        template<typename T>
        T* createComponent<T>();
        void removeComponent(Component* component);
        template<typename T>
        void removeComponents();
        template<typename T>
        T* component(glm::u64 id = 0ULL) const;
        template<typename T>
        T* componentOrCreate(glm::u64 id = 0ULL);
        template<typename T>
        std::list<T*> components() const;
        std::size_t componentCount() const { return m_components.size(); }


        void setPosition(const glm::vec3& position);
        void setRotation(const glm::quat& rotation);
        void setScale(glm::f32 scalar);
        void setScale(const glm::vec3& scale);
        void setWorldPosition(const glm::vec3& position);
        void setWorldRotation(const glm::quat& rotation);
        void setWorldScale(const glm::f32 scalar);
        void setWorldScale(const glm::vec3& scale);

        const glm::vec3& position() const { return m_position; }
        const glm::quat& rotation() const { return m_rotation; }
        const glm::vec3& scale() const { return m_scale; }
        glm::vec3 forward() const { return m_rotation * glm::vec3(1.f, 0.f, 0.f); }
        glm::vec3 up() const { return m_rotation * glm::vec3(0.f, 1.f, 0.f); }
        glm::vec3 right() const { return m_rotation * glm::vec3(0.f, 0.f, 1.f); }
        glm::mat4 transform() const;
        glm::vec3 worldPosition() const;
        glm::quat worldRotation() const;
        glm::vec3 worldScale() const;
        glm::vec3 worldForward() const { return worldRotation() * glm::vec3(1.f, 0.f, 0.f); }
        glm::vec3 worldUp() const { return worldRotation() * glm::vec3(0.f, 1.f, 0.f); }
        glm::vec3 worldRight() const { return worldRotation() * glm::vec3(0.f, 0.f, 1.f); }
        glm::mat4 worldTransform() const { return m_world_transform; }

    private:
        void invalidateWorldTransform();

        glm::u64 m_id;
        glm::mat4 m_world_transform;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
        WeakPtr<Node> m_parent;
        std::list<SharedPtr<Node>> m_children;
        std::list<SharedPtr<Component>> m_components;

        static glm::u64 s_next_node_id;
    };

    template<typename T>
    std::list<T*>
    Node::components() const
    {

    }

    template<typename T>
    T* Node::componentOrCreate(glm::u64 id /*= 0ULL*/)
    {

    }

    template<typename T>
    T* Node::component(glm::u64 id /*= 0ULL*/) const
    {

    }

    template<typename T>
    void Node::removeComponents()
    {

    }
}