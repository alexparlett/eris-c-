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

#include "XMLFile.h"

#include "Core/Log.h"
#include "IO/File.h"
#include "Memory/ArrayPointers.h"

namespace Eris
{
    class XMLWriter : public pugi::xml_writer
    {
    public:
        XMLWriter(Serializer& serializer) :
            m_serializer(serializer)
        {
        }

        virtual void write(const void* data, std::size_t size) override
        {
            std::size_t out_size = m_serializer.write(data, size);

            m_success = out_size == size;
        }

        bool success() const { return m_success; }

    private:
        Serializer& m_serializer;
        bool m_success;
    };

    XMLFile::XMLFile(Context* context) :
        Resource(context),
        m_doc(new pugi::xml_document())
    {
    }

    XMLFile::~XMLFile()
    {
        if (m_doc)
        {
            delete m_doc;
            m_doc = nullptr;
        }
    }

    bool XMLFile::load(Deserializer& deserializer)
    {
        std::size_t ds_size = deserializer.getSize();
        pugi::allocation_function alloc = pugi::get_memory_allocation_function();

        void* buffer = alloc(ds_size);
        std::size_t read = deserializer.read(buffer, ds_size);
        if (read != ds_size)
        {
            return false;
        }

        pugi::xml_parse_result result = m_doc->load_buffer_inplace_own(buffer, ds_size);
        if (!result)
        {
            Log::errorf("Failed parsing XMLFile %s due to %s", deserializer.getPath().string().c_str(), result.description());
            m_doc->reset();
            return false;
        }

        return true;
    }

    bool XMLFile::save(Serializer& serializer)
    {
        XMLWriter writer(serializer);
        m_doc->save(writer);
        return writer.success();
    }

    Eris::XMLElement XMLFile::createRoot(const std::string& name)
    {
        m_doc->reset();
        m_doc->append_child(pugi::xml_node_type::node_declaration);
        pugi::xml_node root = m_doc->append_child(pugi::xml_node_type::node_document);

        if (root.empty())
            return XMLElement::EMPTY;

        root.set_name(name.c_str());

        return XMLElement(this, root);
    }

    void XMLFile::patch(const XMLElement& element)
    {
        for (auto patch : element.getNode())
        {
            pugi::xml_attribute sel = patch.attribute("sel");
            if (sel.empty())
            {
                Log::error("XML Patch failed due to node not having a sel attribute.");
                continue;
            }

            pugi::xpath_node original = m_doc->select_node(sel.value());
            if (!original)
            {
                Log::errorf("XML Patch failed with bad select: %s.", sel.value());
                continue;
            }

            if (strcmp(patch.name(), "add") == 0)
                patchAdd(patch, original);
            else if (strcmp(patch.name(), "replace") == 0)
                patchReplace(patch, original);
            else if (strcmp(patch.name(), "remove") == 0)
                patchRemove(original);
            else
                Log::error("XMLFiles used for patching should only use 'add', 'replace' or 'remove' elements.");
        }
    }

    Eris::XMLElement XMLFile::getRoot(const std::string& name)
    {
        pugi::xml_node node = m_doc->document_element();

        if (node.empty())
            return XMLElement::EMPTY;

        if (!name.empty() && name != node.name())
            return XMLElement::EMPTY;
        
        return XMLElement(this, node);
    }

    void XMLFile::patchAdd(const pugi::xml_node& add, const pugi::xpath_node& original)
    {
        // If not a node, log an error
        if (original.attribute())
        {
            Log::errorf("XML Patch failed calling Add due to not selecting a node, %s attribute was selected.", original.attribute().name());
            return;
        }

        // If no type add node, if contains '@' treat as attribute
        pugi::xml_attribute type = add.attribute("type");
        if (!type || strlen(type.value()) <= 0)
            patchAddNode(add, original);
        else if (type.value()[0] == '@')
            patchAddAttribute(add, original);
    }

