#include "Memory.h"

Memory* Memory::_instance = nullptr;

void Memory::Shutdown()
{
#if defined(_DEBUG) || defined(DEBUG)
	if(_allocatedObjects.size() <= 0)
	{
		return;
	}
	PrintActualStatus();
	system("pause");
#endif
}

void Memory::PrintActualStatus() const
{
	printf("----------------------------------------------------\nACTUAL MEMORY STATUS\n\n");
	auto& it = _allocatedObjects.begin();
	auto& end = _allocatedObjects.end();
	for(; it != end; ++it)
	{
		MemoryObject* mo = (*it);
		if(mo)
		{
			printf("MemoryObject at %p, size: %zi \t allocation place: %s, type: %s\n", mo->_memory, mo->_size, mo->_allocationPlace.c_str(), mo->_typeName.c_str());
		}
	}
	printf("\n----------------------------------------------------\n\n");
}

void Memory::PrintStatistics() const
{
	size_t allocatedObjectsCount = _allocatedObjects.size(); 
	size_t wholeAllocatedSize = 0;
	auto& it = _allocatedObjects.begin();
	auto& end = _allocatedObjects.end();
	for(; it != end; ++it)
	{
		MemoryObject* mo = (*it);
		if(mo)
		{
			wholeAllocatedSize += mo->_size;
		}
	}

	printf("Memory contains %zi objects. Memory used: %zi bytes\n", allocatedObjectsCount, wholeAllocatedSize);
}

unsigned int Memory::GetObjectsCount() const
{
	return (unsigned int)_allocatedObjects.size();
}

unsigned int Memory::GetMemoryAllocated() const
{
	size_t allocatedObjectsCount = _allocatedObjects.size();
	size_t wholeAllocatedSize = 0;
	auto& it = _allocatedObjects.begin();
	auto& end = _allocatedObjects.end();
	for(; it != end; ++it)
	{
		MemoryObject* mo = (*it);
		if(mo)
		{
			wholeAllocatedSize += mo->_size;
		}
	}

	return (unsigned int)wholeAllocatedSize;
}
