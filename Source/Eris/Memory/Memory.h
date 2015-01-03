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
#include "Thread/SpinLock.h"
#include "Util/NonCopyable.h"

#include <atomic>

namespace Eris
{
    class BaseMemoryPool : public RefCounted, public NonCopyable
    {
    public:
        enum class Type : glm::u8
        {
            NONE,
            HEAP,
            STACK,
            CHAIN
        };

        BaseMemoryPool(Type type);
        virtual ~BaseMemoryPool();

        void* allocate(std::size_t size, std::size_t alignment_bits);
        void free(void* ptr);

        glm::u32 getAllocations() const { return m_allocations.load(); }

    protected:
        std::atomic<glm::u32> m_allocations;

    private:
        Type m_type;
    };

    class HeapMemoryPool : public BaseMemoryPool
    {
    public:
        HeapMemoryPool();
        virtual ~HeapMemoryPool() final;

        void* allocate(std::size_t size, std::size_t alignment_bits);
        void free(void* ptr);
    };

    class StackMemoryPool : public BaseMemoryPool
    {
        using Snapshot = void*;

    public:
        StackMemoryPool(std::size_t size, bool deallocation_flag = false, std::size_t alignment_bits = 16);
        virtual ~StackMemoryPool() final;

        void* allocate(std::size_t size, std::size_t alignment_bits);
        void free(void* ptr);

        std::size_t getTotalSize() const { return m_mem_size; }
        std::size_t getAllocatedSize() const { return m_head.load() - m_memory; }

        Snapshot takeSnapshot() const;

        void resetUsingSnapshot(Snapshot s);
        void reset();

    private:
        struct MemoryBlockHeader
        {
            glm::u8 size[sizeof(std::size_t)];
        };

        bool m_deallocation_flag;
        std::size_t m_alignment_bits;
        std::size_t m_header_size;
        std::size_t m_mem_size;
        std::atomic<glm::u8*> m_head;
        glm::u8* m_memory;
    };

    class ChainMemoryPool : public BaseMemoryPool
    {
    public:
        enum class ChunkGrowMethod : glm::u8
        {
            NONE,
            FIXED,
            MULTIPLY,
            ADD
        };

        ChainMemoryPool(std::size_t initial_chunk_size, std::size_t max_chunk_size, ChunkGrowMethod chunk_grow_method = ChunkGrowMethod::MULTIPLY, std::size_t chunk_alloc_step = 2, std::size_t alignment_bits = 16);
        virtual ~ChainMemoryPool() final;

        void* allocate(std::size_t size, std::size_t alignment_bits);
        void free(void* ptr);

        std::size_t getChunksCount() const;
        std::size_t getAllocatedSize() const;

    private:
        struct Chunk
        {
            glm::u8* memory = nullptr;
            std::size_t mem_size = 0;
            glm::u8* head = nullptr;
            std::size_t allocation_count = 0;
            Chunk* next = nullptr;
        };

        std::size_t computeChunkSize(std::size_t size);
        Chunk* createChunk(std::size_t size);
        void* allocateFromChunk(Chunk* ch, std::size_t size, std::size_t alignment_bits);
        void destroyChunk(Chunk* chunk);

        std::size_t m_alignment_bits;
        std::size_t m_init_size;
        std::size_t m_max_size;
        std::size_t m_step;
        ChunkGrowMethod m_method;
        SpinLock m_lock;
        Chunk* m_head_chunk;
        Chunk* m_tail_chunk;
    };
}