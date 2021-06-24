/** @file */
#pragma once
#include <algorithm>
#include <ostream>
#include <string>
#include <cmath>
#include "Matrix2D.h"
#include "PlatformIdentifier.h"

#if PLATFORM == WINDOWS_PLATFORM
#include "WindowsCharacter.h"
#define CHARACTER_TYPE WindowsCharacter

#else
#include "Character.h"
#define CHARACTER_TYPE Character
#endif

#define CHARACTER_SIZE sizeof(CHARACTER_TYPE)

/** @brief Class that is used to represent a group of characters
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Surface
{
protected:
	Matrix2D<CHARACTER_TYPE> content;

public:
	Surface(const int _height, const int _width) : content(_height, _width){};
	Surface(const Surface &source) : content(source.content){};
	Surface(Surface &&source) noexcept : content(std::move(source.content)){};

	Surface(const int _height, const int _width, const std::string &c) : content(_height, _width)
	{
		setContent(c);
	};

	void clear()
	{
		content.reset();
	};

	size_t getWidth() const
	{
		return content.colCount();
	}

	Matrix2D<CHARACTER_TYPE> *getRow(size_t row)
	{
		return content.getRow(row);
	}

	void setRow(size_t row, const CHARACTER_TYPE values[])
	{
		content.setRow(row, values);
	}

	Matrix2D<CHARACTER_TYPE> *getCol(size_t col)
	{
		return content.getCol(col);
	}

	void setCol(size_t col, const CHARACTER_TYPE values[])
	{
		content.setCol(col, values);
	}

	size_t getHeight() const
	{
		return content.rowCount();
	}

	void rotateRight()
	{
		content.rotateRight();
	}

	void rotateLeft()
	{
		content.rotateLeft();
	}

	CHARACTER_TYPE &getCharAt(const size_t row, const size_t col) const
	{
		return content(row, col);
	}

	void setCharAt(const size_t row, const size_t col, const CHARACTER_TYPE c)
	{
		content.setAt(row, col, c);
	}

	void setContent(const std::string &s)
	{
		if (s.length() != getHeight() * getWidth())
		{
			throw std::invalid_argument("Length of the string s argument doesn't match width*height of the surface");
		}

		for (size_t i = 0; i < getHeight() * getWidth(); i++)
		{
			content.as1DArr()[i].setGlyph(s.at(i));
		}
	};

	void writeText(const std::string &text, long row, long col)
	{
		if (-1 < row && row < (long)getHeight())
		{
			for (size_t i = 0; i < text.length(); i++)
			{
				if (-1 < col && col < (long)getWidth())
				{
					content(row, col).setGlyph(text.at(i));
				}
				col++;
			}
		}
	}
	void blit(const Surface &source, const long _col, const long _row, char alpha_char = ' ')
	{
		//blits a surface onto this surface
		// ' ' char will be omitted
		//copies content of surf to this.content
		for (long row = 0; row < (long)source.getHeight(); row++)
		{
			if ((row + _row > -1) && (row + _row < (long)getHeight()))
			{
				for (long col = 0; col < (long)source.getWidth(); col++)
				{
					if (_col + col > -1 && _col + col < (long)getWidth())
					{
						CHARACTER_TYPE c = source.getCharAt(row, col);
						if (c.getGlyph() != alpha_char)
						{
							content.setAt(_row + row, _col + col, c);
						}
					}
				}
			}
		}
	};

	Surface *scaled(const size_t scale) const
	{
		Surface *s = new Surface(getHeight() * scale, getWidth() * scale);
		for (size_t row = 0; row < getHeight(); row++)
		{
			for (size_t col = 0; col < getWidth(); col++)
			{
				// FOR EVERY CHARACTER IN SURFACE
				CHARACTER_TYPE c = getCharAt(row, col);
				//FILLS THE SCALED SURFACE
				for (unsigned short rowOffset = 0; rowOffset < scale; rowOffset++)
				{
					for (unsigned short colOffset = 0; colOffset < scale; colOffset++)
					{
						s->setCharAt(row * scale + rowOffset, col * scale + colOffset, c);
					}
				}
			}
		}
		return s;
	};

	void operator=(Surface &&s) noexcept
	{
		content = std::move(s.content);
	};

	void operator=(const Surface &s) noexcept
	{
		content = s.content;
	};

	friend std::ostream &operator<<(std::ostream &out, Surface s);
};

std::ostream &operator<<(std::ostream &out, Surface s)
{
	out << s.content;
	return out;
}