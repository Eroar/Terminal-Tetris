/** @file */
#pragma once
#include <string>
#include "ScreenObject.h"
#include <stdexcept>

/** @brief Class that is used to represent text that can be displayed on screen
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class TextObject : public ScreenObject
{
public:
	TextObject(std::string id, int _xPos, int _yPos, std::string _text, int length, char align = 'C') : ScreenObject(id, _xPos, _yPos, Surface(1, length))
	{
		setText(_text, align);
	};

	TextObject(std::string id, int _xPos, int _yPos, std::string _text, char align = 'C') : TextObject(id, _xPos, _yPos, _text, _text.length(), align){};

	void setText(const std::string s, char align = 'R')
	{
		if (s.length() > surf.getWidth())
		{
			throw std::invalid_argument("New text too long for this object");
		}
		//allign argument can be 'L', 'R' or 'C' - center
		surf.clear();
		if (align == 'R')
		{
			surf.writeText(s, 0, surf.getWidth() - s.length());
		}
		else if (align == 'L')
		{
			surf.writeText(s, 0, 0);
		}
		else
		{
			int pos = (surf.getWidth() - s.length()) / 2;
			surf.writeText(s, 0, pos);
		}
	}
};