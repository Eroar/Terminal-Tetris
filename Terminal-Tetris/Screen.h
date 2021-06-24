#pragma once
#include <string>
#include <stdexcept>
#include "Surface.h"
#include "ScreenObject.h"

class Screen :public Surface
{
protected:
	std::wstring title;
	virtual void updateTitle() {};
public:
	Screen(const int _width, const int _height) :Surface(_height, _width) {
		clear();
	};

	using Surface::blit;

	void setTitle(std::wstring newTitle) {
		if (newTitle.length() < 256) {
			title = newTitle;
			updateTitle();
		}
		else {
			throw std::invalid_argument("New title too long max length is 256 characters");
		}
	};

	virtual void blit(const Surface& source, const long _col, const long _row, char alpha_char = ' ') = 0;

	virtual void blit(const ScreenObject& source, unsigned short scale = 1, char alpha_char = ' ') = 0;

	virtual void refresh() {};

	virtual void clear() {
		content.reset();
	};
};

