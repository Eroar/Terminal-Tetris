#pragma once
#include <Windows.h>
#include <cwchar>
#include "Screen.h"
#include "Matrix2D.h"

class WindowsCMDScreen : public Screen {
	SMALL_RECT m_rectWindow;
	HANDLE hConsole;
	Matrix2D<CHAR_INFO> bufferContent;

protected:
	void updateTitle() {
		wchar_t s[256];
		swprintf_s(s, 256, L"%s", title.c_str());
		SetConsoleTitle(s);
	};
public:
	WindowsCMDScreen(const size_t _width, const size_t _height) :Screen(_width, _height), bufferContent(Matrix2D<CHAR_INFO>(_height, _width)) {
		// SETS UP WINDOWS CONSOLE
		clear();
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		m_rectWindow = { 0,0,1,1 };
		SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);
		COORD coord = { (short)getWidth(), (short)getHeight() };
		SetConsoleScreenBufferSize(hConsole, coord);
		SetConsoleActiveScreenBuffer(hConsole);

		// Set the font size now that the screen buffer has been assigned to the console
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 2;
		cfi.dwFontSize.X = 8;
		cfi.dwFontSize.Y = 8;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		wcscpy_s(cfi.FaceName, L"Terminal");
		SetCurrentConsoleFontEx(hConsole, false, &cfi);

		m_rectWindow = { 0, 0, short(getWidth() - 1), short(getHeight() - 1) };
		SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);
	};

	void blit(const Surface& source, const long _col, const long _row, char alpha_char = ' ') {
		for (long row = 0; row < (long)source.getHeight(); row++) {
			if ((row + _row > -1) && (row + _row < (long)getHeight())) {
				for (long col = 0; col < (long)source.getWidth(); col++) {
					if (_col + col > -1 && _col + col < (long)getWidth()) {
						CHARACTER_TYPE c = source.getCharAt(row, col);
						if (c.getGlyph() != alpha_char) {
							bufferContent(row + _row, col + _col).Char.AsciiChar = c.getGlyph();
							bufferContent(row + _row, col + _col).Attributes = c.getAttributes();
						}
					}
				}
			}
		}
	}

	void blit(const ScreenObject& source, unsigned short scale = 1, char alpha_char = ' ') {
		Surface* scaled = source.getSurf().scaled(scale);
		blit(*scaled, source.xPos * scale, source.yPos * scale, alpha_char);
		delete scaled;
	};

	void refresh() {
		WriteConsoleOutput(hConsole, bufferContent.as1DArr(), { static_cast<short>(getWidth()),  static_cast<short>(getHeight()) }, { 0,0 }, &m_rectWindow);
	};

	virtual void clear() {
		memset(bufferContent.as1DArr(), 0, sizeof(CHAR_INFO) * bufferContent.colCount() * bufferContent.rowCount());
	};
};
