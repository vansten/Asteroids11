#include "Core/Engine.h"
#include "Core/Memory.h"

int main()
{
	{
		Engine* e = Engine::GetInstance();
		if(e->Initialize())
		{
			e->Run();
		}
		e->Shutdown();
		Memory::GetInstance()->Deallocate<Engine>(e);
		
		Memory::GetInstance()->Shutdown();
		Memory::FreeInstance();
	}

	printf("\n\n\n");
	system("pause");

	_CrtDumpMemoryLeaks();

	return 0;
}
