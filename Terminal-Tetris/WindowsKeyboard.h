#pragma once
#include <Windows.h>
#include "Keyboard.h"

class WindowsKeyboard :public Keyboard {
	HANDLE hConsoleIn;
public:
	WindowsKeyboard() {
		hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	}
	void checkKeys() {
		for (short i = 0; i < 256; i++) {
			short s = GetAsyncKeyState(i);
			buttons[i].newState(s & 0x8000);
		}
	}
};