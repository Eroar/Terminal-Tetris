#pragma once
#include "Button.h"
#include <stdexcept>


class Keyboard {
protected:
	Button buttons[256];

public:

	virtual void checkKeys() {};

	const Button& getButton(int buttonID) const {
		if (-1 < buttonID && buttonID < 256) {
			return buttons[buttonID];
		}
		else {
			throw std::out_of_range("Button id outside of bounds must be between 0 and 255");
		}
	};
};