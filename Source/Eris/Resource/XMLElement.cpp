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

#include "XMLElement.h"
#include "XMLFile.h"

#include "Collections/Functions.h"

namespace Eris
{
    const XMLElement XMLElement::EMPTY;

    XMLElement::XMLElement() :
        m_file(WeakPtr<XMLFile>()),
        m_node(nullptr)
    {
    }

    XMLElement::XMLElement(XMLFile* file, pugi::xml_node node) :
        m_file(file),
        m_node(node)
    {
    }

    XMLElement::XMLElement(const XMLElement& rhs)
    {
        *this = rhs;
    }

    XMLElement& XMLElement::operator=(const XMLElement& rhs)
    {
        m_file = rhs.m_file;
        m_node = rhs.m_node;
        return *this;
    }

    Eris::XMLElement XMLElement::addChild(const std::string& name)
    {
        if (!m_file || !m_node)
            return EMPTY;

        pugi::xml_node child = m_node.append_child(name.c_str());
        return XMLElement(m_file, child);
    }

    bool XMLElement::removeChild(XMLElement& ele)
    {
        if (!ele.m_file || !ele.m_node || !m_file || !m_node)
            return false;

        return m_node.remove_child(ele.m_node);
    }

    bool XMLElement::removeChild(const std::string& name)
    {
        if (!m_file || !m_node)
            return false;

        return m_node.remove_child(name.c_str());
    }

    bool XMLElement::removeChildren(const std::string& name /*= StringEmpty*/)
    {
        if (!m_file || !m_node)
            return false;

        for (auto node : m_node.children(name.c_str()))
        {
            m_node.remove_child(node);
        }

        return true;
    }

    bool XMLElement::removeAttribute(const std::string& name /*= StringEmpty*/)
    {
        if (!m_file || !m_node)
            return false;

        return m_node.remove_attribute(name.c_str());
    }

    XMLElement::operator bool() const
    {
        if (!m_file || !m_node)
            return false;

        return m_node && !m_node.empty();
    }

    XMLElement::iterator XMLElement::begin()
    {
        auto iter = m_node.begin();
        return XMLElementIterator(m_file, XMLElement(m_file, *iter), iter, m_node.end());
    }

    XMLElement::iterator XMLElement::end()
    {
        return XMLElementIterator(m_file, XMLElement(), m_node.end(), m_node.end());
    }

    XMLElement::const_iterator XMLElement::begin() const
    {
        auto iter = m_node.begin();
        return XMLElementIterator(m_file, XMLElement(m_file, *iter), iter, m_node.end());
    }

    XMLElement::const_iterator XMLElement::end() const
    {
        return XMLElementIterator(m_file, XMLElement(), m_node.end(), m_node.end());
    }

    bool XMLElement::empty() const
    {
        if (!m_file || !m_node)
            return false;

        return m_node.empty();
    }

    bool XMLElement::hasChild(const std::string& name) const
    {
        if (!m_file || !m_node)
            return false;

        return !m_node.child(name.c_str()).empty();
    }

    bool XMLElement::hasAttribute(const std::string& name) const
    {
        if (!m_file || !m_node)
            return false;

        return !m_node.attribute(name.c_str()).empty();
    }

    bool XMLElement::hasChildren() const
    {
        if (!m_file || !m_node)
            return false;

        return !m_node.first_child().empty();
    }

    std::string XMLElement::getName() const
    {
        if (!m_file || !m_node)
            return StringEmpty;

        return m_node.name();
    }

    std::string XMLElement::getAttribute(const std::string& name /*= StringEmpty*/) const
    {
        if (!m_file || !m_node)
            return StringEmpty;

        return m_node.attribute(name.c_str()).value();
    }

    XMLElement XMLElement::getChild(const std::string& name /*= StringEmpty*/) const
    {
        if (!m_file || !m_node)
            return EMPTY;

        return XMLElement(m_file, m_node.child(name.c_str()));
    }

