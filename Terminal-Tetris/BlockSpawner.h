#pragma once
#include <cstdlib>
#include <ctime>
#include <string>
#include "CollidableObject.h"
#include "LongPiece.h"
#include "Surface.h"
#include "PlatformIdentifier.h"
#include "WindowsCharacter.h"

#define SQUARE_PIECE "     %%  %%     "
#define L_PIECE "   ####  "
#define REVERSED_L_PIECE "   $$$  $"
#define T_PIECE "   aaa a "
#define S_PIECE "    OOOO "
#define Z_PIECE "   !!  !!"
//LONG_PIECE is defined in LongPiece.h


#if PLATFORM == WINDOWS_PLATFORM
void changeSurfAttributes(Surface& s, short attributes) {
	for (size_t row = 0; row < s.getHeight(); row++) {
		for (size_t col = 0; col < s.getWidth(); col++) {
			s.getCharAt(row, col).setAttributes(attributes);
		}
	}
}
#endif

class BlockSpawner {
	static unsigned long blockCount;
	unsigned short lastN;
public:
	BlockSpawner() : lastN(99) {
		std::srand((unsigned int)std::time(0)); //use current time as seed for random generator
		int random_variable = std::rand();
	};

	CollidableObject* getBlock(bool color) {
		unsigned short n;
		do {
			n = rand() % 7;
		} while (n == lastN);
		lastN = n;
		std::string blockID = "block_" + std::to_string(blockCount);
		CollidableObject* block;
		short attributes = 0x000F;
		if (n == 0) {
			block = new LongPiece(blockID, 0, 0);
			if (PLATFORM == WINDOWS_PLATFORM) {
				attributes = FG_BLUE;
			}
		}
		else {
			Surface s(0, 0);

			switch (n) {
			case 1:
				s = std::move(Surface(4, 4, SQUARE_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_YELLOW;
				}
				break;
			case 2:
				s = std::move(Surface(3, 3, L_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_WHITE;
				}
				break;
			case 3:
				s = std::move(Surface(3, 3, REVERSED_L_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_RED;
				}
				break;
			case 4:
				s = std::move(Surface(3, 3, T_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_GREEN;
				}
				break;
			case 5:
				s = std::move(Surface(3, 3, S_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_MAGENTA;
				}
				break;
			case 6:
				s = std::move(Surface(3, 3, Z_PIECE));
				if (PLATFORM == WINDOWS_PLATFORM) {
					attributes = FG_DARK_CYAN;
				}
				break;
			}
			block = new CollidableObject(blockID, 0, 0, s);
			block->updateCollider();
		}
		if (PLATFORM == WINDOWS_PLATFORM && color) {
			changeSurfAttributes(block->getSurf(), attributes);
		}

		return block;
	};
};