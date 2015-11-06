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
	_settings.screenWidth = 1024;
	_settings.screenHeight = 768;
	_settings.clearColor = ds::Color(0,0,0,255);	
	//_settings.showEditor = true;
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
	int texture = ds::renderer::loadTexture("TextureArray");
	assert(texture != -1);
	ds::sprites::initializeTextSystem(texture, "xscale");
	gui::initialize();
	initializeGUI();
	_context->hudDialog = gui.get("HUD");
	_context->board = new Board;
	stateMachine->add(new MainGameState(_context));
	stateMachine->add(new GameOverState(&gui,_context));
	stateMachine->add(new HighscoreState(&gui, _context));
	stateMachine->add(new MainMenuState(&gui, _context));
	stateMachine->connect("GameOver", 1, "MainGame");
	stateMachine->connect("GameOver", 2, "MainMenuState");
	stateMachine->connect("MainGame", 1, "GameOver");
	stateMachine->connect("MainMenuState", 3, "MainGame");
	return true;
}

void EnergyRush::init() {
	// for testing
	_context->reset();
	stateMachine->activate("MainMenuState");
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
	if (dlgID == 4 && button == 4) {
		shutdown();
	}
}
