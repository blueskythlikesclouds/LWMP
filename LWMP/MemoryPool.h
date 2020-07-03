#pragma once

class MemoryArena
{
public:
    uint8_t* data;
    std::atomic<bool> used;

    MemoryArena(size_t arenaSize);
    ~MemoryArena();
};

class MemoryPool
{
    size_t arenaSize;
    std::vector<std::unique_ptr<MemoryArena>> arenas;

    std::mutex mutex;

public:
    MemoryPool(size_t arenaSize);

    std::shared_ptr<uint8_t[]> allocate();

    template <typename T>
    std::shared_ptr<T> allocate()
    {
        return std::reinterpret_pointer_cast<T>(allocate());
    }

    size_t getArenaSize() const;
};