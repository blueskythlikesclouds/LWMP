#pragma once

#include <atomic>
#include <mutex>
#include <vector>

struct MemoryArena
{
    uint8_t* data;
    std::atomic<bool> used;

    MemoryArena(size_t arenaSize);
    ~MemoryArena();
};

class MemoryPool
{
    size_t arenaSize;
    std::vector<MemoryArena*> arenas;

    std::mutex mutex;

public:
    MemoryPool(size_t arenaSize);
    ~MemoryPool();

    std::shared_ptr<uint8_t[]> allocate();

    template <typename T>
    std::shared_ptr<T> allocate()
    {
        return std::reinterpret_pointer_cast<T>(allocate());
    }

    size_t getArenaSize() const;
};
