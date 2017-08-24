#pragma once

#include <string> 
#include <vector>
#include <new>

#define NewObject(T, ...) Memory::GetInstance()->Allocate<T>(sizeof(T), #T, __FILE__, __LINE__, __VA_ARGS__)

class Memory
{
protected:
	struct MemoryObject
	{
		std::string _typeName;
		std::string _allocationPlace;
		void* _memory;
		size_t _size;
	};

protected:
	static Memory* _instance;

	std::vector<MemoryObject*> _allocatedObjects;

public:
	inline static Memory* GetInstance()
	{
		if(_instance == nullptr)
		{
			_instance = new Memory();
		}
		return _instance;
	}

	inline static void FreeInstance()
	{
		if(_instance)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	template<typename T, typename... Args>
	T* Allocate(size_t size, const char* typeName, const char* filename, int line, Args... args)
	{
		MemoryObject* mo = new MemoryObject();
		mo->_allocationPlace = std::string(filename) + " " + std::to_string(line);
		mo->_typeName = typeName != nullptr ? typeName : "";
		mo->_memory = _aligned_malloc(size, 16);
		mo->_size = size;
		_allocatedObjects.push_back(mo);
		::new(mo->_memory) T(args...);
		return (T*)mo->_memory;
	}

	void* Allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		MemoryObject* mo = new MemoryObject();
		mo->_allocationPlace = std::string(filename) + " " + std::to_string(line);
		mo->_typeName = typeName != nullptr ? typeName : "";
		mo->_memory = _aligned_malloc(size, 16);
		mo->_size = size;
		_allocatedObjects.push_back(mo);
		return mo->_memory;
	}

	template<typename T>
	void Deallocate(T* ptr)
	{
		auto& it = _allocatedObjects.begin();
		auto& end = _allocatedObjects.end();
		int size = 0;
		for(; it != end; ++it)
		{
			MemoryObject* mo = (*it);
			if(mo->_memory == ptr)
			{
				delete mo;
				break;
			}
			++size;
		}
		if(size < _allocatedObjects.size())
		{
			_allocatedObjects.erase(_allocatedObjects.begin() + size);
		}
		ptr->~T();
		_aligned_free(ptr);
	}

	void Deallocate(void* ptr)
	{
		auto& it = _allocatedObjects.begin();
		auto& end = _allocatedObjects.end();
		int size = 0;
		for(; it != end; ++it)
		{
			MemoryObject* mo = (*it);
			if(mo->_memory == ptr)
			{
				delete mo;
				break;
			}
			++size;
		}
		if(size < _allocatedObjects.size())
		{
			_allocatedObjects.erase(_allocatedObjects.begin() + size);
		}
		_aligned_free(ptr);
	}

	void Shutdown();
	void PrintActualStatus() const;
	void PrintStatistics() const;
};

