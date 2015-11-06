#pragma once
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "GameContext.h"
#include "HexGrid.h"
#include <vector>
#include "PostProcessEffect.h"

struct ClickResult {

	int bombsRemoved;
	int collected;
	bool killed;
	bool finished;

	ClickResult() : bombsRemoved(0), collected(0) , killed(false) , finished(false) {}
};

class Board {

public:
	Board();
	~Board();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
	ClickResult onClick(int x, int y);
	void refill();
	void nextLevel(int level);
	void fadeOut();
	void flashBombs();
private:	
	int _maxBombs;
	int _killed;
	HexGrid _grid;
	int _selected;
	int _hover;
	int _width;
	int _height;
	FadeOutEffect* _fadeOutEffect;
	FadeOutEffectSettings _settings;
};

