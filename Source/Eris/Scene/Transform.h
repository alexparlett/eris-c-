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
    enum class TransformSpace : glm::u8
    {
        LOCAL,
        WORLD
    };

    class Transform : public Component
    {
    public:
        static const Up = glm::vec3( 0.f, 1.f, 0.f );
        static const Forward = glm::vec3( 0.f, 0.f, -1.f );
        static const Right = glm::vec3( 1.f, 0.f, 0.f );

        Transform(Context* context, Node* node);
        virtual ~Transform();

        virtual void load(const JsonElement* src) const;
        virtual void save(JsonElement* dest) const;

        void lookAt(const glm::vec3& position, const glm::vec3& up = Transform::UP);
        void rotate(const glm::vec3& eulerAngles, const TransformSpace space = TransformSpace::LOCAL);

        void setLocalPosition(const glm::vec3& position);
        void setLocalRotation(const glm::quat& rotation);
        void setLocalScale(glm::f32 scalar);
        void setLocalScale(const glm::vec3& scale);
        void setLocalEulerAngles(const glm::vec3& eulerAngles);
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::quat& rotation);
        void setScale(glm::f32 scalar);
        void setScale(const glm::vec3& scale);
        void setEulerAngles( const glm::vec3& eurlerAngles );

        const glm::vec3& getLocalPosition() const { return m_local_position; }
        const glm::quat& getLocalRotation() const { return m_local_rotation; }
        const glm::vec3& getLocalScale() const { return m_local_scale; }
        glm::vec3 getLocalEulerAngles() const;
        glm::vec3 getLocalForward() const;
        glm::vec3 getLocalUp() const;
        glm::vec3 getLocalRight() const;
        glm::f32 getLocalRoll() const;
        glm::f32 getLocalPitch() const;
        glm::f32 getLocalYaw() const;
        glm::mat4 getLocalTransform() const;
        glm::vec3 getPosition() const;
        glm::quat getRotation() const;
        glm::vec3 getScale() const;
        glm::vec3 getEulerAngles() const;
        glm::vec3 getForward() const;
        glm::vec3 getUp() const;
        glm::vec3 getRight() const;
        glm::f32 getRoll() const;
        glm::f32 getPitch() const;
        glm::f32 getYaw() const;
        glm::mat4 getTransform() const { return m_world_transform; }

        void invalidateTransform();

    private:
        glm::u64 m_id;
        glm::mat4 m_world_transform;
        glm::vec3 m_local_position;
        glm::quat m_local_rotation;
        glm::vec3 m_local_scale;
    };
}