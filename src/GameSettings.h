#pragma once
#include <vector>
#include <imgui\IMGUI.h>
#include <core\data\DynamicSettings.h>

// -------------------------------------------------------
// game settings
// -------------------------------------------------------
struct GameSettings : public ds::DynamicGameSettings {

	// laser
	struct Bombs {
		float startDelay;
		float endDelay;
		float frequency;
		float amplitude;
		float angle;
		float minScale;
		float scaleVariance;
		float scaleFrequency;
		float shakeTTL;
	} bombs;

	

	GameSettings() : ds::DynamicGameSettings("content\\settings.json") {
		// bombs
		add("bombs.start_delay", &bombs.startDelay, 2.0f);
		add("bombs.end_delay", &bombs.endDelay, 2.0f);
		add("bombs.frequency", &bombs.frequency, 2.0f);
		add("bombs.amplitude", &bombs.amplitude, 2.0f);
		add("bombs.angle", &bombs.angle, 2.0f);
		add("bombs.min_scale", &bombs.minScale, 2.0f);
		add("bombs.scale_variance", &bombs.scaleVariance, 2.0f);
		add("bombs.scale_frequency", &bombs.scaleFrequency, 2.0f);
		add("bombs.shake_ttl", &bombs.shakeTTL, 2.0f);
	}
};
