#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\GameSettings.h"
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>

struct GridItem {

	Hex hex;
	v2 position;
};

class MainGameState : public ds::GameState {

typedef std::vector<GridItem> Items;

public:
	MainGameState(GameSettings* settings,GameContext* context);
	virtual ~MainGameState();
	void init();
	int update(float dt);
	void render();
	void activate();
	int onChar(int ascii);
	int onButtonUp(int button, int x, int y);
private:
	GameSettings* _settings;
	GameContext* _context;
	Layout _layout;
	Items _items;
	int _selected;
};

