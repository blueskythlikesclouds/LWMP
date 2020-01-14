#pragma once
#include <mutex>
#include <vector>

struct MemoryArena
{
	uint8_t* data;
	bool used;

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
	
	std::shared_ptr<void> rent();

	template<typename T>
	std::shared_ptr<T> rent()
	{
		return std::reinterpret_pointer_cast<T>(rent());
	}
	
	size_t getArenaSize() const;
};
