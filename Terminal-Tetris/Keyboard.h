/** @file */
#pragma once
#include "Button.h"
#include <stdexcept>

/** @brief Abstract object that represents a keyboard
 *  @remark A class should inherit from it and implement OS depended way of checking keys state
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Keyboard
{
protected:
	Button buttons[256];

public:
	virtual void checkKeys(){};

	const Button &getButton(int buttonID) const
	{
		if (-1 < buttonID && buttonID < 256)
		{
			return buttons[buttonID];
		}
		else
		{
			throw std::out_of_range("Button id outside of bounds must be between 0 and 255");
		}
	};
};