/** @file */
#pragma once

#include <ostream>

/** @brief Class representing a character
 *  @remark A class shouldinherit from it and provide OS specific Character class
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Character
{
	char glyph;

public:
	// Default constructor
	Character() : glyph(' '){};

	// Parameter constructor
	Character(const char _glyph) : glyph(_glyph){};

	char getGlyph() const
	{
		return glyph;
	};

	// Copy constructor
	Character(const Character &c) : glyph(c.getGlyph()){};
	void const setGlyph(char c)
	{
		glyph = c;
	}
};

// sends glyps into the out stream
std::ostream &operator<<(std::ostream &out, const Character &c)
{
	out << c.getGlyph();
	return out;
}
