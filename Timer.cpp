#include "Timer.h"



Timer::Timer()
{
	prev = cur = clock(); 
	t = dt = fps = 0; 
}


Timer::~Timer()
{
}

void Timer::update()
{
	prev = cur; 
	cur = clock(); 
	dt = (cur - prev) / (float)CLOCKS_PER_SEC; 
	t += dt; 
	fps = 1 / dt; 
}