    XMLElement XMLElement::getNext(const std::string& name /* = StringEmpty */) const
    {
        if (!m_file || !m_node)
            return EMPTY;

        return XMLElement(m_file, m_node.next_sibling(name.c_str()));
    }

    std::string XMLElement::getValue() const
    {
        if (!m_file || !m_node)
            return StringEmpty;

        return m_node.text().get();
    }

    bool XMLElement::getBool(bool default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::string_lower(value) == "true";
    }

    glm::i32 XMLElement::getI32(glm::i32 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stoi(value);
    }

    glm::u32 XMLElement::getU32(glm::u32 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stoul(value);
    }

    glm::f32 XMLElement::getF32(glm::f32 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stof(value);
    }

    glm::i64 XMLElement::getI64(glm::i64 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stoll(value);
    }

    glm::u64 XMLElement::getU64(glm::u64 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stoull(value);
    }

    glm::f64 XMLElement::getF64(glm::f64 default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return std::stod(value);
    }


    std::string XMLElement::getString(std::string default) const
    {
        std::string value = getValue();
        if (value == StringEmpty)
            return default;

        return value;
    }

    bool XMLElement::getBool(const std::string& name, bool default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::string_lower(value) == "true";
    }

    glm::i32 XMLElement::getI32(const std::string& name, glm::i32 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stoi(value);
    }

    glm::u32 XMLElement::getU32(const std::string& name, glm::u32 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stoul(value);
    }

    glm::f32 XMLElement::getF32(const std::string& name, glm::f32 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stof(value);
    }

    glm::i64 XMLElement::getI64(const std::string& name, glm::i64 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stoll(value);
    }

    glm::u64 XMLElement::getU64(const std::string& name, glm::u64 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stoull(value);
    }

    glm::f64 XMLElement::getF64(const std::string& name, glm::f64 default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return std::stod(value);
    }


    std::string XMLElement::getString(const std::string& name, std::string default) const
    {
        std::string value = getAttribute(name);
        if (value == StringEmpty)
            return default;

        return value;
    }

    bool XMLElement::setValue(const std::string& value)
    {
        if (!m_file || !m_node)
            return false;

        return m_node.set_value(value.c_str());
    }

    bool XMLElement::setAttribute(const std::string& name, const std::string& value)
    {
        if (!m_file || !m_node)
            return false;

        pugi::xml_attribute attr = m_node.attribute(name.c_str());
        if (attr)
            return attr.set_value(value.c_str());
        else
            return m_node.append_attribute(name.c_str()).set_value(value.c_str());
    }

    XMLElementIterator::XMLElementIterator(XMLFile* file, XMLElement elemenet, pugi::xml_node_iterator iter, pugi::xml_node_iterator end) :
        m_file(file),
        m_iter(iter),
        m_end(end),
        m_element(elemenet)
    {
    }

    XMLElementIterator XMLElementIterator::operator++ ()
    {
        XMLElementIterator i = *this;

        m_iter++;
        if (m_iter != m_end)
            m_element = XMLElement(m_file, *m_iter);
        else
            m_element = XMLElement();

        return i;
    }

    XMLElementIterator XMLElementIterator::operator++ (glm::i32 junk)
    {
        m_iter++;
        if (m_iter != m_end)
            m_element = XMLElement(m_file, *m_iter);
        else
            m_element = XMLElement();

        return *this;
    }

    XMLElement& XMLElementIterator::operator* () 
    { 
        ERIS_ASSERT(m_element); 
        return m_element; 
    }

    XMLElement* XMLElementIterator::operator-> () 
    { 
        ERIS_ASSERT(m_element); 
        return &m_element; 
    }

    bool XMLElementIterator::operator == (const XMLElementIterator& rhs) 
    { 
        return m_iter == rhs.m_iter; 
    }

    bool XMLElementIterator::operator != (const XMLElementIterator& rhs) 
    { 
        return m_iter != rhs.m_iter; 
    }
}