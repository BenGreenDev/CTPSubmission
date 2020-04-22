#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	//Will mark a point to count from
	float Mark();
	//Will return the time elapsed from the last mark
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};

