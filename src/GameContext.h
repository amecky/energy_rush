#pragma once
#include <utils\GameTimer.h>

struct Score {

	int points;
	int seconds;
	int minutes;
	int goals;
	int wrongGoals;

};

enum GameModeType {
	GM_EASY,
	GM_MEDIUM,
	GM_HARD
};

struct GameMode {
	GameModeType type;
	int maxBombs;
	int width;
	int height;
	v2 origin;

	GameMode() : type(GM_EASY), maxBombs(0), width(0), height(0) , origin(100,130) {}
	GameMode(GameModeType t, int m, int w, int h,const v2& o) : type(t), maxBombs(m), width(w), height(h) , origin(o) {}
};

const GameMode GAME_MODES[] = {
	GameMode(GM_EASY, 12, 8, 8, v2(300, 330)),
	GameMode(GM_MEDIUM, 32, 14, 12, v2(200, 230)),
	GameMode(GM_HARD, 60, 20, 15, v2(100,130)),
};

struct GameContext {

	Score score;
	ds::GameTimer timer;
	int mode;

	void resetScore() {
		score.points = 0;
		score.goals = 0;
		score.wrongGoals = 0;
		score.seconds = 0;
		score.minutes = 0;
	}
};

