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

#include "RefCounted.h"

namespace Eris
{

    RefCounted::RefCounted() :
        m_ref_count(new RefCounter())
    {
        m_ref_count->m_weak_refs++;
    }

    RefCounted::~RefCounted()
    {
        ERIS_ASSERT(m_ref_count);
        ERIS_ASSERT(m_ref_count->m_refs == 0);
        ERIS_ASSERT(m_ref_count->m_weak_refs > 0);

        m_ref_count->m_refs = -1;
        m_ref_count->m_weak_refs--;
        if (!m_ref_count->m_weak_refs)
            delete m_ref_count;

        m_ref_count = nullptr;
    }

    void RefCounted::increment()
    {
        ERIS_ASSERT(m_ref_count->m_refs >= 0);
        m_ref_count->m_refs++;
    }

    void RefCounted::release()
    {
        ERIS_ASSERT(m_ref_count->m_refs > 0);
        m_ref_count->m_refs--;
        if (!m_ref_count->m_refs)
            delete this;
    }

    glm::i32 RefCounted::getRefs()
    {
        return m_ref_count->m_refs;
    }

    glm::i32 RefCounted::getWeakRefs()
    {
        return m_ref_count->m_weak_refs - 1;
    }
}