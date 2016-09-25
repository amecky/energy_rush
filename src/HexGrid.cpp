#include "HexGrid.h"
#include <assert.h>
#include <renderer\graphics.h>
#include <core\math\tweening.h>
#include <core\math\math.h>
#include <base\InputSystem.h>
#include <resources\ResourceContainer.h>

HexGrid::HexGrid(GameContext* context) : _qMax(0), _rMax(0), _items(0), _layout(layout_pointy, v2(24.0f, 24.0f), v2(100, 130)) , _hover(-1) , _context(context) {
	_bombScript = ds::res::getScript("Bomb");
}

HexGrid::~HexGrid() {
	if (_items != 0) {
		delete[] _items;
	}
}

// -------------------------------------------------------
// resize
// -------------------------------------------------------
void HexGrid::resize(int qMax, int rMax) {
	if (_items != 0) {
		delete[] _items;
	}
	_qMax = qMax;
	_rMax = rMax;
	_items = new GridItem[qMax * rMax];
	fill();
}

// -------------------------------------------------------
// fill
// -------------------------------------------------------
void HexGrid::fill() {
	assert(_items != 0);
	for (int r = 0; r < _rMax; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _qMax - q_offset; q++) {
			GridItem item;
			item.hex = Hex(q, r);
			item.position = hex_math::hex_to_pixel(_layout, item.hex);
			item.bombCounter = 0;
			item.color = math::random(0, 4);
			item.state = IS_GROW;
			item.timer = 0.0f;
			item.counterScale = 1.0f;
			item.bombTimer = 0.0f;
			item.counterTimer = math::random(_context->settings.bombs.startDelay, _context->settings.bombs.endDelay);
			item.rotation = 0.0f;
			int idx = (q + q_offset) + r * _qMax;
			_items[idx] = item;
		}
	}
	_hover = -1;
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void HexGrid::fillBombs(int max) {
	int total = _qMax * _rMax;
	Hex* temp = new Hex[total];
	int cnt = 0;
	for (int r = 0; r < _rMax; r++) {
		int q_offset = r >> 1;
		for (int q = -q_offset; q < _qMax - q_offset; q++) {
			temp[cnt++] = Hex(q, r);
		}
	}
	for (int i = 0; i < total; ++i) {
		int idx = math::random(0, total - 1);
		Hex t = temp[i];
		temp[i] = temp[idx];
		temp[idx] = t;
	}
	for (int i = 0; i < max; ++i) {
		markAsBomb(temp[i]);
	}
	delete[] temp;
}

// -------------------------------------------------------
// get
// -------------------------------------------------------
const GridItem& HexGrid::get(const Hex& hex) const {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	return _items[idx];
}

// -------------------------------------------------------
// get by hex
// -------------------------------------------------------
GridItem& HexGrid::get(const Hex& hex) {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	return _items[idx];
}

// -------------------------------------------------------
// neighbors
// -------------------------------------------------------
int HexGrid::neighbors(const Hex& hex, Hex* ret) {
	int cnt = 0;
	for (int i = 0; i < 6; ++i) {
		Hex n = hex_math::neighbor(hex, i);
		if (isValid(n)) {
			ret[cnt++] = n;
		}
	}
	return cnt;
}

// -------------------------------------------------------
// convert from mouse pos
// -------------------------------------------------------
Hex HexGrid::convertFromMousePos() {
	v2 mp = ds::input::getMousePosition();
	return hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
}
// -------------------------------------------------------
// get by index
// -------------------------------------------------------
const GridItem& HexGrid::get(int index) const {
	return _items[index];
}

// -------------------------------------------------------
// get by index
// -------------------------------------------------------
GridItem& HexGrid::get(int index) {
	return _items[index];
}

// -------------------------------------------------------
// size
// -------------------------------------------------------
const int HexGrid::size() const {
	return _rMax * _qMax;
}

// -------------------------------------------------------
// select
// -------------------------------------------------------
int HexGrid::select(int x, int y) {
	v2 mp = ds::input::getMousePosition();
	Hex h = hex_math::hex_round(hex_math::pixel_to_hex(_layout, mp));
	int q_offset = h.r >> 1;
	int selected = -1;
	if (!isValid(h)) {
		return -1;
	}
	for (size_t i = 0; i < size(); ++i) {
		if (_items[i].hex == h) {
			selected = i;

		}
	}
	return selected;
}

// -------------------------------------------------------
// is valid
// -------------------------------------------------------
bool HexGrid::isValid(int q, int r) const {
	int q_offset = r >> 1;
	if (r < 0 || r >= _rMax) {
		return false;
	}
	if (q_offset + q < 0 || q_offset + q >= _qMax) {
		return false;
	}
	return true;
}

// -------------------------------------------------------
// is valid
// -------------------------------------------------------
bool HexGrid::isValid(const Hex& hex) const {
	return isValid(hex.q, hex.r);
}

// -------------------------------------------------------
// mark as bomb
// -------------------------------------------------------
void HexGrid::markAsBomb(const Hex& hex) {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	_items[idx].bombCounter = 10;
}

// -------------------------------------------------------
// pick a random color
// -------------------------------------------------------
void HexGrid::pickRandomColor(const Hex& hex) {
	int q_offset = hex.r >> 1;
	int idx = (hex.q + q_offset) + hex.r * _qMax;
	_items[idx].color = math::random(0, 4);
}

