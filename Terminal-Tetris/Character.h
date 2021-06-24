#pragma once

#include <ostream>


class Character
{
	char glyph;
public:
	// Default constructor
	Character() : glyph(' ') {};

	// Parameter constructor
	Character(const char _glyph) : glyph(_glyph) {};

	// returns Glyph
	char getGlyph() const {
		return glyph;
	};
	// Copy constructor
	Character(const Character& c) : glyph(c.getGlyph()) {};
	void const setGlyph(char c) {
		glyph = c;
	}
};

std::ostream& operator<<(std::ostream& out, const Character& c) {
	out << c.getGlyph();
	return out;
}
