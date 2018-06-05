#ifndef ANIMATION_H
#define ANIMATION_H
#include <map>

class Animation {
public:
	Animation::Animation(int numOfSpawnPositions);
	std::map<int, float> endTimes;
	void setEndTime(int index, float currentTime);
	bool checkExpiration(int position, float currentTime);
};
#endif