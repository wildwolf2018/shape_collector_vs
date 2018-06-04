#include "time_clock.h"

Clock::Clock():timer{0.0f}, stopTime{true}{}

void Clock::startClock()
{
	stopTime = false;
	start_ticks = std::chrono::steady_clock::now();
}

void Clock::addTime()
{
	if(!stopTime)
	{
		end_ticks = std::chrono::steady_clock::now();
		timeDuration = end_ticks - start_ticks;
		timer += timeDuration.count();
		start_ticks = end_ticks;
	}
}

float Clock::getElapsedTime() const
{
	return timer;
}

void Clock::stopClock()
{
	stopTime = true;
	timer = 0.0f;
}