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

#include "Component.h"
#include "Node.h"
#include "Transform.h"

namespace Eris
{
    glm::u64 Node::s_next_node_id = 1ULL;

    Node::Node(Context* context) :
        Serializable(context),
        m_id(s_next_node_id++),
        m_parent(nullptr)
    {
        addComponent<Transform>();
    }

    Node::~Node()
    {
        if (m_parent)
            m_parent->removeChild(this);

        removeChildren();
    }

    void Node::load(const JsonElement* src) const
    {
        ERIS_ASSERT(src);
    }

    void Node::save(JsonElement* dest) const
    {
        ERIS_ASSERT(dest);
    }

    void Node::addChild(Node* child)
    {
        ERIS_ASSERT(child);
        child->m_parent = this;
        child->component<Transform>()->invalidateWorldTransform();
        m_children.push_back(SharedPtr<Node>(child));
    }

    void Node::removeChild(Node* child)
    {
        ERIS_ASSERT(child);
        for (auto i = m_children.begin(); i != m_children.end(); ++i)
        {
            if (*i == child)
            {
                child->m_parent = nullptr;
                child->component<Transform>()->invalidateWorldTransform();
                m_children.erase(i);
                break;
            }
        }
    }

    Node* Node::child(glm::u64 id) const
    {
        for (auto child : m_children)
        {
            if (child->id() == id)
                return child;
        }

        return nullptr;
    }

    Node* Node::child(glm::u32 index) const
    {
        ERIS_ASSERT(index >= 0 && index < m_children.size());
        return m_children.at(index);
    }

    void Node::removeChildren()
    {
        for (auto child : m_children)
        {
            child->m_parent = nullptr;
            child->component<Transform>()->invalidateWorldTransform();
        }

        m_children.clear();
    }

    void Node::removeComponent( Component* component )
    {
        ERIS_ASSERT( component );
        for ( auto i = m_components.begin(); i != m_components.end(); ++i )
        {
            if ( *i == component )
            {
                m_components.erase( i );
                break;
            }
        }
    }
}