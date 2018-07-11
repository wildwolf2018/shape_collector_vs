#include "..\\Header Files\\animation.h"

Animation::Animation(int numOfSpawnPositions)
{
	for (int i = 0; i < numOfSpawnPositions; ++i) {
		endTimes.insert({ i, 0.0f });
	}
}

void Animation::setEndTime(int index, float currentTime)
{
	constexpr float LIFE_TIME = 10.0f;
	endTimes[index] = currentTime + LIFE_TIME;
}

bool Animation::checkExpiration(int position, float currentTime)
{
	return (endTimes[position] < currentTime);
}
