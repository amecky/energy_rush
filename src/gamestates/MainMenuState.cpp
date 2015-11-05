#include "MainMenuState.h"
#include <utils\Log.h>
#include <sprites\SpriteBatch.h>
#include <renderer\graphics.h>
#include <base\GameStateMachine.h>

MainMenuState::MainMenuState(ds::DialogManager* gui, GameContext* context) : ds::GameState("MainMenuState"), _context(context), _gui(gui) {
}


MainMenuState::~MainMenuState() {
}

// --------------------------------------------
// activate
// --------------------------------------------
void MainMenuState::activate() {
	_gui->activate("MainMenu");		
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
	ds::sprites::draw(v2(256, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(768, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(256, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(768, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
}


