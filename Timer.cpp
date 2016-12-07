#include "Engine.h"
#include <boost/timer.hpp>

namespace Batman
{

	/*
		When a timer object is created, the reset function is called, which reset the stopwatch.
		Then when the object.Stopwatch() function is called with a parameter of a specific time,
		the stopwatch function returns true if said amount of time has passed, and then resets itself.
		It can also be manually reset by calling object.reset() if needed.
	*/

	Timer::Timer(void)
	{
		Reset();
	}

	Timer::~Timer(void) {}

	double Timer::getElapsed()
	{
		double elapsed = p_timer.elapsed() * 1000;	//multiplied by 1000 to get milliseconds,
													//as the boost p_timer.elapsed() function returns seconds
		return elapsed;
	}

	void Timer::Rest(double ms)
	{
		//start at current elapsed value
		double start = getElapsed();

		//loop with sleep for as many ms as specified
		while(start + ms > getElapsed())
		{
			Sleep(1);
		}
	}

	void Timer::Reset()
	{
		//set stopwatch time to current elapsed time
		stopwatch_start = getElapsed();
	}

	bool Timer::StopWatch(double ms)
	{
		//checks to see if ms time has passed
		if(getElapsed() > stopwatch_start + (double)ms)
		{
			Reset();
			return true;
		}
		else
		{
			return false;
		}
	}
};