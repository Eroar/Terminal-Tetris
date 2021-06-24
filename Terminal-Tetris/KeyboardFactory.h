#pragma once
#include "PlatformIdentifier.h"
#include "Keyboard.h"

#if PLATFORM == WINDOWS_PLATFORM
#include "WindowsKeyboard.h"
#endif

class KeyboardFactory {
public:
	static Keyboard* makeKeyboard() {
		if (PLATFORM == WINDOWS_PLATFORM) {
			return new WindowsKeyboard();
		}
		else {
			throw "Your platform isn't supported";
		}
	};
};
