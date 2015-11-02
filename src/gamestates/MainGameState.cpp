#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"
#include <Vector.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_selected = -1;
	_maxBombs = 2;
	_grid.resize(20, 15);
	_grid.setOrigin(v2(100, 100));
}


MainGameState::~MainGameState() {
	
}

// -------------------------------------------------------
// init
// -------------------------------------------------------
void MainGameState::init() {
	_hud.init(0, "xscale");
	ds::assets::load("hud", &_hud, ds::CVT_HUD);
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void MainGameState::fillBombs() {
	_grid.fill();
	_grid.fillBombs(_maxBombs);
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	
	_maxBombs = 2;
	_width = 20;
	_height = 15;
	_selected = -1;
	
	_context->collected = 0;
	_context->points = 0;
	_context->level = 0;
	_hud.setTimer(0, 0, 0);
	nextLevel();
	
}

// -------------------------------------------------------
// next level
// -------------------------------------------------------
void MainGameState::nextLevel() {
	++_context->level;
	_hover = -1;
	_killed = 0;
	_context->kills = 0;
	_maxBombs = _context->level * 2;
	fillBombs();
	_hud.setCounterValue(1, _maxBombs);
	_hud.setCounterValue(0, 0);
	_grid.decrementBombs();
}
// -------------------------------------------------------
// dactivate
// -------------------------------------------------------
void MainGameState::deactivate() {
	ds::GameTimer *timer = _hud.getTimer(0);
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	Hex h = _grid.convertFromMousePos();
	if (_grid.isValid(h)) {
		// FIXME: user cannot select bomb
		int r = _grid.select(x, y);
		if (_selected != r) {
			if (_selected == -1) {
				_selected = r;
			}
			else {
				bool dec = true;
				// swap elements
				_grid.swap(_selected, r);
				std::vector<Hex> list;				
				_grid.findConnectedItems(h, list);
				LOG << "1: connected: " << list.size();
				int firstSize = list.size();
				GridItem& selItem = _grid.get(_selected);
				_grid.findConnectedItems(selItem.hex, list);
				int secondSize = list.size() - firstSize;
				// check if both will remove more than 2
				LOG << "2: connected: " << list.size();
				if (firstSize >= 3 && secondSize >= 3) {					
					// reset selection
					_selected = -1;
					// refill items
					int itemsKilled = _grid.refill(list);
					_killed += itemsKilled;
					_context->kills += itemsKilled;
					LOG << "killed: " << _killed;
					int d = _maxBombs - _killed;
					// all bombs killed -> next level
					if (d <= 0) {
						LOG << "NEXT LEVEL";
						nextLevel();
						dec = false;
					}
					_hud.setCounterValue(1, d);
					// set points
					_context->collected += list.size();
					_hud.setCounterValue(0, _context->collected);
				}
				else {
					// swap back - no legal move
					_grid.swap(r, _selected);
				}
				if (dec) {
					if (_grid.decrementBombs()) {
						LOG << "GAME OVER!!!";
						// FIXME: game over
					}
				}

			}
		}
		else {
			_selected = -1;
		}
	}
	return 0;
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
int MainGameState::update(float dt) {

	_grid.update(dt);

	_hud.update(dt);

	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void MainGameState::render() {
	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		int offset = item.color * 40;
		ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, offset, 40, 44)), 0.0f, item.scale.x, item.scale.y);		
		if (item.bombCounter > 0 && (item.state == IS_NORMAL || item.state == IS_WIGGLE)) {
			offset = 200 + (item.bombCounter - 1) * 30;
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, offset, 30, 18)),0.0f,1.0f,1.0f,ds::Color(192,192,192,255));
		}
	}
	if ( _selected != -1) {
		const GridItem& item = _grid.get(_selected);
		ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, 210, 56, 60)));
	}
	_hud.render();
}

// -------------------------------------------------------
// on char
// -------------------------------------------------------
int MainGameState::onChar(int ascii) {	
	if (ascii == 'e') {
		return 1;
	}
	if (ascii == 'r') {
		fillBombs();
	}
	return 0;
}

