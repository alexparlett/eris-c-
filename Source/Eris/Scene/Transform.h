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

#include "Component.h"

namespace Eris
{
    class Transform : public Component
    {
    public:
        Transform(Context* context);
        Transform(Context* context, Node* node);
        virtual ~Transform();

        virtual void load(const JsonElement* src) const;
        virtual void save(JsonElement* dest) const;

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::quat& rotation);
        void setScale(glm::f32 scalar);
        void setScale(const glm::vec3& scale);
        void setWorldPosition(const glm::vec3& position);
        void setWorldRotation(const glm::quat& rotation);
        void setWorldScale(glm::f32 scalar);
        void setWorldScale(const glm::vec3& scale);

        const glm::vec3& position() const { return m_position; }
        const glm::quat& rotation() const { return m_rotation; }
        const glm::vec3& scale() const { return m_scale; }
        glm::vec3 forward() const;
        glm::vec3 up() const;
        glm::vec3 right() const;
        glm::mat4 transform() const;
        glm::vec3 worldPosition() const;
        glm::quat worldRotation() const;
        glm::vec3 worldScale() const;
        glm::vec3 worldForward() const;
        glm::vec3 worldUp() const;
        glm::vec3 worldRight() const;
        glm::mat4 worldTransform() const { return m_world_transform; }

        void invalidateWorldTransform();

    private:
        glm::u64 m_id;
        glm::mat4 m_world_transform;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };
}