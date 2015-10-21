#pragma once
#include <base\GameState.h>
#include <renderer\render_types.h>
#include <ui\IMGUI.h>
#include "..\GameSettings.h"
#include "..\GameContext.h"
#include "..\HexGrid.h"
#include <vector>
#include "..\Constants.h"

struct GridItem {

	Hex hex;
	v2 position;
	int state;
	bool bomb;
	int adjacentBombs;
};

class HexGrid {

public:
	HexGrid() : _width(0), _height(0), _items(0), _layout(layout_pointy, v2(24.0f, 24.0f), v2(100, 100)) {}
	~HexGrid() {
		if (_items != 0) {
			delete[] _items;
		}
	}

	void init(int w, int h) {
		if (_items != 0) {
			delete[] _items;
		}
		_items = new GridItem[w * h];
		_width = w;
		_height = h;
	}

	void set(int q, int r) {
		int idx = getIndex(q, r);
		GridItem& item = _items[idx];
		_items[idx].state = 1;
	}

	void setState(int q, int r,int state) {
		int idx = getIndex(q, r);
		GridItem& item = _items[idx];
		_items[idx].state = state;
	}

	void markBomb(int q, int r) {
		int idx = getIndex(q, r);
		GridItem& item = _items[idx];
		_items[idx].bomb = true;
	}

	bool isBomb(int q, int r) const {
		int idx = getIndex(q, r);
		return _items[idx].bomb;
	}

	const GridItem& get(int q, int r) const {
		int idx = getIndex(q, r);
		return _items[idx];
	}

	const GridItem& get(int idx) const {
		return _items[idx];
	}

	GridItem& get(int idx) {
		return _items[idx];
	}

	GridItem& get(int q, int r) {
		int idx = getIndex(q, r);
		return _items[idx];
	}

	void fill() {
		for (int r = 0; r < GRID_HEIGHT; ++r) {
			int q_offset = r >> 1;
			for (int q = -q_offset; q < GRID_WIDTH - q_offset; ++q) {
				LOG << "r: " << r << " q: " << q << " q_offset: " << q_offset;
				int idx = getIndex(q, r);
				GridItem& item = _items[idx];
				item.hex = Hex(q, r);				
				item.position = hex_math::hex_to_pixel(_layout, item.hex);
				item.state = 0;
				item.bomb = false;
				item.adjacentBombs = 0;
			}
		}
	}

	bool isValid(int q, int r) const {
		int q_offset = r >> 1;
		if (r < 0 || r > GRID_HEIGHT) {
			return false;
		}
		if ((q_offset + q) < 0 || (q + q_offset) > GRID_WIDTH) {
			return false;
		}
		return true;
	}

private:
	int getIndex(int q, int r) const {
		int q_offset = r >> 1;
		return (q_offset + q + r * _width);
	}
	
	int _width;
	int _height;
	GridItem* _items;
	Layout _layout;
};

class MainGameState : public ds::GameState {

public:
	MainGameState(GameSettings* settings,GameContext* context);
	virtual ~MainGameState();
	void init();
	int update(float dt);
	void render();
	void activate();
	int onChar(int ascii);
	int onButtonUp(int button, int x, int y);
private:
	void fillGrid(int maxBombs);
	void openEmptyCells(const Hex& h);
	void debug();

	GameSettings* _settings;
	GameContext* _context;
	Layout _layout;
	HexGrid _grid;
	int _marked;
};

