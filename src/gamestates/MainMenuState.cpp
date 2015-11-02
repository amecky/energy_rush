#include "MainMenuState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

MainMenuState::MainMenuState(ds::DialogManager* gui, GameContext* context) : ds::GameState("MainMenu"), _context(context), _gui(gui) {
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainMenuState::activate() {
	_gui->activate("MainMenu");	
	/*
	ds::GUIDialog* dlg = _gui->get("GameOver");
	std::string str;
	ds::string::formatInt(_context->score.goals, str);
	dlg->updateText(12, str);
	ds::string::formatInt(_context->score.wrongGoals, str);
	dlg->updateText(8, str);
	ds::string::formatTime(_context->timer.getMinutes(), _context->timer.getSeconds(), str);
	int seconds = _context->timer.getSeconds() + _context->timer.getMinutes() * 60;
	int diff = seconds - 100;
	dlg->updateText(14, str);
	_context->score.points = _context->score.goals * 1000 + diff * 500;
	ds::string::formatInt(_context->score.points, str);
	dlg->updateText(16, str);
	*/
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainMenuState::deactivate() {
	_gui->deactivate("MainMenu");
}

// --------------------------------------------
// update
// --------------------------------------------
int MainMenuState::update(float dt) {
	// nothing to do
	return 0;
}

// --------------------------------------------
// click
// --------------------------------------------
int MainMenuState::onGUIButton(ds::DialogID dlgID, int button) {
	return button;
}

// --------------------------------------------
// render
// --------------------------------------------
void MainMenuState::render() {
}


