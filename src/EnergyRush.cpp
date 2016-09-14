#include "EnergyRush.h"
#include <core\log\Log.h>
#include "Constants.h"
#include <gamestates\GameStateMachine.h>
#include "gamestates\MainGameState.h"

ds::BaseApp *app = new EnergyRush(); 

EnergyRush::EnergyRush() : ds::BaseApp() {	
}

EnergyRush::~EnergyRush() {
	delete _context;
}

// -------------------------------------------------------
// prepare
// -------------------------------------------------------
void EnergyRush::prepare(ds::Settings* settings) {
	settings->screenWidth = 1024;
	settings->screenHeight = 768;
	settings->clearColor = ds::Color(0, 0, 0, 255);
	settings->fullScreen = false;
	settings->reportingDirectory = "reports";
	settings->synched = true;
	settings->logTypes = ds::LogTypes::LT_CONSOLE | ds::LogTypes::LT_FILE;
}

// -------------------------------------------------------
// Load content and prepare game
// -------------------------------------------------------
bool EnergyRush::loadContent() {	
	_context = new GameContext;
	_context->settings.load();
	//_context->hudDialog = gui->get("HUD");
	//_context->board = new Board;
	addGameState(new MainGameState(_context));

	RID material = ds::res::find("SpriteMaterial", ds::ResourceType::MATERIAL);
	ds::Material* m = ds::res::getMaterial(material);
	m->texture = ds::res::find("TextureArray", ds::ResourceType::TEXTURE);

	_context->pick_colors();
	return true;
}

void EnergyRush::init() {
	// for testing
	_context->reset();
	activate("MainGame");
}


// -------------------------------------------------------
// Update
// -------------------------------------------------------
void EnergyRush::update(float dt) {
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void EnergyRush::render() {
	
}
