#include "Board.h"
#include <sprites\SpriteBatch.h>
#include "Constants.h"
#include <Vector.h>

Board::Board() {
	_selected = -1;
	_maxBombs = 2;
	_grid.resize(20, 15);
	_grid.setOrigin(v2(100, 100));
}


Board::~Board() {
}

// -------------------------------------------------------
// fill bombs
// -------------------------------------------------------
void Board::refill() {
	_grid.fill();
	_grid.fillBombs(_maxBombs);
}

// -------------------------------------------------------
// activate
// -------------------------------------------------------
void Board::activate() {
	_maxBombs = 2;
	_width = 20;
	_height = 15;
	_selected = -1;
	_fadeOutEffect.deactivate();
}

// -------------------------------------------------------
// next level
// -------------------------------------------------------
void Board::nextLevel(int level) {
	_hover = -1;
	_killed = 0;
	_maxBombs = level * 2;
	refill();	
	_grid.decrementBombs();
	_fadeOutEffect.deactivate();
}
// -------------------------------------------------------
// dactivate
// -------------------------------------------------------
void Board::deactivate() {
}

void Board::fadeOut() {
	_fadeOutEffect.activate();
}

// -------------------------------------------------------
// on button up
// -------------------------------------------------------
ClickResult Board::onClick(int x, int y) {
	ClickResult result;
	Hex h = _grid.convertFromMousePos();
	if (_grid.isValid(h)) {
		const GridItem& item = _grid.get(h);
		if (item.bombCounter == 0) {
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
					int firstSize = list.size();
					GridItem& selItem = _grid.get(_selected);
					_grid.findConnectedItems(selItem.hex, list);
					int secondSize = list.size() - firstSize;
					// check if both will remove more than 2
					if (firstSize >= 3 && secondSize >= 3) {
						// reset selection
						_selected = -1;
						// refill items
						int itemsKilled = _grid.refill(list);
						_killed += itemsKilled;
						result.collected = itemsKilled;
						int d = _maxBombs - _killed;
						// all bombs killed -> next level
						if (d <= 0) {
							result.finished = true;
							dec = false;
						}
						result.bombsRemoved = itemsKilled;
						// set points
						result.collected += list.size();
					}
					else {
						// swap back - no legal move
						_grid.swap(r, _selected);
					}
					if (dec) {
						if (_grid.decrementBombs()) {
							result.killed = true;
						}
					}
				}
			}
			else {
				_selected = -1;
			}
		}
	}
	return result;
}
// -------------------------------------------------------
// Update
// -------------------------------------------------------
int Board::update(float dt) {

	_grid.update(dt);
	_fadeOutEffect.update(dt);
	return 0;
}

// -------------------------------------------------------
// render
// -------------------------------------------------------
void Board::render() {
	ds::sprites::draw(v2(256, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(768, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(256, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	ds::sprites::draw(v2(768, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));

	_fadeOutEffect.begin();

	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		int offset = item.color * 40;
		ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, offset, 40, 44)), 0.0f, item.scale.x, item.scale.y);
		if (item.bombCounter > 0 && (item.state == IS_NORMAL || item.state == IS_WIGGLE)) {
			offset = 200 + (item.bombCounter - 1) * 30;
			ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(0, offset, 30, 18)), 0.0f, 1.0f, 1.0f, ds::Color(192, 192, 192, 255));
		}
	}
	if (_selected != -1) {
		const GridItem& item = _grid.get(_selected);
		ds::sprites::draw(item.position, ds::math::buildTexture(ds::Rect(50, 210, 56, 60)));
	}
	_fadeOutEffect.end();
}
