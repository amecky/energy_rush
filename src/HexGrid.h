#pragma once
#include <Vector.h>
struct Hex {
	int q;
	int r;
};

namespace hex_math {

	v2 to_pixel(const Hex& h, int size);
}