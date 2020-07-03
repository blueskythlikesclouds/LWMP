#include "MemoryPool.h"

MemoryArena::MemoryArena(const size_t arenaSize) : data((uint8_t*)operator new(arenaSize)), used(false)
{
}

MemoryArena::~MemoryArena()
{
    operator delete(data);
}

MemoryPool::MemoryPool(const size_t arenaSize) : arenaSize(arenaSize)
{
}

std::shared_ptr<uint8_t[]> MemoryPool::allocate()
{
    std::unique_lock<std::mutex> lock(mutex);

    MemoryArena* arena = nullptr;

    for (auto& item : arenas)
    {
        if (item->used)
            continue;

        arena = item.get();
        break;
    }

    if (!arena)
    {
        arena = new MemoryArena(arenaSize);
        arenas.push_back(std::unique_ptr<MemoryArena>(arena));
    }

    memset(arena->data, 0, arenaSize);

    arena->used = true;
    return std::shared_ptr<uint8_t[]>(arena->data, [arena](uint8_t*)
    {
        arena->used = false;
    });
}

size_t MemoryPool::getArenaSize() const
{
    return arenaSize;
}
