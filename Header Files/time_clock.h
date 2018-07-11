#ifndef TIME_CLOCK_H
#define TIME_CLOCK_H

#include <chrono>

class Clock
{
public:
	Clock();
	void startClock();
	void addTime();
	void stopClock();
	float getElapsedTime() const;
	bool stopTime;
	float timer;
private:
	std::chrono::time_point<std::chrono::steady_clock> start_ticks;
	std::chrono::time_point<std::chrono::steady_clock> end_ticks;
	std::chrono::duration<double> timeDuration;
};
#endif