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

#include "RefCounted.h"

namespace Eris
{
    template<typename T>
    class SharedPtr
    {
    public:
        SharedPtr() :
            m_ptr(nullptr)
        {
        }

        SharedPtr(const SharedPtr<T>& rhs) :
            m_ptr(rhs.m_ptr)
        {
            incrementRef();
        }

        explicit SharedPtr(T* ptr) :
            m_ptr(ptr)
        {
            incrementRef();
        }

        ~SharedPtr()
        {
            releaseRef();
        }

        SharedPtr<T>& operator = (const SharedPtr<T>& rhs)
        {
            if (m_ptr == rhs.m_ptr)
                return *this;

            releaseRef();
            m_ptr = rhs.m_ptr;
            incrementRef();

            return *this;
        }

        SharedPtr<T>& operator = (T* ptr)
        {
            if (m_ptr == ptr)
                return *this;

            releaseRef();
            m_ptr = ptr;
            incrementRef();

            return *this;
        }

        T* operator -> () const { ERIS_ASSERT(m_ptr); return m_ptr; }
        T& operator * () const { ERIS_ASSERT(m_ptr); return *m_ptr; }
        T& operator [] (const glm::i32 index) { ERIS_ASSERT(m_ptr); return m_ptr[index]; }

        operator T* () const { return m_ptr; }

        std::size_t operator () () const { return (size_t) m_ptr / sizeof(T); }

        bool operator == (const SharedPtr<T>& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator != (const SharedPtr<T>& rhs) const { return m_ptr != rhs.m_ptr; }
        bool operator < (const SharedPtr<T>& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator > (const SharedPtr<T>& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator >= (const SharedPtr<T>& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator <= (const SharedPtr<T>& rhs) const { return m_ptr <= rhs.m_ptr; }

        void reset() { releaseRef(); }

        bool isNull() const { return m_ptr == nullptr; }
        T* get() const { return m_ptr; }
        glm::i32 getRefs() const { return m_ptr ? m_ptr->getRefs() : 0; }
        glm::i32 getWeakRefs() const { return m_ptr ? m_ptr->getWeakRefs() : 0; }

    private:
        template<typename U> SharedPtr<T> operator = (const SharedPtr<U>& rhs) = delete;

        void incrementRef()
        {
            if (m_ptr)
                m_ptr->increment();
        }

        void releaseRef()
        {
            if (m_ptr)
            {
                m_ptr->release();
                m_ptr = nullptr;
            }
        }

        T* m_ptr;
    };

    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() :
            m_ptr(nullptr),
            m_ref_count(nullptr)
        {
        }

        WeakPtr(const WeakPtr<T>& rhs) :
            m_ptr(rhs.m_ptr),
            m_ref_count(rhs.m_ref_count)
        {
            incrementRef();
        }
        
        WeakPtr(const SharedPtr<T>& rhs) :
            m_ptr(rhs.get()),
            m_ref_count(rhs.m_ptr->m_ref_count)
        {
            incrementRef();
        }

        explicit WeakPtr(T* ptr) :
            m_ptr(ptr),
            m_ref_count(ptr ? ptr->m_ref_count : nullptr)
        {
            incrementRef();
        }

        ~WeakPtr()
        {
            releaseRef();
        }

        WeakPtr<T>& operator = (const SharedPtr<T>& rhs)
        {
            if (m_ptr == rhs.m_ptr)
                return *this;

            releaseRef();
            m_ptr = rhs.get();
            m_ref_count = m_ptr->m_ref_count;
            incrementRef();

            return *this;
        }

        WeakPtr<T>& operator = (const WeakPtr<T>& rhs)
        {
            if (m_ptr == rhs.m_ptr)
                return *this;

            releaseRef();
            m_ptr = rhs.m_ptr;
            m_ref_count = rhs.m_ref_count;
            incrementRef();

            return *this;
        }

        WeakPtr<T>& operator = (const T* ptr)
        {
            if (m_ptr == ptr)
                return *this;

            releaseRef();
            m_ptr = ptr;
            m_ref_count = ptr ? ptr->m_ref_count : nullptr;
            incrementRef();

            return *this;
        }

        SharedPtr<T> lock() const
        {
            if (isExpired())
                return SharedPtr<T>();
            else
                return SharedPtr<T>(m_ptr);
        }

        T* get() const
        {
            if (isExpired())
                return nullptr;
            else
                return m_ptr;
        }

        T* operator -> () const
        {
            T* raw = get();
            ERIS_ASSERT(raw);
            return raw;
        }

        T& operator * () const
        {
            T* raw = get();
            ERIS_ASSERT(raw);
            return *raw;
        }

        T& operator [] (const glm::i32 index)
        {
            T* raw = get();
            ERIS_ASSERT(raw);
            return raw[index];
        }

        bool operator == (const WeakPtr<T>& rhs) const { return m_ptr == rhs.m_ptr && m_ref_count == rhs.m_ref_count; }
        bool operator != (const WeakPtr<T>& rhs) const { return m_ptr != rhs.m_ptr && m_ref_count != rhs.m_ref_count; }
        bool operator < (const WeakPtr<T>& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator > (const WeakPtr<T>& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator >= (const WeakPtr<T>& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator <= (const WeakPtr<T>& rhs) const { return m_ptr <= rhs.m_ptr; }

        operator T* () const { return get(); }

        std::size_t operator () () const { return (size_t) m_ptr / sizeof(T); }

        void reset() { releaseRef(); }

        bool isNull() const { return m_ref_count == nullptr; }
        bool isExpired() const { return m_ref_count ? m_ref_count->m_refs < 0 : true; }
        glm::i32 getRefs() const { return m_ref_count ? m_ref_count->m_refs : 0; }
        glm::i32 getWeakRefs() const { return m_ref_count ? m_ref_count->m_weak_refs : 0; }

    private:
        template<typename U> WeakPtr<T> operator = (const WeakPtr<U>& rhs) = delete;

        void incrementRef()
        {
            if (m_ref_count)
            {
                ERIS_ASSERT(m_ref_count->m_weak_refs >= 0);
                m_ref_count->m_weak_refs++;
            }
        }

        void releaseRef()
        {
            if (m_ref_count)
            {
                ERIS_ASSERT(m_ref_count);
                m_ref_count->m_weak_refs--;

                if (isExpired() && !m_ref_count->m_weak_refs)
                    delete m_ref_count;
            }

            m_ptr = nullptr;
            m_ref_count = nullptr;
        }

        T* m_ptr;
        RefCounter* m_ref_count;
    };
}