#include "Memory.h"

Memory* Memory::_instance = nullptr;

void Memory::Shutdown()
{
	if(_allocatedObjects.size() <= 0)
	{
		return;
	}
	printf("----------------------------------------------------\nTHERE ARE DEALLOCATED OBJECTS\n\n");
	auto& it = _allocatedObjects.begin();
	for(; it != _allocatedObjects.end(); ++it)
	{
		MemoryObject* mo = (*it);
		if(mo)
		{
			printf("MemoryObject at %p, size: %zi \t allocation place: %s, type: %s\n", mo->_memory, mo->_size, mo->_allocationPlace.c_str(), mo->_typeName.c_str());
		}
	}
	printf("\n----------------------------------------------------\n\n");
	system("pause");
}

void Memory::PrintActualStatus() const
{
	printf("----------------------------------------------------\nACTUAL MEMORY STATUS\n\n");
	auto& it = _allocatedObjects.begin();
	for(; it != _allocatedObjects.end(); ++it)
	{
		MemoryObject* mo = (*it);
		if(mo)
		{
			printf("MemoryObject at %p, size: %zi \t allocation place: %s, type: %s\n", mo->_memory, mo->_size, mo->_allocationPlace.c_str(), mo->_typeName.c_str());
		}
	}
	printf("\n----------------------------------------------------\n\n");
}
