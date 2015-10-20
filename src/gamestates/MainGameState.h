#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\GameSettings.h"
#include "..\GameContext.h"


class MainGameState : public ds::GameState {

public:
	MainGameState(GameSettings* settings,GameContext* context);
	virtual ~MainGameState();
	void init();
	int update(float dt);
	void render();
	void activate();
	int onChar(int ascii);
private:
	GameSettings* _settings;
	GameContext* _context;
};

