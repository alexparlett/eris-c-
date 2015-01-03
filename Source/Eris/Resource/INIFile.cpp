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

#include "INIFile.h"

#include "Core/Log.h"
#include "IO/File.h"
#include "Memory/ArrayPointers.h"

#include <cstring>
#include <sstream>

namespace Eris
{
    INIFile::INIFile(Context* context) :
        Resource(context)
    {
    }

    INIFile::~INIFile()
    {
    }

    bool INIFile::load(Deserializer& deserializer)
    {
        std::size_t dsSize = deserializer.getSize();

        SharedArrayPtr<char> buffer = SharedArrayPtr<char>(dsSize);
        deserializer.read(buffer.get(), dsSize);
        
        char* line = nullptr;
        char* next_line = nullptr;
        std::string current_section = StringEmpty;
            
        line = strtok_s(buffer.get(), "\n\0", &next_line);
        while (line)
        {
            if (line[0] == '[')
            {
                std::stringstream section;

                for (glm::uint i = 1; i < strlen(line); i++)
                {
                    if (line[i] == ']')
                        break;

                    section << line[i];
                }

                m_sections[section.str()] = INISection();
                current_section = section.str();
            }
            else
            {
                char* key = nullptr;
                char* value = nullptr;

                key = strtok_s(line, "=\n\0", &value);
                m_sections[current_section].m_values[key] = value;
            }

            line = strtok_s(nullptr, "\n\0", &next_line);
        }

        return true;
    }

    bool INIFile::save(Serializer& serializer)
    {
        return true;
    }
}