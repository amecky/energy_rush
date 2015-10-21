#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"


MainGameState::MainGameState(GameSettings* settings, GameContext* context) 
	: ds::GameState("MainGame"), _settings(settings), _context(context), _layout(layout_pointy, v2(24.0f, 24.0f), v2(100, 100)) {
	for (int r = 0; r < GRID_HEIGHT; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < GRID_WIDTH - q_offset; q++) {
			GridItem item;
			item.hex = Hex(q, r);
			item.position = hex_math::hex_to_pixel(_layout, item.hex);
			_items.push_back(item);
		}
	}
	_selected = -1;
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

int MainGameState::onButtonUp(int button, int x, int y) {
	v2 mp = ds::renderer::getMousePosition();
	Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
	_selected = -1;
	LOG << "h: " << h.q << " " << h.r;
	for (size_t i = 0; i < _items.size(); ++i) {
		if (_items[i].hex == h) {
			_selected = i;
		}
	}
	return 0;
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
	for (size_t i = 0; i < _items.size(); ++i) {
		if (i == _selected) {
			ds::sprites::draw(_items[i].position, ds::math::buildTexture(ds::Rect(400, 42, 40, 44)));
		}
		else {
			ds::sprites::draw(_items[i].position, ds::math::buildTexture(ds::Rect(400, 0, 40, 44)));
		}
	}
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