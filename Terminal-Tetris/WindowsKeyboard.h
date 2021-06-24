/** @file */
#pragma once
#include <Windows.h>
#include "Keyboard.h"

/** @brief Class that is used to represent a windows keyboard
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class WindowsKeyboard : public Keyboard
{
	HANDLE hConsoleIn;

public:
	WindowsKeyboard()
	{
		hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	}
	void checkKeys()
	{
		for (short i = 0; i < 256; i++)
		{
			short s = GetAsyncKeyState(i);
			buttons[i].newState(s & 0x8000);
		}
	}
};