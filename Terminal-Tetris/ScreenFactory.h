#pragma once
#include "Screen.h"
#include "PlatformIdentifier.h"

#if PLATFORM == WINDOWS_PLATFORM
#include "WindowsCMDScreen.h"
#endif

class ScreenFactory {
public:
	static Screen* makeScreen(const int _width, const int _height) {
		if (PLATFORM == WINDOWS_PLATFORM) {

			return new WindowsCMDScreen(_width, _height);
		}
		else {
			throw "Your platform isn't supported";
		}
	};
};
