//
// Copyright (c) 2013-2015 the Eris project.
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

namespace Eris
{
    class StringHash
    {
    public:
        StringHash();
        StringHash(const std::string& value);
        StringHash(const char* value);
        StringHash(const StringHash& rhs);

        void operator = (const std::string& value);
        void operator = (const char* value);
        void operator = (const StringHash& rhs);

        bool operator == (const StringHash& rhs) const;
        bool operator != (const StringHash& rhs) const;
        bool operator < (const StringHash& rhs) const;
        bool operator > (const StringHash& rhs) const;
        bool operator <= (const StringHash& rhs) const;
        bool operator >= (const StringHash& rhs) const;

        std::size_t operator()() const;

        static const StringHash ZERO;

    private:
        std::size_t m_value;
    };
}

namespace std
{
    template<>
    struct hash<Eris::StringHash>
    {
        typedef Eris::StringHash argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& value) const
        {
            return value();
        }
    };
}