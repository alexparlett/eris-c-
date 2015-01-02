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

#include "Functions.h"
#include "Memory.h"

#include "Core/Log.h"

namespace Eris
{

    BaseMemoryPool::BaseMemoryPool(Type type) :
        RefCounted(),
        NonCopyable(),
        m_type(type),
        m_allocations(0)
    {
    }

    BaseMemoryPool::~BaseMemoryPool()
    {

    }

    void* BaseMemoryPool::allocate(std::size_t size, std::size_t alignment_bits)
    {
        void* out = nullptr;
        switch (m_type)
        {
        case Type::HEAP:
            out = static_cast<HeapMemoryPool*>(this)->allocate(size, alignment_bits);
            break;
        case Type::STACK:
            out = static_cast<StackMemoryPool*>(this)->allocate(size, alignment_bits);
            break;
        case Type::CHAIN:
            out = static_cast<ChainMemoryPool*>(this)->allocate(size, alignment_bits);
            break;
        default:
            ERIS_ASSERT(0);
        }

        return out;
    }

    void BaseMemoryPool::free(void* ptr)
    {
        switch (m_type)
        {
        case Type::HEAP:
            static_cast<HeapMemoryPool*>(this)->free(ptr);
            break;
        case Type::STACK:
            static_cast<StackMemoryPool*>(this)->free(ptr);
            break;
        case Type::CHAIN:
            static_cast<ChainMemoryPool*>(this)->free(ptr);
            break;
        default:
            ERIS_ASSERT(0);
        }
    }


    HeapMemoryPool::HeapMemoryPool() :
        BaseMemoryPool(Type::HEAP)
    {

    }

    HeapMemoryPool::~HeapMemoryPool()
    {
        if (m_allocations != 0)
        {
            Log::warn("Destruct called with allocations undeleted");
        }
    }

    void* HeapMemoryPool::allocate(std::size_t size, std::size_t alignment_bits)
    {
        void* out = _aligned_malloc(size, alignment_bits);
        if (out)
        {
            ERIS_ASSERT(isAligned(alignment_bits, out));
            m_allocations++;
        }
        else
            Log::error("Not enough memory for allocation");

        return out;
    }

    void HeapMemoryPool::free(void* ptr)
    {
        m_allocations--;
        _aligned_free(ptr);
    }

    StackMemoryPool::StackMemoryPool(std::size_t size, bool deallocation_flag, std::size_t alignment_bits) :
        BaseMemoryPool(Type::STACK),
        m_deallocation_flag(deallocation_flag),
        m_alignment_bits(alignment_bits),
        m_head(nullptr),
        m_memory(nullptr),
        m_mem_size(size)
    {
        ERIS_ASSERT(size > 0);
        ERIS_ASSERT(alignment_bits > 0);

        m_mem_size = getAlignedRoundUp(alignment_bits, size);
        m_memory = reinterpret_cast<glm::u8*>(_aligned_malloc(m_mem_size, alignment_bits));
        if (m_memory)
        {
            m_head = m_memory;
            m_header_size = getAlignedRoundUp(alignment_bits, sizeof(MemoryBlockHeader));
        }
        else
        {
            Log::error("Not enough memory for allocation");
        }
    }

    StackMemoryPool::~StackMemoryPool()
    {
        if (m_memory)
            _aligned_free(m_memory);
    }

    void* StackMemoryPool::allocate(std::size_t size, std::size_t alignment_bits)
    {
        ERIS_ASSERT(alignment_bits <= m_alignment_bits);
        (void) alignment_bits;

        size = getAlignedRoundUp(m_alignment_bits, size + m_header_size);

        glm::u8* out = m_head.fetch_add(size);
        if (out + size <= m_memory + m_mem_size)
        {
            MemoryBlockHeader* header = reinterpret_cast<MemoryBlockHeader*>(out);
            std::size_t size32 = size;
            memcpy(&header->size[0], &size32, sizeof(std::size_t));

            out += m_header_size;
            
            ERIS_ASSERT(isAligned(m_alignment_bits, out));

            m_allocations++;
        }
        else
        {
            Log::error("Not enough memory for allocation");
            m_head = out;
            out = nullptr;
        }

        return out;
    }

