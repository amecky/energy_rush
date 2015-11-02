#pragma once
#include <utils\GameTimer.h>

struct GameMode {
	int maxBombs;
	int width;
	int height;
	v2 origin;

	GameMode() : maxBombs(0), width(0), height(0) , origin(100,130) {}
	GameMode(int m, int w, int h,const v2& o) : maxBombs(m), width(w), height(h) , origin(o) {}
};

struct GameContext {

	int marked;
	int markedCorrectly;
	int playedSeconds;
	int playedMinutes;

	void reset() {
		marked = 0;
		markedCorrectly = 0;
	}
};

