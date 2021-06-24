/** @file */
#pragma once
#include "PlatformIdentifier.h"
#include "Keyboard.h"

#if PLATFORM == WINDOWS_PLATFORM
#include "WindowsKeyboard.h"
#endif

/** @brief Class that is used for getting a keyboard based on the OS
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class KeyboardFactory
{
public:
	static Keyboard *makeKeyboard()
	{
		if (PLATFORM == WINDOWS_PLATFORM)
		{
			return new WindowsKeyboard();
		}
		else
		{
			throw "Your platform isn't supported";
		}
	};
};
