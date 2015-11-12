#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"
#include <Vector.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_maxBombs = 2;
	_width = 20;
	_height = 15;
}


MainGameState::~MainGameState() {
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
	_context->board->refill();
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	
	_context->hudDialog->activate();

	_maxBombs = 2;
	_context->collected = 0;
	_context->points = 0;
	_context->level = 0;
	_context->kills = 0;

	_context->hudDialog->setNumber(HUD_COLLECTED, 0);
	_context->hudDialog->setNumber(HUD_BOMBS, 0);
	_context->hudDialog->setNumber(HUD_LEVEL, 0);

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
	_context->board->nextLevel(_context->level);
	_context->hudDialog->setNumber(HUD_LEVEL, _context->level);
	_context->hudDialog->setNumber(HUD_BOMBS, _maxBombs);
	
	//_grid.decrementBombs();
}
// -------------------------------------------------------
// dactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	_context->hudDialog->deactivate();
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	ClickResult result = _context->board->onClick(x, y);
	if (result.killed) {
		return 1;
	}
	_context->collected += result.collected;
	_context->hudDialog->setNumber(HUD_COLLECTED, _context->collected);
	_killed += result.bombsRemoved;
	_context->kills += result.bombsRemoved;
	int d = _maxBombs - _killed;
	_context->hudDialog->setNumber(HUD_BOMBS, d);	
	if (result.finished) {
		nextLevel();
	}	
	return 0;
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
int MainGameState::update(float dt) {

	_context->board->update(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f), 0.0f, 2.0f, 2.0f);
	_context->board->render();
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		return 1;
	}
	if (ascii == 'r') {
		_context->board->fadeOut();
	}
	if (ascii == 'f') {
		_context->board->flashBombs();
	}
	return 0;
}

