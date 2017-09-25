#pragma once
#include <ctime>

class Timer
{
public:
	Timer();
	~Timer();
	float t; 
	float dt; 
	float fps;
	void update(); 
private:
	clock_t cur, prev;

};