    void XMLFile::patchReplace(const pugi::xml_node& replace, const pugi::xpath_node& original)
    {
        // If no attribute but node then its a node, otherwise its an attribute or null
        if (!original.attribute() && original.node())
        {
            pugi::xml_node parent = original.node().parent();

            parent.insert_copy_before(replace.first_child(), original.node());
            parent.remove_child(original.node());
        }
        else if (original.attribute())
        {
            original.attribute().set_value(replace.child_value());
        }
    }

    void XMLFile::patchRemove(const pugi::xpath_node& original)
    {
        // If no attribute but node then its a node, otherwise its an attribute or null
        if (!original.attribute() && original.node())
        {
            pugi::xml_node parent = original.parent();
            parent.remove_child(original.node());
        }
        else if (original.attribute())
        {
            pugi::xml_node parent = original.parent();
            parent.remove_attribute(original.attribute());
        }
    }

    void XMLFile::patchAddNode(const pugi::xml_node& node, pugi::xpath_node original)
    {
        // If pos is null, append or prepend add as a child, otherwise add before or after, the default is to append as a child
        pugi::xml_attribute pos = node.attribute("pos");
        if (!pos || strlen(pos.value()) <= 0 || strcmp(pos.value(), "append") == 0)
        {
            auto start = node.begin();
            auto end = node.end();

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the first node of the nodes to add
            if (patchCombineText(node.first_child(), original.node().last_child(), false))
                start++;

            for (; start != end; start++)
                original.node().append_copy(*start);
        }
        else if (strcmp(pos.value(), "prepend") == 0)
        {
            auto start = node.begin();
            auto end = node.end();

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the last node of the nodes to add
            if (patchCombineText(node.last_child(), original.node().first_child(), true))
                end--;

            pugi::xml_node pos = original.node().first_child();
            for (; start != end; start++)
                original.node().insert_copy_before(*start, pos);
        }
        else if (strcmp(pos.value(), "before") == 0)
        {
            auto start = node.begin();
            auto end = node.end();

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the first node of the nodes to add
            if (patchCombineText(node.first_child(), original.node().previous_sibling(), false))
                start++;

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the last node of the nodes to add
            if (patchCombineText(node.last_child(), original.node(), true))
                end--;

            for (; start != end; start++)
                original.parent().insert_copy_before(*start, original.node());
        }
        else if (strcmp(pos.value(), "after") == 0)
        {
            auto start = node.begin();
            auto end = node.end();

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the first node of the nodes to add
            if (patchCombineText(node.first_child(), original.node(), false))
                start++;

            // There can not be two consecutive text nodes, so check to see if they need to be combined
            // If they have been we can skip the last node of the nodes to add
            if (patchCombineText(node.last_child(), original.node().next_sibling(), true))
                end--;

            pugi::xml_node pos = original.node();
            for (; start != end; start++)
                pos = original.parent().insert_copy_after(*start, pos);
        }
    }

    void XMLFile::patchAddAttribute(const pugi::xml_node& node, pugi::xpath_node original)
    {
        pugi::xml_attribute attribute = node.attribute("type");

        if (!node.first_child() || node.first_child().type() != pugi::node_pcdata)
        {
            Log::errorf("XML Patch failed calling Add due to attempting to add non text to an attribute for %s.", attribute.value());
            return;
        }

        std::string name(attribute.value());
        name = name.substr(1);

        pugi::xml_attribute newAttribute = original.node().append_attribute(name.c_str());
        newAttribute.set_value(node.child_value());
    }

    bool XMLFile::patchCombineText(const pugi::xml_node& node, pugi::xml_node original, bool prepend)
    {
        if (!node || !original)
            return false;

        if ((node.type() == pugi::node_pcdata && original.type() == pugi::node_pcdata) ||
            (node.type() == pugi::node_cdata && original.type() == pugi::node_cdata))
        {
            if (prepend)
                original.set_value(std::string_format("%s%s", node.value(), original.value()).c_str());
            else
                original.set_value(std::string_format("%s%s", original.value(), node.value()).c_str());

            return true;
        }

        return false;
    }

}