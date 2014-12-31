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

#include <utility>
#include <stdarg.h>
#include <stdlib.h>

#include "Math/Functions.h"

namespace Eris
{
    template<class Type>
    inline Type getAlignedRoundUp(std::size_t alignment, Type value)
    {
        ERIS_ASSERT(powerOfTwo(alignment));
        std::size_t v = (std::size_t) value;
        v = (v + alignment - 1) & ~(alignment - 1);
        return (Type) v;
    }

    template<class Type>
    inline void alignRoundUp(std::size_t alignment, Type& value)
    {
        value = getAlignedRoundUp(alignment, value);
    }

    template<class Type>
    inline Type getAlignedRoundDown(std::size_t alignment, Type value)
    {
        ERIS_ASSERT(powerOfTwo(alignment));
        std::size_t v = (std::size_t) value;
        v &= ~(alignment - 1);
        return (Type) v;
    }

    template<class Type>
    inline void alignRoundDown(std::size_t alignment, Type& value)
    {
        value = getAlignedRoundDown(alignment, value);
    }

    template<class Type>
    inline bool isAligned(std::size_t alignment, Type value)
    {
        return ((std::size_t) value % alignment) == 0;
    }

    template<class T, class... Args>
    inline void construct(T* ptr, Args&&... args)
    {
        ::new (reinterpret_cast<void*>(ptr)) T(forward<Args>(args)...);
    }

    template<class T>
    inline void destruct(T* ptr)
    {
        ptr->~T();
    }
}