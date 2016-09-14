#pragma once
#include <Vector.h>
#include <core\math\hex.h>
#include <vector>
#include "GameContext.h"

enum ItemState {
	IS_NORMAL,
	IS_WIGGLE,
	IS_SHRINK,
	IS_GROW
};

struct GridItem {

	Hex hex;
	v2 position;
	v2 scale;
	float rotation;
	int bombCounter;
	int color;
	ItemState state;
	float timer;
	float counterScale;
	float counterTimer;
	float bombTimer;
};

class HexGrid {

public:
	HexGrid(GameContext* context);
	~HexGrid();
	void resize(int qMax, int rMax);
	void fill();
	void fillBombs(int max);
	const GridItem& get(int index) const;
	GridItem& get(int index);
	const GridItem& get(const Hex& hex) const;
	GridItem& get(const Hex& hex);
	const int size() const;
	int select(int x, int y);
	bool isValid(int q, int r) const;
	bool isValid(const Hex& hex) const;
	void markAsBomb(const Hex& hex);
	int neighbors(const Hex& hex,Hex* ret);
	Hex convertFromMousePos();
	void setOrigin(const v2& origin);
	int getIndex(const Hex& h)const ;
	void swap(int firstIndex, int secondIndex);
	void pickRandomColor(const Hex& h);
	void update(float dt);
	void findConnectedItems(const Hex& h, std::vector<Hex>& list);
	int refill(const std::vector<Hex>& list);
	bool decrementBombs();
	void flashBombs();
private:
	int _qMax;
	int _rMax;
	GridItem* _items;
	GameContext* _context;
	Layout _layout;
	int _hover;
};