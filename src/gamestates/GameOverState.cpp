#include "GameOverState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

GameOverState::GameOverState(ds::DialogManager* gui, GameContext* context) : ds::GameState("GameOver"), _context(context), _gui(gui) {
}


GameOverState::~GameOverState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::activate() {
	_gui->activate("GameOver");	
	
	ds::GUIDialog* dlg = _gui->get("GameOver");
	dlg->setNumber(6, _context->collected);
	dlg->setNumber(8, _context->level);
	dlg->setNumber(9, _context->kills);
	_context->points = _context->collected * 10 + (_context->level - 1) * 1000 + _context->kills * 100;
	dlg->setNumber(14, _context->points);
	_context->board->fadeOut();
}

// --------------------------------------------
// activate
// --------------------------------------------
void GameOverState::deactivate() {
	_gui->deactivate("GameOver");
}

// --------------------------------------------
// update
// --------------------------------------------
int GameOverState::update(float dt) {
	
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int GameOverState::onGUIButton(ds::DialogID dlgID, int button) {	
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void GameOverState::render() {
	ds::sprites::draw(v2(512, 384), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f), 0.0f, 2.0f, 2.0f);
}


