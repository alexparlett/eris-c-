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
#include "Serializable.h"
#include "Memory/Pointers.h"

namespace Eris
{
    class Node : public Serializable
    {
    public:
        Node(Context* context);
        virtual ~Node();

        virtual void load(const JsonElement* src) const;
        virtual void save(JsonElement* dest) const;

        glm::u64 getId() const { return m_id; }
        Node* getParent() const { return m_parent; }
        bool isActive() const;
        bool isActiveSelf() const { return m_active; }

        void addChild(Node* child);
        void removeChild(Node* child);
        void removeChildren();
        Node* getChild(glm::u64 id) const;
        Node* getChild(glm::u32 index) const;
        const std::vector<SharedPtr<Node>>& getChildren() const { return m_children; }

        template<typename T> T* addComponent();
        void removeComponent(Component* component);
        template<typename T> void removeComponents();
        template<typename T> T* getComponent() const;
        template<typename T> std::vector<T*> getComponents() const;

    private:
        glm::u64 m_id;
        bool m_active;
        Node* m_parent;
        std::vector<SharedPtr<Node>> m_children;
        std::vector<SharedPtr<Component>> m_components;

        static glm::u64 s_next_node_id;
    };

    template<typename T> std::vector < T* >
    Eris::Node::getComponents() const
    {
        std::vector<T*> components;
        for (auto c : m_components)
        {
            T* cast_component = static_cast<T*>(c.get());
            if (cast_component)
                components.push_back(cast_component);
        }

        return components;
    }

    template<typename T>
    T* Eris::Node::getComponent() const
    {
        for (auto c : m_components)
        {
            T* cast_component = static_cast<T*>(c.get());
            if (cast_component)
                return cast_component;
        }

        return nullptr;
    }

    template<typename T>
    void Eris::Node::removeComponents()
    {
        auto current = m_components.begin();
        while (current != m_components.end())
        {
            T* cast_component = static_cast<T*>((*current).get());
            if (cast_component)
                current = m_components.erase(current);
            else
                current++;
        }
    }

    template<typename T>
    T* Eris::Node::addComponent()
    {
        T* new_component = new T(m_context, this);

        ERIS_ASSERT(new_component);

        m_components.push_back(SharedPtr<Component>(new_component));
        return new_component;
    }

}