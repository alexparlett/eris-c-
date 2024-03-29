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

#include "Memory/Pointers.h"

#include <pugixml/pugixml.hpp>

namespace Eris
{
    class XMLFile;
    class XMLElementIterator;

    class XMLElement
    {
    public:
        using iterator = XMLElementIterator;
        using const_iterator = const XMLElementIterator;

        XMLElement();
        XMLElement(XMLFile* file, pugi::xml_node node);
        XMLElement(const XMLElement& rhs);

        XMLElement& operator = (const XMLElement& rhs);

        XMLElement addChild(const std::string& name);
        bool removeChild(XMLElement& ele);
        bool removeChild(const std::string& name);
        bool removeAttribute(const std::string& name);
        bool removeChildren(const std::string& name = StringEmpty);
        bool removeAttributes();

        operator bool () const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        bool isEmpty() const;
        bool hasChild(const std::string& name) const;
        bool hasAttribute(const std::string& name) const;
        bool hasChildren() const;
        bool hasAttributes() const;

        std::string getName() const;
        std::string getAttribute(const std::string& name = StringEmpty) const;
        XMLElement getChild(const std::string& name = StringEmpty) const;
        XMLElement getNext(const std::string& name = StringEmpty) const;
        std::string getValue() const;

        bool getBool(bool default) const;
        glm::i32 getI32(glm::i32 default) const;
        glm::u32 getU32(glm::u32 default) const;
        glm::f32 getF32(glm::f32 default) const;
        glm::i64 getI64(glm::i64 default) const;
        glm::u64 getU64(glm::u64 default) const;
        glm::f64 getF64(glm::f64 default) const;
        glm::fvec2 getFVec2(glm::fvec2 default) const;
        glm::fvec3 getFVec3(glm::fvec3 default) const;
        glm::fvec4 getFVec4(glm::fvec4 default) const;
        glm::ivec2 getIVec2(glm::ivec2 default) const;
        glm::ivec3 getIVec3(glm::ivec3 default) const;
        glm::ivec4 getIVec4(glm::ivec4 default) const;
        glm::bvec2 getBVec2(glm::bvec2 default) const;
        glm::bvec3 getBVec3(glm::bvec3 default) const;
        glm::bvec4 getBVec4(glm::bvec4 default) const;
        glm::mat2 getMat2(glm::mat2 default) const;
        glm::mat3 getMat3(glm::mat3 default) const;
        glm::mat4 getMat4(glm::mat4 default) const;
        std::string getString(std::string default) const;

        bool getBool(const std::string& name, bool default) const;
        glm::i32 getI32(const std::string& name, glm::i32 default) const;
        glm::u32 getU32(const std::string& name, glm::u32 default) const;
        glm::f32 getF32(const std::string& name, glm::f32 default) const;
        glm::i64 getI64(const std::string& name, glm::i64 default) const;
        glm::u64 getU64(const std::string& name, glm::u64 default) const;
        glm::f64 getF64(const std::string& name, glm::f64 default) const;
        std::string getString(const std::string& name, std::string default) const;

        bool setValue(const std::string& value);
        bool setAttribute(const std::string& name, const std::string& value);

        XMLFile* getFile() const { return m_file.get(); }
        pugi::xml_node getNode() const { return m_node; }

        static const XMLElement EMPTY;

    private:
        WeakPtr<XMLFile> m_file;
        pugi::xml_node m_node;
    };

    class XMLElementIterator
    {
    public:
        XMLElementIterator(XMLFile* file, XMLElement element, pugi::xml_node_iterator iter, pugi::xml_node_iterator end);

        XMLElementIterator operator++ ();
        XMLElementIterator operator++ (glm::i32 junk);

        XMLElement& operator* ();
        XMLElement* operator-> ();

        bool operator == (const XMLElementIterator& rhs);
        bool operator != (const XMLElementIterator& rhs);
    private:
        XMLElement m_element;
        WeakPtr<XMLFile> m_file;
        pugi::xml_node_iterator m_iter;
        pugi::xml_node_iterator m_end;
    };
}