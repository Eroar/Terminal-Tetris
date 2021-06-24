/** @file */
#pragma once
#include "Screen.h"
#include "PlatformIdentifier.h"

#if PLATFORM == WINDOWS_PLATFORM
#include "WindowsCMDScreen.h"
#endif

/** @brief Class that is used for getting a screen based on the OS
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class ScreenFactory
{
public:
	static Screen *makeScreen(const int _width, const int _height)
	{
		if (PLATFORM == WINDOWS_PLATFORM)
		{

			return new WindowsCMDScreen(_width, _height);
		}
		else
		{
			throw "Your platform isn't supported";
		}
	};
};
