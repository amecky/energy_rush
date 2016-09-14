#pragma once
#include <dialogs\GUIDialog.h>
#include <core\data\DynamicSettings.h>
#include "Constants.h"
#include "GameSettings.h"

struct GameContext {

	int collected;
	int points;
	int level;
	int kills;
	//ds::GUIDialog* hudDialog;
	GameSettings settings;
	ds::Color colors[8];

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

