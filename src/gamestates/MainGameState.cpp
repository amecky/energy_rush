#include "MainGameState.h"
#include <sprites\SpriteBatch.h>
#include "..\Constants.h"
#include <Vector.h>

MainGameState::MainGameState(GameContext* context) : ds::GameState("MainGame"), _context(context) {
	_selected = -1;
	_maxBombs = 60;
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
	/*
	int total = _width * _height;
	Hex* temp = new Hex[total];
	int cnt = 0;
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			temp[cnt++] = Hex(q, r);
		}
	}
	for (int i = 0; i < total; ++i) {
		int idx = ds::math::random(0, total - 1);
		Hex t = temp[i];
		temp[i] = temp[idx];
		temp[idx] = t;
	}
	for (int i = 0; i < _maxBombs; ++i) {
		_grid.markAsBomb(temp[i]);
	}
	Hex n[6];
	for (int r = 0; r < _height; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _width - q_offset; q++) {
			Hex h = Hex(q, r);
			if (_grid.isValid(h)) {
				int cnt = _grid.neighbors(h, n);
				GridItem& current = _grid.get(h);
				for (int i = 0; i < cnt; ++i) {
					const GridItem& item = _grid.get(n[i]);
					if (item.bomb) {
						++current.adjacentBombs;
					}
				}
			}
		}
	}
	delete[] temp;
	*/
}
// -------------------------------------------------------
// activate
// -------------------------------------------------------
void MainGameState::activate() {
	_grid.resize(20,15);
	_grid.setOrigin(v2(100,100));
	_width = 20;
	_height = 15;
	_selected = -1;
	fillBombs();
	_context->marked = 0;
	_context->markedCorrectly = 0;
	_hud.setTimer(0, 0, 0);
	_hover = -1;
}

void MainGameState::deactivate() {
	ds::GameTimer *timer = _hud.getTimer(0);
	_context->playedSeconds = timer->getSeconds();
	_context->playedMinutes = timer->getMinutes();
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	Hex h = _grid.convertFromMousePos();
	if (_grid.isValid(h)) {
		int r = _grid.select(x, y);
		if (_selected != r) {
			if (_selected == -1) {
				_selected = r;
			}
			else {
				// swap elements
				_grid.swap(_selected, r);
				std::vector<Hex> list;				
				_grid.findConnectedItems(h, list);
				LOG << "1: connected: " << list.size();
				GridItem& selItem = _grid.get(_selected);
				_grid.findConnectedItems(selItem.hex, list);
				LOG << "2: connected: " << list.size();				
				_selected = -1;
				_grid.refill(list);
				
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

