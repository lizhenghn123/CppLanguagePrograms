/**
* This file is part of Easylogging++ samples
* Demonstration of VC++ thread
* Base code taken from: http://msdn.microsoft.com/en-us/library/12a04hfd(v=vs.80).aspx
* git repo : https://github.com/easylogging/easyloggingpp
* Revision: 1.1
* @author mkhan3189
*/

#define _ELPP_STL_LOGGING
//#define _ELPP_THREAD_SAFE
#include "easylogging++/easylogging++.h"

//must do this
_INITIALIZE_EASYLOGGINGPP   

using namespace std;

volatile bool Sentinel = true;
int CriticalData = 0;

unsigned ThreadFunc1(void* pArguments) {
	while (Sentinel)
		Sleep(0);   // volatile spin lock
	// CriticalData load guaranteed after every load of Sentinel
	LOG(INFO) << "ThreadFunc1 log - Critical Data = " << CriticalData;

	return 0;
}

unsigned  ThreadFunc2(void* pArguments) {
	Sleep(2000);
	CriticalData++;   // guaranteed to occur before write to Sentinel
	Sentinel = false; // exit critical section
	LOG(DEBUG) << "test";
	return 0;
}

int main() {
	HANDLE hThread1, hThread2;
	DWORD retCode;

	hThread1 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&ThreadFunc1, nullptr, 0, nullptr);
	hThread2 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&ThreadFunc2, nullptr, 0, nullptr);

	if (hThread1 == nullptr || hThread2 == nullptr)       {
		LOG(ERROR) << "CreateThread failed - Could not create thread";
		return 1;
	}

	retCode = WaitForSingleObject(hThread1, 3000);

	CloseHandle(hThread1);
	CloseHandle(hThread2);

	if (retCode == WAIT_OBJECT_0 && CriticalData == 1)
		LOG(INFO) << "Succeeded";
	else
		LOG(ERROR) << "Failed";

	system("pause");
}
/*****
08/10/2014 23:19:23,701 INFO  [default] ThreadFunc1 log - Critical Data = 1
08/10/2014 23:19:23,706 DEBUG [default] [lizheng@LIZHENG-PC] [unsigned int __cdecl ThreadFunc2(void *)] [x:\visualstudio2013\easylogging++\easylogging++\main.cpp:38] test
08/10/2014 23:19:23,714 INFO  [default] Succeeded

*****/