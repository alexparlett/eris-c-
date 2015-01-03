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
    class SharedArrayPtr
    {
        template<typename K>
        friend class WeakArrayPtr;

    public:
        SharedArrayPtr() :
            m_ptr(0),
            m_ref_count(0)
        {
        }

        SharedArrayPtr(const SharedArrayPtr<T>& rhs) :
            m_ptr(rhs.m_ptr),
            m_ref_count(rhs.m_ref_count)
        {
            incrementRef();
        }

        explicit SharedArrayPtr(T* ptr) :
            m_ptr(ptr),
            m_ref_count(new RefCount())
        {
            incrementRef();
        }

        ~SharedArrayPtr()
        {
            releaseRef();
        }

        SharedArrayPtr<T>& operator = (const SharedArrayPtr<T>& rhs)
        {
            if (m_ptr == rhs.m_ptr)
                return *this;

            releaseRef();
            m_ptr = rhs.m_ptr;
            m_ref_count = rhs.m_ref_count;
            incrementRef();

            return *this;
        }

        SharedArrayPtr<T>& operator = (T* ptr)
        {
            if (m_ptr == ptr)
                return *this;

            releaseRef();

            if (ptr)
            {
                m_ptr = ptr;
                m_ref_count = new RefCount();
                incrementRef();
            }

            return *this;
        }

        T* operator -> () const { ERIS_ASSERT(m_ptr); return m_ptr; }
        T& operator * () const { ERIS_ASSERT(m_ptr); return *m_ptr; }
        T& operator[] (const int index) { ERIS_ASSERT(m_ptr); return m_ptr[index]; }
        
        operator T* () const { return m_ptr; }

        std::size_t operator () () const { return (size_t) m_ptr / sizeof(T); }

        bool operator == (const SharedPtr<T>& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator != (const SharedPtr<T>& rhs) const { return m_ptr != rhs.m_ptr; }
        bool operator < (const SharedPtr<T>& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator >(const SharedPtr<T>& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator >= (const SharedPtr<T>& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator <= (const SharedPtr<T>& rhs) const { return m_ptr <= rhs.m_ptr; }

        T* get() const { return m_ptr; }
        void reset() { releaseRef(); }
        bool null() const { return m_ptr == nullptr; }

        glm::i32 refs() const { return m_ref_count ? m_ref_count->m_refs : 0; }
        glm::i32 weakRefs() const { return m_ref_count ? m_ref_count->m_weak_refs : 0; }

    private:
        template<typename K> SharedArrayPtr<T>& operator = (const SharedArrayPtr<K>& rhs) = delete;

        void incrementRef()
        {
            if (m_ref_count)
            {
                ERIS_ASSERT(m_ref_count->m_refs >= 0);
                m_ref_count->m_refs++;
            }
        }

        void releaseRef()
        {
            if (m_ref_count)
            {
                ERIS_ASSERT(m_ref_count->m_refs > 0);
                m_ref_count->m_refs--;
                if (!m_ref_count->m_refs)
                {
                    m_ref_count->m_refs = -1;
                    delete[] m_ptr;
                }

                if (m_ref_count->m_refs < 0 && !m_ref_count->m_weak_refs)
                    delete m_ref_count;
            }
        }

        T* m_ptr;
        RefCount* m_ref_count;
    };

    template<typename T>
    class WeakArrayPtr
    {
    public:
        WeakArrayPtr() :
            m_ptr(0),
            m_ref_count(0)
        {
        }

        WeakArrayPtr(const WeakArrayPtr<T>& rhs) :
            m_ptr(rhs.m_ptr),
            m_ref_count(rhs.m_ref_count)
        {
            incrementRef();
        }

        WeakArrayPtr(const SharedArrayPtr<T>& rhs) :
            m_ptr(rhs.m_ptr),
            m_ref_count(rhs.m_ref_count)
        {
            incrementRef();
        }

        ~WeakArrayPtr()
        {
            releaseRef();
        }

        WeakArrayPtr<T>& operator = (const WeakArrayPtr<T>& rhs)
        {
            if (m_ptr == rhs.get() && m_ref_count == rhs.m_ref_count)
                return *this;

            releaseRef();
            m_ptr = rhs.m_ptr;
            m_ref_count = rhs.m_ref_count;
            incrementRef();

            return *this;
        }

        WeakArrayPtr<T>& operator = (const SharedArrayPtr<T>& rhs)
        {
            if (m_ptr == rhs.get() && m_ref_count == rhs.m_ref_count)
                return *this;

            releaseRef();
            m_ptr = rhs.m_ptr;
            m_ref_count = rhs.m_ref_count;
            incrementRef();

            return *this;
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

        T& operator[] (const int index) 
        { 
            T* raw = get();
            ERIS_ASSERT(raw);
            return raw[index];
        }

        operator T* () const { return get(); }

        std::size_t operator () () const { return (size_t) m_ptr / sizeof(T); }

        bool operator == (const WeakArrayPtr<T>& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator != (const WeakArrayPtr<T>& rhs) const { return m_ptr != rhs.m_ptr; }
        bool operator < (const WeakArrayPtr<T>& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator >(const WeakArrayPtr<T>& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator >= (const WeakArrayPtr<T>& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator <= (const WeakArrayPtr<T>& rhs) const { return m_ptr <= rhs.m_ptr; }

        T* get() const
        {
            if (expired())
                return nullptr;

            return m_ptr;
        }

        SharedArrayPtr<T> lock() const
        {
            if (expired())
                return SharedArrayPtr<T>();

            return SharedArrayPtr<T>(m_ptr, m_ref_count);
        }

        void reset() { releaseRef(); }
        bool null() const { return m_ref_count == nullptr; }
        bool expired() const { return m_ref_count ? m_ref_count->m_refs < 0 : true; }

        glm::i32 refs() const { return m_ref_count ? m_ref_count->m_refs : 0; }
        glm::i32 weakRefs() const { return m_ref_count ? m_ref_count->m_weak_refs : 0; }

    private:
        template<typename K> WeakArrayPtr<T>& operator = (const WeakArrayPtr<K>& rhs) = delete;

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
                ERIS_ASSERT(m_ref_count->m_weak_refs >= 0);

                if (m_ref_count->m_weak_refs > 0)
                    m_ref_count->m_weak_refs--;

                if (expired() && !m_ref_count->m_weak_refs)
                    delete m_ref_count;
            }
        }

        T* m_ptr;
        RefCount* m_ref_count;
    };
}