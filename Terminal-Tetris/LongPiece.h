#pragma once
#include <string>
#include "CollidableObject.h"

#define LONGPIECE_TEXTURE "        @@@@    "

class LongPiece : public CollidableObject {
	// Long piece only has two possible rotation states straight or sideways
	bool isSideways;
public:
	LongPiece(std::string _id, int _xPos, int _yPos) :CollidableObject(_id, _xPos, _yPos, Surface(4, 4, LONGPIECE_TEXTURE)), isSideways(true) {};

	void rotateRight() {
		if (isSideways) {
			CollidableObject::rotateLeft();
		}
		else {
			CollidableObject::rotateRight();
		}
		isSideways = !isSideways;
	};

	void rotateLeft() {
		if (isSideways) {
			CollidableObject::rotateLeft();
		}
		else {
			CollidableObject::rotateRight();
		}
		isSideways = !isSideways;
	};
};
