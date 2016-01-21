#include "EnergyRush.h"
#include "utils\Log.h"
#include "Constants.h"
#include <sprites\SpriteBatch.h>
#include <base\GameStateMachine.h>
#include "gamestates\MainGameState.h"
#include "gamestates\GameOverState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\HighscoreState.h"

ds::BaseApp *app = new EnergyRush(); 

EnergyRush::EnergyRush() : ds::BaseApp() {
	_context = new GameContext;
}

EnergyRush::~EnergyRush() {
	delete _context->board;
	delete _context;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool EnergyRush::loadContent() {	
	_context->hudDialog = gui->get("HUD");
	_context->board = new Board;
	addGameState(new MainGameState(_context));
	addGameState(new GameOverState(gui, _context));
	addGameState(new HighscoreState(gui, _context));
	addGameState(new MainMenuState(gui, _context));
	connectGameStates("GameOver", 1, "MainGame");
	connectGameStates("GameOver", 2, "MainMenuState");
	connectGameStates("MainGame", 1, "GameOver");
	connectGameStates("MainMenuState", 3, "MainGame");
	return true;
}

void EnergyRush::init() {
	// for testing
	_context->reset();
	activate("MainMenuState");
}


// -------------------------------------------------------
// Update
// -------------------------------------------------------
void EnergyRush::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void EnergyRush::draw() {
	
}

void EnergyRush::onGUIButton(ds::DialogID dlgID, int button) {
	LOG << "dialog: " << dlgID << " button:" << button;
	if (dlgID == 4 && button == 1) {
		shutdown();
	}
}
