#include "MultiCore.h"
#include <process.h>

MultiCore::Core::Core() : stop(false)
{
}

void MultiCore::Core::Routine()
{
	while(WaitForSingleObject(doWorkEvent, INFINITE) == WAIT_OBJECT_0)
	{
		if(stop)
			break;
		work(workData);
		SetEvent(workDoneEvent);
	}
}

MultiCore::MultiCore()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	//создать потоки
	cores.resize(systemInfo.dwNumberOfProcessors);
	for(int i = 0; i < (int)cores.size(); ++i)
	{
		cores[i].doWorkEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		cores[i].workDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		cores[i].thread = (HANDLE)_beginthread(ThreadRoutine, 0, &cores[i]);
		if(cores[i].thread == INVALID_HANDLE_VALUE)
			throw std::wstring(L"Can't create thread");
		if(!SetThreadAffinityMask(cores[i].thread, 1 << i))
			throw std::wstring(L"Can't set affinity mask");
	}
	workDoneEvents.resize(cores.size());
	for(int i = 0; i < (int)cores.size(); ++i)
		workDoneEvents[i] = cores[i].workDoneEvent;
}

MultiCore::~MultiCore()
{
	for(int i = 0; i < (int)cores.size(); ++i)
	{
		cores[i].stop = true;
		SetEvent(cores[i].doWorkEvent);
	}

	std::vector<HANDLE> e(cores.size());
	for(int i = 0; i < (int)cores.size(); ++i)
		e[i] = cores[i].thread;

	WaitForMultipleObjects(cores.size(), &*e.begin(), TRUE, INFINITE);
}

void MultiCore::ThreadRoutine(void *param)
{
	((Core*)param)->Routine();
}

int MultiCore::GetCoreCount() const
{
	return cores.size();
}

void MultiCore::SetWork(int core, void (*work)(void*), void* workData)
{
	cores[core].work = work;
	cores[core].workData = workData;
}

void MultiCore::DoWork()
{
	for(int i = 0; i < (int)cores.size(); ++i)
		SetEvent(cores[i].doWorkEvent);
	WaitForMultipleObjects(cores.size(), &*workDoneEvents.begin(), TRUE, INFINITE);
}
