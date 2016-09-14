#include "Board.h"
#include <renderer\sprites.h>
#include "Constants.h"
#include <Vector.h>

Board::Board(GameContext* context) : _context(context) , _grid(context) {
	_selection.texture = math::buildTexture(ds::Rect(0, 50, 44, 50));
	_selection.index = -1;
	_selection.timer = 0.0f;
	_maxBombs = 2;
	_grid.resize(20, 15);
	_grid.setOrigin(v2(100, 100));
	_pieceTexture = math::buildTexture(ds::Rect(0, 0, 40, 46));
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
	_selection.index = -1;
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
}
// -------------------------------------------------------
// dactivate
// -------------------------------------------------------
void Board::deactivate() {
}

void Board::fadeOut() {
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
			if (_selection.index != r) {
				if (_selection.index == -1) {
					_selection.index = r;
				}
				else {
					bool dec = true;
					// swap elements
					_grid.swap(_selection.index, r);
					std::vector<Hex> list;
					_grid.findConnectedItems(h, list);
					int firstSize = list.size();
					GridItem& selItem = _grid.get(_selection.index);
					_grid.findConnectedItems(selItem.hex, list);
					int secondSize = list.size() - firstSize;
					// check if both will remove more than 2
					if (firstSize >= 3 && secondSize >= 3) {
						// reset selection
						_selection.index = -1;
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
						_grid.swap(r, _selection.index);
					}
					if (dec) {
						_grid.flashBombs();
						if (_grid.decrementBombs()) {							
							result.killed = true;
						}
					}
				}
			}
			else {
				_selection.index = -1;
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
	if (_selection.index != -1) {
		_selection.timer += dt;
	}
	return 0;
}

void Board::flashBombs() {
	_grid.flashBombs();
}
// -------------------------------------------------------
// render
// -------------------------------------------------------
void Board::render() {
	//ds::sprites::draw(v2(256, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	//ds::sprites::draw(v2(768, 192), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	//ds::sprites::draw(v2(256, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));
	//ds::sprites::draw(v2(768, 576), ds::math::buildTexture(0.0f, 512.0f, 512.0f, 384.0f));

	//_fadeOutEffect->begin();
	ds::SpriteBuffer* sprites = graphics::getSpriteBuffer();
	for (int i = 0; i < _grid.size(); ++i) {
		const GridItem& item = _grid.get(i);
		int offset = item.color * 40;
		sprites->draw(item.position, _pieceTexture, 0.0f, item.scale, _context->colors[item.color]);
		// draw number
		if (item.bombCounter > 0 && (item.state == IS_NORMAL || item.state == IS_WIGGLE)) {
			offset = 200 + (item.bombCounter - 1) * 30;
			sprites->draw(item.position, math::buildTexture(ds::Rect(20, offset, 30, 22)), item.rotation, v2(item.counterScale, item.counterScale));// , ds::Color(192, 192, 192, 255));
		}
	}
	if (_selection.index != -1) {
		const GridItem& item = _grid.get(_selection.index);
		float s = 1.0f + sin(_selection.timer * 10.0f) * 0.05f;
		sprites->draw(item.position, _selection.texture,0.0f,v2(s,s));
	}
	//_fadeOutEffect->end();
}
