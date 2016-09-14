#pragma once
#include <renderer\render_types.h>
#include <imgui\IMGUI.h>
#include "GameContext.h"
#include "HexGrid.h"
#include <vector>

struct ClickResult {

	int bombsRemoved;
	int collected;
	bool killed;
	bool finished;

	ClickResult() : bombsRemoved(0), collected(0) , killed(false) , finished(false) {}
};

class Board {

	struct Selection {
		float timer;
		ds::Texture texture;
		int index;
	};

public:
	Board(GameContext* context);
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
	//int _selected;
	int _hover;
	int _width;
	int _height;
	GameContext* _context;
	Selection _selection;
	ds::Texture _pieceTexture;
};

