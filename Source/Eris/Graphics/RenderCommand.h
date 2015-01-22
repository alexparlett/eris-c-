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

#include "Material.h"
#include "Model.h"
#include "ShaderProgram.h"

#include "Memory/RefCounted.h"

namespace Eris
{
    struct RenderKey
    {
        glm::u64 key = 0;
        glm::u64 target; // 2
        glm::u64 target_layer; // 2
        glm::u64 transparency; // 2
        glm::u64 command; // 1
        glm::u64 extra; // 3
        glm::u64 depth; // 24
        glm::u64 material; // 32

        glm::u64 operator()();
    };

    struct RenderCommand : public RefCounted
    {
        RenderKey key;

        virtual void operator()(Graphics* graphics, const RenderKey* queue_id) = 0;
    };

    struct ClearColorCommand : public RenderCommand
    {
        virtual void operator()(Graphics* graphics, const RenderKey* last_key);

        glm::vec4 color;
    };

    struct ClearCommand : public RenderCommand
    {
        virtual void operator()(Graphics* graphics, const RenderKey* last_key);

        glm::u32 bitfield;
    };

    struct EnableCommand : public RenderCommand
    {
        virtual void operator()(Graphics* graphics, const RenderKey* last_key);

        glm::u32 capability;
    };

    struct DisableCommand : public RenderCommand
    {
        virtual void operator()(Graphics* graphics, const RenderKey* last_key);

        glm::u32 capability;
    };

    struct Draw3DCommand : public RenderCommand
    {
        virtual void operator()(Graphics* graphics, const RenderKey* last_key);

        SharedPtr<Model> model;
        SharedPtr<Material> material;
        std::list<ShaderUniform> uniforms;
    };
}
