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

#include "Resource.h"
#include "XMLElement.h"

namespace Eris
{
    class XMLFile : public Resource
    {
    public:
        XMLFile(Context* context);
        virtual ~XMLFile();

        virtual bool load(Deserializer& deserializer) override;
        virtual bool save(Serializer& serializer) override;

        XMLElement createRoot(const std::string& name);

        void patch(const XMLElement& element);
        void clear() { m_doc->reset(); }

        bool isEmpty() { return m_doc->empty(); }

        XMLElement getRoot(const std::string& name = StringEmpty);
        pugi::xml_document* getDocument() const { return m_doc; }

    private:
        void patchAdd(const pugi::xml_node& add, const pugi::xpath_node& original);
        void patchReplace(const pugi::xml_node& replace, const pugi::xpath_node& original);
        void patchRemove(const pugi::xpath_node& original);
        void patchAddNode(const pugi::xml_node& node, pugi::xpath_node original);
        void patchAddAttribute(const pugi::xml_node& attribute, pugi::xpath_node original);
        bool patchCombineText(const pugi::xml_node& node, pugi::xml_node original, bool prepend);

        pugi::xml_document* m_doc;
    };
}