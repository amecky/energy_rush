#pragma once
#include <dialogs\GUIDialog.h>
#include "Board.h"
#include <data\DynamicSettings.h>
#include "Constants.h"

class Board;

struct GameContext {

	int collected;
	int points;
	int level;
	int kills;
	ds::GUIDialog* hudDialog;
	Board* board;
	GameSettings gameSettings;

	void reset() {
		collected = 0;
		points = 0;
		level = 1;
		kills = 0;
	}
};

