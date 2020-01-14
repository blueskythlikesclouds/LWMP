#include "MemoryPool.h"

MemoryArena::MemoryArena(size_t arenaSize) : data((uint8_t*)_aligned_malloc(arenaSize, 16)), used(false)
{
}

MemoryArena::~MemoryArena()
{
	_aligned_free(data);
}

MemoryPool::MemoryPool(size_t arenaSize) : arenaSize(arenaSize)
{
}

MemoryPool::~MemoryPool()
{
	for (auto& item : arenas)
		delete item;
}

std::shared_ptr<void> MemoryPool::rent()
{
	std::unique_lock<std::mutex> lock(mutex);

	MemoryArena* arena = NULL;
	
	for (auto& item : arenas)
	{
		if (item->used)
			continue;
		
		arena = item;
		break;
	}

	if (!arena)
	{
		arena = new MemoryArena(arenaSize);
		arenas.push_back(arena);
	}
	
	memset(arena->data, 0, arenaSize);

	arena->used = true;
	return std::shared_ptr<void>(arena->data, [this, arena](void*)
	{
		std::unique_lock<std::mutex> lock(mutex);
		arena->used = false;
	});
}

size_t MemoryPool::getArenaSize() const
{
	return arenaSize;
}
