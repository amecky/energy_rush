#include "MainGameState.h"
#include <sprites\SpriteBatch.h>



MainGameState::MainGameState(GameSettings* settings, GameContext* context) 
	: ds::GameState("MainGame"), _settings(settings), _context(context), _layout(layout_pointy, v2(24.0f, 24.0f), v2(100, 100)) {
	_grid.init(GRID_WIDTH, GRID_HEIGHT);
}


MainGameState::~MainGameState() {

}

// -------------------------------------------------------
// fill grid
// -------------------------------------------------------
void MainGameState::fillGrid(int maxBombs) {
	_grid.fill();
	int total = GRID_WIDTH * GRID_HEIGHT;
	Vector2i* tmp = new Vector2i[total];
	int cnt = 0;
	for (int r = 0; r < GRID_HEIGHT; ++r) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < GRID_WIDTH - q_offset; ++q) {
			tmp[cnt++] = Vector2i(q, r);
		}
	}

	for (int i = total - 1; i > 0; --i) {
		int idx = ds::math::random(0, total - 1);
		Vector2i current = tmp[i];
		tmp[i] = tmp[idx];
		tmp[idx] = current;
	}

	for (int i = 0; i < maxBombs; ++i) {
		v2 p = tmp[i];
		if (_grid.isValid(p.x, p.y)) {
			_grid.markBomb(p.x, p.y);
		}		
	}
	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		GridItem& item = _grid.get(i);
		Hex h = item.hex;
		int cnt = 0;
		for (int j = 0; j < 6; ++j) {
			Hex n = hex_math::neighbor(h, j);
			if (_grid.isValid(n.q, n.r) && _grid.isBomb(n.q,n.r)) {
				++cnt;
			}
		}
		item.adjacentBombs = cnt;
	}
	_marked = 0;
	delete[] tmp;
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
	fillGrid(60);
}

// -------------------------------------------------------
// open empty cells
// -------------------------------------------------------
void MainGameState::openEmptyCells(const Hex& h) {
	GridItem& item = _grid.get(h.q, h.r);
	if (item.adjacentBombs == 0 && item.state == 0) {
		item.state = 1;
		for (int j = 0; j < 6; ++j) {
			Hex n = hex_math::neighbor(h, j);
			if (_grid.isValid(n.q, n.r)) {
				GridItem& nb = _grid.get(n.q, n.r);
				if (nb.adjacentBombs == 0) {
					nb.state = 1;
					openEmptyCells(n);
				}
			}
		}
	}
}

// -------------------------------------------------------
// on button
// -------------------------------------------------------
int MainGameState::onButtonUp(int button, int x, int y) {
	if (button == 0) {
		v2 mp = ds::renderer::getMousePosition();
		Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
		GridItem& item = _grid.get(h.q, h.r);
		if (item.state != 2) {
			if (_grid.isBomb(h.q, h.r)) {
				LOG << "DEAD!!!!";
				return 1;
			}
			LOG << "adjacent: " << item.adjacentBombs;
			if (item.adjacentBombs == 0) {
				openEmptyCells(h);
			}
			item.state = 1;
		}
	}
	// mark or unmark cell
	else {
		v2 mp = ds::renderer::getMousePosition();
		Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
		GridItem& item = _grid.get(h.q, h.r);
		if (item.state == 2) {
			--_marked;
			if (_marked < 0) {
				_marked = 0;
			}
			item.state = 0;
		}
		else {
			++_marked;
			item.state = 2;
			// check if all marked spots are correct

			// if correct marked == maxBombs -> WIN!
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
	for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i) {
		const GridItem& item = _grid.get(i);
		if (item.state == 1) {
			int offset = 40 * item.adjacentBombs;
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, offset, 40, 44)));
		}
		else if (item.state == 2) {
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, 120, 40, 44)));
		}
		else {
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, 40, 40, 44)));
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
	if (ascii == 'r') {
		fillGrid(60);
	}
	if (ascii == 'd') {
		debug();
	}
	return 0;
}

void MainGameState::debug() {
	char buffer[32];
	for (int r = 0; r < GRID_HEIGHT; ++r) {
		std::string line;
		for (int q = 0; q < GRID_WIDTH; ++q) {
			if (_grid.isValid(q, r)) {
				const GridItem& item = _grid.get(q, r);
				if (item.bomb) {
					line += "x ";
				}
				else {
					sprintf_s(buffer, 32, "%1d ", item.adjacentBombs);
					line += buffer;
				}
			}
			else {
				line += "- ";
			}
		}
		LOG << line;
	}
}