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

#include "Vector.h"

namespace Eris
{
    template<typename T>
    struct false_wrapper
    {
        false_wrapper(const T& value) : value(value) {}
        operator bool() const { return false; }
        T value;
    };

    template<typename T>
    false_wrapper<T> make_false_wrapper(const T& value)
    {
        return false_wrapper<T>(value);
    }

    // vector support functions

    template <class T>
    Eris::RandomAccessIterator<T> Begin(Eris::Vector<T> &v)
    {
        return v.Begin();
    }
    template <class T>
    Eris::RandomAccessIterator<T> Begin(Eris::Vector<T> *v)
    {
        return v->Begin();
    }

    template <class T>
    Eris::RandomAccessConstIterator<T> Begin(const Eris::Vector<T> &v)
    {
        return v.Begin();
    }
    template <class T>
    Eris::RandomAccessConstIterator<T> Begin(const Eris::Vector<T> *v)
    {
        return v->Begin();
    }

    template <class T>
    Eris::RandomAccessIterator<T> End(Eris::Vector<T> &v)
    {
        return v.End();
    }
    template <class T>
    Eris::RandomAccessIterator<T> End(Eris::Vector<T> *v)
    {
        return v->End();
    }

    template <class T>
    Eris::RandomAccessConstIterator<T> End(const Eris::Vector<T> &v)
    {
        return v.End();
    }
    template <class T>
    Eris::RandomAccessConstIterator<T> End(const Eris::Vector<T> *v)
    {
        return v->End();
    }

    // podvector support functions

    template <class T>
    Eris::RandomAccessIterator<T> Begin(Eris::PODVector<T> &v)
    {
        return v.Begin();
    }
    template <class T>
    Eris::RandomAccessIterator<T> Begin(Eris::PODVector<T> *v)
    {
        return v->Begin();
    }

    template <class T>
    Eris::RandomAccessConstIterator<T> Begin(const Eris::PODVector<T> &v)
    {
        return v.Begin();
    }
    template <class T>
    Eris::RandomAccessConstIterator<T> Begin(const Eris::PODVector<T> *v)
    {
        return v->Begin();
    }

    template <class T>
    Eris::RandomAccessIterator<T> End(Eris::PODVector<T> &v)
    {
        return v.End();
    }
    template <class T>
    Eris::RandomAccessIterator<T> End(Eris::PODVector<T> *v)
    {
        return v->End();
    }

    template <class T>
    Eris::RandomAccessConstIterator<T> End(const Eris::PODVector<T> &v)
    {
        return v.End();
    }
    template <class T>
    Eris::RandomAccessConstIterator<T> End(const Eris::PODVector<T> *v)
    {
        return v->End();
    }

} // namespace Eris

#define foreach(VAL, VALS) \
if (const auto& _foreach_begin = Eris::make_false_wrapper(Eris::Begin(VALS))) {}
 else \
 if (const auto& _foreach_end = Eris::make_false_wrapper(Eris::End(VALS))) {}
 else \
 for (auto it = _foreach_begin.value; it != _foreach_end.value; ++it) \
 if (bool _foreach_flag = false) {}
 else \
 for (VAL = *it; !_foreach_flag; _foreach_flag = true)

#define foreachv(ITER, VAL, VALS) \
 if (const auto& _foreach_begin = Eris::make_false_wrapper(Eris::Begin(VALS))) {}
 else \
 if (const auto& _foreach_end = Eris::make_false_wrapper(Eris::End(VALS))) {}
 else \
 if (int ITER = 0) {}
 else \
 for (auto it = _foreach_begin.value; it != _foreach_end.value; ++it) \
 if (bool _foreach_flag = false) {}
 else \
 for (VAL = *it; !_foreach_flag; ITER++, _foreach_flag = true)