    void StackMemoryPool::free(void* ptr)
    {
        ERIS_ASSERT(ptr && isAligned(m_alignment_bits, ptr));
        ERIS_ASSERT(m_memory);

        glm::u8* real_ptr = reinterpret_cast<glm::u8*>(ptr) - m_header_size;

        ERIS_ASSERT(real_ptr >= m_memory);

        MemoryBlockHeader* header = reinterpret_cast<MemoryBlockHeader*>(real_ptr);
        std::size_t size;
        memcpy(&size, &header->size[0], sizeof(std::size_t));

        ERIS_ASSERT(real_ptr + size <= m_memory + m_mem_size);

        glm::u8* expected = real_ptr + size;
        glm::u8* desired = real_ptr;

        bool exchange = m_head.compare_exchange_strong(expected, desired);

        m_allocations--;

        if (!m_deallocation_flag && !exchange)
        {
            Log::warn("Deallocated in wrong order");
        }
    }

    StackMemoryPool::Snapshot StackMemoryPool::takeSnapshot() const
    {
        return m_head.load();
    }

    void StackMemoryPool::resetUsingSnapshot(StackMemoryPool::Snapshot s)
    {
        ERIS_ASSERT(static_cast<glm::u8*>(s) >= m_memory);
        ERIS_ASSERT(static_cast<glm::u8*>(s) <= m_memory + m_mem_size);

        m_head.store(static_cast<glm::u8*>(s));
    }

    void StackMemoryPool::reset()
    {
        ERIS_ASSERT(m_memory);
        m_head = m_memory;
        m_allocations = 0;
    }

    ChainMemoryPool::ChainMemoryPool(std::size_t initial_chunk_size, std::size_t max_chunk_size, ChunkGrowMethod chunk_grow_method, std::size_t chunk_alloc_step, std::size_t alignment_bits) :
        BaseMemoryPool(Type::CHAIN),
        m_alignment_bits(alignment_bits),
        m_head_chunk(nullptr),
        m_init_size(initial_chunk_size),
        m_max_size(max_chunk_size),
        m_tail_chunk(nullptr),
        m_step(chunk_alloc_step),
        m_method(chunk_grow_method),
        m_lock()
    {
        ERIS_ASSERT(m_init_size > 0);

        if (m_method == ChunkGrowMethod::FIXED)
        {
            ERIS_ASSERT(m_step == 0);
            ERIS_ASSERT(m_init_size == m_max_size);
        }

        if (m_method == ChunkGrowMethod::ADD || m_method == ChunkGrowMethod::MULTIPLY)
            ERIS_ASSERT(m_init_size < m_max_size);
    }

    ChainMemoryPool::~ChainMemoryPool()
    {
        if (m_allocations != 0)
        {
            Log::warn("Destructed called with allocations undeleted");
        }

        Chunk* ch = m_head_chunk;
        while (ch)
        {
            Chunk* next = ch->next;
            destroyChunk(ch);
            ch = next;
        }
    }

    void* ChainMemoryPool::allocate(std::size_t size, std::size_t alignment_bits)
    {
        ERIS_ASSERT(size < m_max_size);

        Chunk* ch;
        void* out = nullptr;

        std::lock_guard<SpinLock> lock(m_lock);

        ch = m_tail_chunk;

        if (!ch || !(out = allocateFromChunk(ch, size, alignment_bits)))
        {
            std::size_t chunk_size = computeChunkSize(size);
            ch = createChunk(chunk_size);

            if (!ch)
                return out;
        }

        if (!out)
        {
            out = allocateFromChunk(ch, size, alignment_bits);
            ERIS_ASSERT(out);
        }

        m_allocations++;

        return out;
    }

