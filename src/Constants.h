#pragma once
#include <utils\Color.h>
#include <renderer\render_types.h>
#include <data\DynamicSettings.h>

const int HUD_COLLECTED = 3;
const int HUD_BOMBS = 4;
const int HUD_LEVEL = 5;

struct GameSettings : public ds::DynamicGameSettings {

	float grayScaleFactor;

	GameSettings() {
		addFloat("grayScaleFactor", &grayScaleFactor, 2.0f);
	}
};