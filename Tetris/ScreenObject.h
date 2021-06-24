#pragma once
#include <string>
#include "Surface.h"


class ScreenObject {
protected:
	Surface surf;
public:
	long xPos, yPos;
	std::string id;

	ScreenObject(std::string _id, long _xPos, long _yPos, const Surface& source) :xPos(_xPos), yPos(_yPos), surf(source), id(_id) {};

	ScreenObject(std::string _id, long _xPos, long _yPos) :xPos(_xPos), yPos(_yPos), surf(1, 1), id(_id) {};

	void updateSurf(const Surface& _surf) {
		surf = _surf;
	}

	const Surface& getSurf() const {
		return surf;
	}

	Surface& getSurf() {
		return surf;
	}
};
