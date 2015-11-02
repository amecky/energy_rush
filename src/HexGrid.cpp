#include "HexGrid.h"
#include <assert.h>
#include <renderer\graphics.h>
#include <math\tweening.h>

HexGrid::HexGrid() : _qMax(0), _rMax(0), _items(0), _layout(layout_pointy, v2(24.0f, 24.0f), v2(100, 130)) , _hover(-1) {
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
			item.color = ds::math::random(0, 4);
			item.state = IS_GROW;
			item.timer = 0.0f;
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
		int idx = ds::math::random(0, total - 1);
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
	v2 mp = ds::renderer::getMousePosition();
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
	v2 mp = ds::renderer::getMousePosition();
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
	_items[idx].color = ds::math::random(0, 4);
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

// -------------------------------------------------------
// update
// -------------------------------------------------------
void HexGrid::update(float dt) {
	// scaling based on item state
	for (int i = 0; i < size(); ++i) {
		GridItem& item = get(i);
		if (item.state == IS_GROW) {
			item.timer += dt;
			float norm = item.timer / 0.4f;
			item.scale = tweening::interpolate(tweening::easeInQuad, v2(0.1f, 0.1f), v2(1.0f, 1.0f), norm);
			if (norm >= 1.0f) {
				item.state = IS_NORMAL;
				item.scale = v2(1, 1);
			}
		}
		else if (item.state == IS_SHRINK) {
			item.timer += dt;
			float norm = item.timer / 0.4f;
			item.scale = tweening::interpolate(tweening::easeInQuad, v2(1.0f, 1.0f), v2(0.05f, 0.05f), norm);
			if (norm >= 1.0f) {
				item.state = IS_GROW;
				item.timer = 0.0f;
				item.color = ds::math::random(0, 4);
			}
		}
		if (item.state == IS_WIGGLE) {
			item.timer += dt;
			float norm = item.timer / 0.4f;
			item.scale.x = 0.8f + sin(item.timer * 5.0f) * 0.2f;
			item.scale.y = 0.8f + sin(item.timer * 5.0f) * 0.2f;
			if (norm >= 1.0f) {
				item.state = IS_NORMAL;
				item.scale = v2(1, 1);
			}
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