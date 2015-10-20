#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"

const v2 RING_POSITIONS[] = {
	v2( 80, 680), // red
	v2( 80,  80), // green
	v2(940, 680), // blue
	v2(940,  80) // yellow
};

MainGameState::MainGameState(GameSettings* settings,GameContext* context) : ds::GameState("MainGame"), _settings(settings) , _context(context) {
	
}


MainGameState::~MainGameState() {
	
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MainGameState::init() {

}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	
}


// -------------------------------------------------------
// Update
// -------------------------------------------------------
int MainGameState::update(float dt) {
	
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	ds::sprites::draw(v2(CENTER_X, CENTER_Y), ds::math::buildTexture(ds::Rect(300, 400, 400, 400)), 0.0f, 2.0f, 2.0f);
	
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		return 1;
	}
	return 0;
}