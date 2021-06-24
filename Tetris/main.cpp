#include <iostream>
#include <stdexcept>
#include "Tetris.h"

void show_usage(std::string name)
{
	std::cout << "Usage: " << name << " <option(s)> SOURCES"
		<< "Options:" << std::endl
		<< "\t-h,--help\t\tShow this help message" << std::endl
		<< "\t-c,--color\t\tMakes the game colorful" << std::endl
		<< "\t-l,--level LEVEL\tSpecify the level at which the games starts" << std::endl
		<< "\t-s,--scale SCALE\tSpecify scaling of the game (how big it will appear)" << std::endl << std::endl
		<< "Controls in game:" << std::endl
		<< "arrow keys to move the block" << std::endl
		<< "R and E to rotate the block" << std::endl
		<< "Escape to exit the game" << std::endl
		<< "L-Shift + escape to restart the game"

		<< std::endl;
}


int main(int argc, char* argv[])
{
	bool color = false;
	unsigned short scale = 1;
	unsigned short level = 0;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage(argv[0]);
			return 0;
		}
		else if ((arg == "-c") || (arg == "--color")) {
			color = true;
		}
		else if ((arg == "-s") || (arg == "--scale")) {
			if (i + 1 < argc) {
				try {
					scale = std::stoi(argv[i + 1]);
				}
				catch (std::invalid_argument) {
					std::cerr << "Scale argument should be a number" << std::endl;
					return 1;
				}
				catch (std::out_of_range) {
					std::cerr << "Scale argument too big" << std::endl;
					return 1;
				}
			}
			else {
				std::cerr << "--scale option requires one argument." << std::endl;
				return 1;
			}
		}
		else if ((arg == "-l") || (arg == "--level")) {
			if (i + 1 < argc) {
				try {
					level = std::stoi(argv[i + 1]);
				}
				catch (std::invalid_argument) {
					std::cerr << "Level argument should be a number" << std::endl;
					return 1;
				}
				catch (std::out_of_range) {
					std::cerr << "Level argument too big" << std::endl;
					return 1;
				}
			}
			else {
				std::cerr << "--level option requires one argument." << std::endl;
				return 1;
			}
		}
	}

	bool play = true;
	while (play) {
		Tetris t(level, scale, color);
		play = t.start();
	};
	return 0;
}

