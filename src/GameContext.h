#pragma once
#include <dialogs\GUIDialog.h>
#include <core\data\DynamicSettings.h>
#include "Constants.h"
#include "GameSettings.h"
#include <core\data\HighscoreService.h>

struct Highscore {

	int collected;
	int level;
	int points;
	char name[16];

	Highscore() : collected(0) , level(1) , points(0) {}

	Highscore(int c, int l, int p) : collected(c), level(l), points(p) {}

	int compare(const Highscore& other) const {
		if (points < other.points) {
			return -1;
		}
		if (points > other.points ) {
			return 1;
		}
		return 0;
	}

};

struct GameContext {

	int collected;
	int points;
	int level;
	int kills;
	GameSettings settings;
	ds::Color colors[8];
	ds::HighscoreService<Highscore, 10> highscores;

	void reset() {
		collected = 0;
		points = 0;
		level = 1;
		kills = 0;
	}

	void pick_colors() {
		// prepare colors
		float goldenRatioConjugate = 0.618033988749895f;
		float currentHue = math::random(0.0f, 1.0f);
		for (int i = 0; i < 8; i++) {
			ds::HSL hslColor = ds::HSL(currentHue * 360.0f, 50.0f, 50.0f);
			colors[i] = ds::color::hsl2rgb(hslColor);
			currentHue += goldenRatioConjugate;
			if (currentHue > 1.0f) {
				currentHue -= 1.0f;
			}
		}
	}
};

