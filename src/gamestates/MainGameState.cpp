#include "MainGameState.h"
#include <renderer\sprites.h>
#include "..\Constants.h"
#include <Vector.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_maxBombs = 2;
	_width = 20;
	_height = 15;
	_board = new Board(context);
	_hud = ds::res::getGUIDialog("HUD");
	_gameOver = ds::res::getGUIDialog("GameOver");
	_mode = GM_RUNNING;
}


MainGameState::~MainGameState() {
	delete _board;
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MainGameState::init() {
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void MainGameState::fillBombs() {
	_board->refill();
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	
	_hud->activate();

	_maxBombs = 2;
	_context->collected = 0;
	_context->points = 0;
	_context->level = 0;
	_context->kills = 0;
	_board->refill();
	_hud->setNumber(HUD_COLLECTED, 0);
	_hud->setNumber(HUD_BOMBS, 0);
	_hud->setNumber(HUD_LEVEL, 0);

	_gameOver->deactivate();
	_mode = GM_RUNNING;
	nextLevel();
	
}

// -------------------------------------------------------
// next level
// -------------------------------------------------------
void MainGameState::nextLevel() {
	++_context->level;
	_hover = -1;
	_killed = 0;
	//_context->kills = 0;
	_maxBombs = _context->level * 2;
	_board->nextLevel(_context->level);
	_hud->setNumber(HUD_LEVEL, _context->level);
	_hud->setNumber(HUD_BOMBS, _maxBombs);
	
	//_grid.decrementBombs();
}
// -------------------------------------------------------
// dactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	_hud->deactivate();
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	if (_mode == GM_OVER) {
		// FIXME: include some delay if someone has clicked accidentially
		int ret = _gameOver->onButton(button, x, y, true);
		if (ret != -1) {
			return ret;
		}
	}
	else {
		ClickResult result = _board->onClick(x, y);
		if (result.killed) {
			return 1;
		}
		_context->collected += result.collected;
		_hud->setNumber(HUD_COLLECTED, _context->collected);
		_killed += result.bombsRemoved;
		_context->kills += result.bombsRemoved;
		int d = _maxBombs - _killed;
		_hud->setNumber(HUD_BOMBS, d);
		if (result.finished) {
			nextLevel();
		}
	}
	return 0;
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
int MainGameState::update(float dt) {
	if (_mode == GM_RUNNING) {
		_board->update(dt);
		_hud->tick(dt);
	}
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	//ds::sprites::draw(v2(512, 384), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f), 0.0f, 2.0f, 2.0f);
	_board->render();
	_hud->render();
	_gameOver->render();
}

void MainGameState::stopGame() {
	_gameOver->activate();
	_hud->deactivate();
	_mode = GM_OVER;
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		stopGame();
	}
	if (ascii == 'r') {
		_context->pick_colors();
	}
	if (ascii == 'f') {
		_board->flashBombs();
	}
	return 0;
}

