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

#include "Core/Context.h"
#include "Core/Object.h"

namespace Eris
{
    enum class GenerationState : glm::u8
    {
        RENDERER,
        LOADER,
        NONE
    };

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoords;
    };

    class Mesh : public Object
    {
    public:
        Mesh(Context* context);

        void draw();
        void compile();

        void setVertices(std::vector<Vertex> vertices);
        void setIndices(std::vector<glm::u32> indices);

        glm::u32 getVao() const { return m_vao; }
        glm::u32 getVbo() const { return m_vbo; }
        glm::u32 getEbo() const { return m_ebo; }
        std::vector<glm::u32> getIndices() const { return m_indices; }
        std::vector<Vertex> getVertices() const { return m_vertices; }

    private:
        GenerationState m_gen_state;
        glm::u32 m_vao;
        glm::u32 m_vbo;
        glm::u32 m_ebo;
        std::vector<glm::u32> m_indices;
        std::vector<Vertex> m_vertices;
    };
}