    void ChainMemoryPool::free(void* ptr)
    {
        if (!ptr)
            return;

        std::lock_guard<SpinLock> lock(m_lock);

        Chunk* current = m_head_chunk;
        Chunk* prev = nullptr;
        const glm::u8* cptr = reinterpret_cast<const glm::u8*>(ptr);

        while (current)
        {
            const glm::u8* from = current->memory;
            const glm::u8* to = from + current->mem_size;
            if (cptr >= from && cptr <= to)
                break;

            prev = current;
            current = current->next;
        }

        ERIS_ASSERT(current);
        ERIS_ASSERT(current->allocation_count > 0);

        if (--current->allocation_count == 0)
        {
            if (prev)
            {
                ERIS_ASSERT(m_head_chunk != current);
                prev->next = current->next;
            }

            if (current == m_head_chunk)
            {
                ERIS_ASSERT(!prev);
                m_head_chunk = current->next;
            }

            if (current == m_tail_chunk)
                m_tail_chunk = prev;

            destroyChunk(current);
        }

        m_allocations--;
    }
   
    std::size_t ChainMemoryPool::getChunksCount() const
    {
        std::size_t count = 0;

        Chunk* ch = m_head_chunk;
        while (ch)
        {
            count++;
            ch = ch->next;
        }

        return count;
    }

    std::size_t ChainMemoryPool::getAllocatedSize() const
    {
        std::size_t size = 0;

        Chunk* ch = m_head_chunk;
        while (ch)
        {
            size += ch->head - ch->memory;
            ch = ch->next;
        }

        return size;
    }

    std::size_t ChainMemoryPool::computeChunkSize(std::size_t size)
    {
        std::size_t current_max = 0;
        if (m_method == ChunkGrowMethod::FIXED)
        {
            current_max = m_init_size;
        }
        else
        {
            if (m_tail_chunk)
            {
                current_max = m_tail_chunk->mem_size;
                ERIS_ASSERT(current_max > 0);

                if (m_method == ChunkGrowMethod::MULTIPLY)
                    current_max *= m_step;
                else
                    current_max += m_step;
            }
            else
                current_max = m_init_size;

            ERIS_ASSERT(current_max > 0);

            glm::min(current_max, m_max_size);
        }

        size = glm::max(current_max, size) + m_alignment_bits;

        return size;
    }

    ChainMemoryPool::Chunk* ChainMemoryPool::createChunk(std::size_t size)
    {
        ERIS_ASSERT(size > 0);

        Chunk* ch = reinterpret_cast<Chunk*>(_aligned_malloc(sizeof(Chunk), __alignof(Chunk)));

        if (ch)
        {
            construct(ch);
            ch->memory = reinterpret_cast<glm::u8*>(_aligned_malloc(size, m_alignment_bits));
            if (ch->memory)
            {
                ch->mem_size = size;
                ch->head = ch->memory;

                if (m_tail_chunk)
                {
                    m_tail_chunk->next = ch;
                    m_tail_chunk = ch;
                }
                else
                {
                    ERIS_ASSERT(!m_head_chunk);
                    m_head_chunk = m_tail_chunk = ch;
                }
            }
            else
            {
                Log::error("Not enough memory for allocation");
                destruct(ch);
                _aligned_free(ch);
                ch = nullptr;
            }
        }
        else
        {
            Log::error("Not enough memory for allocation");
        }

        return ch;
    }

    void* ChainMemoryPool::allocateFromChunk(Chunk* ch, std::size_t size, std::size_t alignment_bits)
    {
        ERIS_ASSERT(ch);
        ERIS_ASSERT(size <= m_max_size);
        ERIS_ASSERT(ch->head <= ch->memory + ch->mem_size);

        glm::u8* mem = ch->head;
        alignRoundUp(m_alignment_bits, mem);
        glm::u8* head = mem + size;

        if (head <= ch->memory + ch->mem_size)
        {
            ch->head = head;
            ch->allocation_count++;
        }
        else
            mem = nullptr;

        return mem;
    }

    void ChainMemoryPool::destroyChunk(ChainMemoryPool::Chunk* ch)
    {
        ERIS_ASSERT(ch);

        if (ch->memory)
            _aligned_free(ch->memory);

        _aligned_free(ch);
    }
}