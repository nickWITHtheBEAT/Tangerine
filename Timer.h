#pragma once
#include "Utils.h"


namespace Timer
{
	struct TimerStruct
	{
		__int64 t0, t1;
		float dt, SecondsPerCount;
	};
	static void Setup(TimerStruct* Ts)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&(Ts->t0));
		__int64 cps = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cps);
		Ts->SecondsPerCount = 1.0f / cps;
		
	}
	static void GetDt(TimerStruct* Ts)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&(Ts->t1));
		Ts->dt = ((Ts->t1 - Ts->t0) * Ts->SecondsPerCount);
	}
	static void Reset(TimerStruct* Ts)
	{
		Ts->t0 = Ts->t1;
	}
}