// -------------------------------------------------------
// set origin
// -------------------------------------------------------
void HexGrid::setOrigin(const v2& origin) {
	_layout.origin = origin;
}

// -------------------------------------------------------
// get index
// -------------------------------------------------------
int HexGrid::getIndex(const Hex& hex) const {
	int q_offset = hex.r >> 1;
	return (hex.q + q_offset) + hex.r * _qMax;
}

// -------------------------------------------------------
// swap colors
// -------------------------------------------------------
void HexGrid::swap(int firstIndex, int secondIndex) {
	GridItem first = _items[firstIndex];
	_items[firstIndex].color = _items[secondIndex].color;
	_items[secondIndex].color = first.color;
}

// -------------------------------------------------------
// decrement all bomb counters
// -------------------------------------------------------
bool HexGrid::decrementBombs() {
	for (int i = 0; i < size(); ++i) {
		GridItem& item = get(i);
		if (item.bombCounter > 0) {
			--item.bombCounter;
			if (item.bombCounter <= 0) {
				return true;
			}
		}
	}
	return false;
}

void HexGrid::flashBombs() {
	for (int i = 0; i < size(); ++i) {
		GridItem& item = get(i);
		if (item.bombCounter > 0) {
			item.counterScale = 1.0f;
			item.counterTimer = _context->settings.bombs.shakeTTL;
		}
	}
}

// -------------------------------------------------------
// update
// -------------------------------------------------------
void HexGrid::update(float dt) {
	// scaling based on item state
	const ds::vm::Method& m = _bombScript->getMethod(SID("shake"));
	const ds::vm::Method& wm = _bombScript->getMethod(SID("wiggle"));
	const ds::vm::Method& gm = _bombScript->getMethod(SID("grow"));
	const ds::vm::Method& sm = _bombScript->getMethod(SID("shrink"));
	const ds::vm::Method& wd = _bombScript->getMethod(SID("wiggledelay"));
	for (int i = 0; i < size(); ++i) {
		GridItem& item = get(i);
		if (item.state == IS_GROW) {
			item.timer += dt;
			_bombScript->set(0, v4(item.timer));
			item.scale = _bombScript->execute(gm).xy();
			if (_bombScript->getRegister(1).x >= 1.0f) {
				item.state = IS_NORMAL;
				item.scale = v2(1, 1);
			}
		}
		else if (item.state == IS_SHRINK) {
			item.timer += dt;
			_bombScript->set(0, v4(item.timer));
			item.scale = _bombScript->execute(sm).xy();
			if (_bombScript->getRegister(1).x >= 1.0f) {
				item.state = IS_GROW;
				item.timer = 0.0f;
				item.color = math::random(0, 4);
			}
		}
		if (item.state == IS_WIGGLE) {
			item.timer += dt;
			_bombScript->set(0, v4(item.timer));
			item.scale = _bombScript->execute(wm).xy();
			if (_bombScript->getRegister(1).x >= 1.0f) {
				item.state = IS_NORMAL;
				item.scale = v2(1, 1);
			}
		}
		if (item.bombCounter > 0 && item.counterTimer > 0.0f) {
			item.counterTimer -= dt;
			if (item.counterTimer <= 0.0f) {
				item.counterTimer = _bombScript->execute(wd).x;
				item.bombTimer = _bombScript->getRegister(1).x;// ->settings.bombs.shakeTTL;
				//item.counterTimer = math::random(_context->settings.bombs.startDelay, _context->settings.bombs.endDelay);
				item.counterScale = 1.0f;
			}
		}
		if (item.bombCounter > 0 && item.bombTimer > 0.0f) {
			item.bombTimer -= dt;
			_bombScript->set(0, v4(item.bombTimer));
			_bombScript->execute(m);
			item.rotation = _bombScript->getRegister(5).x;
			item.counterScale = _bombScript->getRegister(3).x;
		}
	}

	// hover
	Hex h = convertFromMousePos();
	if (isValid(h)) {
		int current = getIndex(h);
		if (current != _hover) {
			_hover = current;
			GridItem& item = get(h);
			if (item.state == IS_NORMAL) {
				item.state = IS_WIGGLE;
				item.timer = 0.0f;
			}
		}
	}
}

// -------------------------------------------------------
// find connected items
// -------------------------------------------------------
void HexGrid::findConnectedItems(const Hex& h, std::vector<Hex>& list) {
	if (isValid(h)) {
		Hex n[6];
		int cnt = neighbors(h, n);
		GridItem& current = get(h);
		for (int i = 0; i < cnt; ++i) {
			GridItem& item = get(n[i]);
			if (current.color == item.color) {
				bool found = false;
				for (size_t j = 0; j < list.size(); ++j) {
					if (list[j] == n[i]) {
						found = true;
					}
				}
				if (!found) {
					list.push_back(n[i]);
					findConnectedItems(n[i], list);
				}
			}
		}
	}
}

// -------------------------------------------------------
// refill
// -------------------------------------------------------
int HexGrid::refill(const std::vector<Hex>& list) {
	int ret = 0;
	for (size_t i = 0; i < list.size(); ++i) {
		GridItem& item = get(list[i]);
		if (item.bombCounter > 0) {
			item.bombCounter = 0;
			++ret;
		}
		item.state = IS_SHRINK;
		item.timer = 0.0f;
	}
	return ret;
}