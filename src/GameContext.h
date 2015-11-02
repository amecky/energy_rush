#pragma once
#include <utils\GameTimer.h>

struct GameContext {

	int collected;
	int points;
	int level;
	int kills;

	void reset() {
		collected = 0;
		points = 0;
		level = 1;
		kills = 0;
	}
};

