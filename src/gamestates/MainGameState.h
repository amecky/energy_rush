#pragma once
#include <gamestates\GameState.h>
#include <renderer\render_types.h>
#include <imgui\IMGUI.h>
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>
#include "..\Board.h"

class MainGameState : public ds::GameState {

	enum GameMode {
		GM_RUNNING,
		GM_OVER
	};

public:
	MainGameState(GameContext* context);
	virtual ~MainGameState();
	void init();
	int update(float dt);
	void render();
	void activate();
	void deactivate();
	int onChar(int ascii);
	int onButtonUp(int button, int x, int y);
private:
	void fillBombs();
	void nextLevel();
	void stopGame();
	int _maxBombs;
	int _killed;
	GameContext* _context;
	ds::GUIDialog* _hud;
	ds::GUIDialog* _gameOver;
	int _hover;
	int _width;
	int _height;
	Board* _board;
	GameMode _mode;
};

