#ifndef ___MULTI_CORE_H___
#define ___MULTI_CORE_H___

/*
���� �������� ���������� ������, ������������ �������� ������� �����������.
*/

#include "windows.h"
#include <vector>

class MultiCore
{
private:
	struct Core
	{
		HANDLE doWorkEvent;
		HANDLE workDoneEvent;
		HANDLE thread;
		bool stop;
		void (*work)(void*);
		void* workData;

		Core();
		void Routine();
	};

	std::vector<Core> cores;
	std::vector<HANDLE> workDoneEvents;

	static void ThreadRoutine(void* param);

public:
	MultiCore();
	~MultiCore();

	int GetCoreCount() const;
	void SetWork(int core, void (*work)(void*), void* workData);
	void DoWork();
};

#endif
