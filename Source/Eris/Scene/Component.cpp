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

namespace Eris
{
    Component::Component(Context* context) :
        Serializable(context),
        m_node(nullptr)
    {
    }

    Component::Component(Context* context, Node* node) :
        Serializable(context),
        m_node(node)
    {
    }

    Component::~Component()
    {
        if (m_node)
        {
            m_node->removeComponent(this);
            m_node = nullptr;
        }
    }

    void Component::setNode(Node* node)
    {
        ERIS_ASSERT(node);
        m_node = node;
    }

    Node* Component::getNode() const
    {
        return m_node;
    }
}