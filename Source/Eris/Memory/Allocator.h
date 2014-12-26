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

#include "RefCounted.h"
#include "Memory.h"
#include "Pointers.h"

#include <atomic>
#include <utility>
#include <cstddef>

namespace Eris
{
    template<class T, class TPool>
    class GenericPoolAllocator : public RefCounted
    {
        template<class K, class KPool>
        friend class GenericPoolAllocator;

    public:
        using size_type = std::size_t;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;

        using propagate_on_container_move_assignment = std::true_type;

        template<class K>
        struct rebind
        {
            using other = GenericPoolAllocator < K, TPool > ;
        };

    public:
        GenericPoolAllocator() :
            m_pool(nullptr)
        {
        }

        GenericPoolAllocator(const GenericPoolAllocator& rhs)
        {
            *this = rhs;
        }

        template<class K>
        GenericPoolAllocator(const GenericPoolAllocator<K, TPool>& rhs)
        {
            *this = rhs;
        }

        template<class... TArgs>
        explicit GenericPoolAllocator(TArgs&&... args) :
            m_pool(new TPool(std::forward<TArgs>(args)...))
        {
        }

        ~GenericPoolAllocator()
        {
            clear();
        }

        GenericPoolAllocator& operator = (const GenericPoolAllocator& rhs)
        {
            copy(rhs);
            return *this;
        }

        template<class K>
        GenericPoolAllocator operator = (const GenericPoolAllocator<K, TPool>& rhs)
        {
            copy(rhs);
            return *this;
        }

        pointer address(reference ref) const
        {
            return &ref;
        }

        const_pointer address(const_reference ref) const
        {
            return &ref;
        }

        pointer allocate(size_type n, const void* hint = nullptr)
        {
            (void) hint;

            size_type size = n * sizeof(value_type);

            std::size_t alignment = (hint != nullptr)
                ? *reinterpret_cast<const std::size_t*>(hint)
                : __alignof(value_type);

            void* out = m_pool->allocate(size, alignment);

            if (!out)
            {
                //todo: LOG ERROR("Allocation failed. There is not enough room");
            }

            return reinterpret_cast<pointer>(out);
        }

        void deallocate(void* p, size_type n)
        {
            (void) n;
            m_pool->free(p);
        }

        void construct(pointer p, const T& val)
        {
            new ((T*) p) T(val);
        }

        template<class K, class... Args>
        void construct(K* p, Args&&... args)
        {
            ::new((void *) p) K(std::forward<Args>(args)...);
        }

        void destroy(pointer p)
        {
            ERIS_ASSERT(!p);
            p->~T();
        }

        template<class K>
        void destroy(K* p)
        {
            ERIS_ASSERT(!p);
            p->~K();
        }

        size_type max_size() const
        {
            return std::numeric_limits<std::size_t>::max();
        }

        const TPool& getMemoryPool() const
        {
            ERIS_ASSERT(m_pool);
            return *m_pool;
        }

        TPool& getMemoryPool()
        {
            ERIS_ASSERT(m_pool);
            return *m_pool;
        }

        template<class K, class... Args>
        K* newInstance(Args&&... args)
        {
            typename rebind<K>::other alloc(*this);

            K* ptr = alloc.allocate(1);
            if (ptr)
            {
                alloc.construct(ptr, std::forward<Args>(args)...);
            }

            return ptr;
        }

        template<typename K>
        K* newArray(size_type n)
        {
            typename rebind<K>::other alloc(*this);

            K* ptr = alloc.allocate(n);
            if (ptr)
            {
                for (size_type i = 0; i < n; i++)
                {
                    alloc.construct(&ptr[i]);
                }
            }

            return ptr;
        }

        template<typename K>
        K* newArray(size_type n, const K& v)
        {
            typename rebind<K>::other alloc(*this);

            K* ptr = alloc.allocate(n);
            if (ptr)
            {
                for (size_type i = 0; i < n; i++)
                {
                    alloc.construct(&ptr[i], v);
                }
            }

            return ptr;
        }

        template<typename K>
        void deleteInstance(K* ptr)
        {
            typename rebind<K>::other alloc(*this);

            if (ptr != nullptr)
            {
                alloc.destroy(ptr);
                alloc.deallocate(ptr, 1);
            }
        }

        template<typename K>
        void deleteArray(K* ptr, size_type n)
        {
            typename rebind<K>::other alloc(*this);

            if (ptr)
            {
                // Call the destructors
                for (size_type i = 0; i < n; i++)
                {
                    alloc.destroy(&ptr[i]);
                }

                alloc.deallocate(ptr, n);
            }
            else
            {
                ERIS_ASSERT(n == 0);
            }
        }

    private:
        template<typename K>
        void copy(const GenericPoolAllocator<K, TPool>& b)
        {
            clear();
            if (b.m_pool)
            {
                m_pool = b.m_pool;
            }
        }

        void clear()
        {
            if (m_pool)
            {
                m_pool.reset();
                m_pool = nullptr;
            }
        }

        SharedPtr<TPool> m_pool;
    };

    template<class T>
    using GenericMemoryPoolAllocator = GenericPoolAllocator<T, BaseMemoryPool>;

    template<class T>
    using HeapAllocator = GenericPoolAllocator<T, HeapMemoryPool>;

    template<class T>
    using StackAllocator = GenericPoolAllocator<T, StackMemoryPool>;

    template<class T>
    using ChainAllocator = GenericPoolAllocator<T, ChainMemoryPool> ;
}