#pragma once
#include <dialogs\GUIDialog.h>

struct GameContext {

	int collected;
	int points;
	int level;
	int kills;
	ds::GUIDialog* hudDialog;

	void reset() {
		collected = 0;
		points = 0;
		level = 1;
		kills = 0;
	}
};

