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
		else
		{
			system("pause");
		}
		e->Shutdown();
		Memory::GetInstance()->Deallocate<Engine>(e);
		
		Memory::GetInstance()->Shutdown();
		Memory::FreeInstance();
	}

#if defined(_DEBUG) || defined(DEBUG)
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
