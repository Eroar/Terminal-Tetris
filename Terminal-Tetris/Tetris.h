/** @file */
#pragma once
#include <algorithm>
#include <vector>
#include "Game.h"
#include "VirtualKeys.h"
#include "ScreenObject.h"
#include "TextObject.h"
#include "CollidableObject.h"
#include "BlockSpawner.h"

#define SCALE 4
#define BOARD_XPOS 0
#define BOARD_YPOS 0
#define NEXTBLOCK_XPOS 14
#define NEXTBLOCK_YPOS 6
#define SCORE_XPOS 14
#define SCORE_YPOS 1
#define LEVEL_XPOS 14
#define LEVEL_YPOS 13

unsigned long BlockSpawner::blockCount = 0;

inline std::string getBordersTexture()
{
	std::string c(21 * 12, '=');
	for (short row = 0; row < 20; row++)
	{
		for (short col = 1; col < 11; col++)
		{
			c.at(row * 12 + col) = ' ';
		}
	}
	return c;
};

/** @brief Class that represents a Tetris game
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Tetris : public Game
{
	bool color;
	bool gameOver;
	const unsigned short startLevel;
	unsigned short level;
	long score;
	unsigned long linesCleared;
	unsigned short leftHeldFor;
	unsigned short rightHeldFor;
	unsigned short downHeldFor;
	unsigned int lineClearAnimationState;
	std::vector<unsigned short> fullLines;
	std::vector<TextObject *> managedTextObjects;
	TextObject *scoreCounter;
	TextObject *levelCounter;
	ScreenObject *upperBorder;
	CollidableObject *borders;
	CollidableObject *blocksBlob;
	BlockSpawner spawner;
	CollidableObject *activeBlock;
	CollidableObject *nextBlock;

	void onStart()
	{
		addObject(upperBorder);
		addObject(borders);
		addObject(blocksBlob);
		setActiveBlockToStartingPos();
		addObject(activeBlock);

		// adds text objects for counting frames
		TextObject *t = new TextObject("ScoreText", SCORE_XPOS * scale, SCORE_YPOS * scale, "Score:", 'L');
		managedTextObjects.push_back(t);
		addObject(t);
		addObject(scoreCounter);

		t = new TextObject("NextText", NEXTBLOCK_XPOS * scale, NEXTBLOCK_YPOS * scale, "Next:", 'L');
		managedTextObjects.push_back(t);
		addObject(t);
		nextBlock->xPos = NEXTBLOCK_XPOS + 1;
		nextBlock->yPos = NEXTBLOCK_YPOS + 1;
		addObject(nextBlock);

		t = new TextObject("LevelText", LEVEL_XPOS * scale, LEVEL_YPOS * scale, "Level:", 'L');
		managedTextObjects.push_back(t);
		addObject(t);
		addObject(levelCounter);
	};

	void onUpdate()
	{
		if (!gameOver)
		{
			if (lineClearAnimationState > 0)
			{
				updateDisplay = true;
				performAnimationStep();
				lineClearAnimationState--;
				if (lineClearAnimationState == 0)
				{
					givePoints();
					linesCleared += fullLines.size();
					if (linesCleared % 10 == 0)
					{
						level++;
					}
					moveLinesDown();
					fullLines.clear();
					blocksBlob->updateCollider();
				}
			}
			else
			{
				updateDisplay = handleInputAndGravity();

				markFullLines();

				if (fullLines.size() > 0)
				{
					lineClearAnimationState = 30;
				}
			}
			scoreCounter->setText(std::to_string(score), 'R');
			levelCounter->setText(std::to_string(level), 'R');
		}
	};

	void onClose() {}

	bool handleInputAndGravity()
	{
		if (getButton(VK_DOWN).isHeld())
		{
			downHeldFor += 1;
		}
		else if (!getButton(VK_DOWN).isPressed())
		{
			downHeldFor = 0;
		}

		if (getButton(VK_LEFT).isHeld())
		{
			leftHeldFor += 1;
		}
		else if (!getButton(VK_LEFT).isPressed())
		{
			leftHeldFor = 0;
		}
		if (getButton(VK_RIGHT).isHeld())
		{
			rightHeldFor += 1;
		}
		else if (!getButton(VK_RIGHT).isPressed())
		{
			rightHeldFor = 0;
		}

		bool movedDown = true;
		if (getButton(VK_DOWN).justPressed() || downHeldFor > 45)
		{
			downHeldFor = 44;
			moveActiveBlockDown();
		}
		else if (shouldActiveBlockFall() && downHeldFor < 35)
		{
			moveActiveBlockDown();
		}
		else
		{
			movedDown = false;
		}

		bool movedSideways = true;
		if (getButton(VK_LEFT).justPressed() || leftHeldFor > 45)
		{
			leftHeldFor = 35;
			moveActiveBlockLeft();
		}
		else if (getButton(VK_RIGHT).justPressed() || rightHeldFor > 45)
		{
			rightHeldFor = 35;
			moveActiveBlockRight();
		}
		else
		{
			movedSideways = false;
		}

		bool rotated = true;
		if (getButton(VK_Z).justPressed())
		{
			rotateActiveBlockLeft();
		}
		else if (getButton(VK_X).justPressed())
		{
			rotateActiveBlockRight();
		}
		else
		{
			rotated = false;
		}

		return movedDown || movedSideways || rotated;
	};

	void moveLinesDown()
	{
		// Move everything above each line down, starting with the highest lines
		for (unsigned short line : fullLines)
		{
			for (short row = line - 1; row > -1; row--)
			{
				Matrix2D<CHARACTER_TYPE> *rowM = blocksBlob->getSurf().getRow(row);
				blocksBlob->getSurf().setRow(row + 1, rowM->as1DArr());
				delete rowM;
			}
		}
	};

	void givePoints()
	{
		switch (fullLines.size())
		{
		case 1:
			score += 40 * (level + 1);
			break;
		case 2:
			score += 100 * (level + 1);
			break;
		case 3:
			score += 300 * (level + 1);
			break;
		case 4:
			score += 1200 * (level + 1);
			break;
		}
	};

	void performAnimationStep()
	{
		if (lineClearAnimationState % 6 == 0)
		{
			for (unsigned short line : fullLines)
			{
				blocksBlob->getSurf().setCharAt(line, 4 - (int)((30 - lineClearAnimationState) / 6), ' ');
				blocksBlob->getSurf().setCharAt(line, 5 + (int)((30 - lineClearAnimationState) / 6), ' ');
			}
		}
	};

	void markFullLines()
	{
		fullLines.clear();
		for (unsigned short row = 0; row < blocksBlob->getSurf().getHeight(); row++)
		{
			bool isRowFull = false;
			for (unsigned short col = 0; col < blocksBlob->getSurf().getWidth(); col++)
			{
				if (blocksBlob->getSurf().getCharAt(row, col).getGlyph() == ' ')
				{
					isRowFull = false;
					break;
				}
				else
				{
					isRowFull = true;
				}
			}
			if (isRowFull)
			{
				fullLines.push_back(row);
			}
		}
	};

	bool shouldActiveBlockFall() const
	{
		unsigned short framesPerGridcell = 0;
		if (level < 9)
		{
			framesPerGridcell = max(6, 48 - 5 * level);
		}
		else if (level < 19)
		{
			framesPerGridcell = 6 - ((level + 2) / 3 - 3);
		}
		else
		{
			framesPerGridcell = max(1, 2 - level / 29);
		}
		return frameCount % framesPerGridcell == 0;
	};

	bool isActiveBlockColiding() const
	{
		return (activeBlock->isColliding(*blocksBlob)) || (activeBlock->isColliding(*borders));
	};

	void rotateActiveBlockLeft()
	{
		activeBlock->rotateLeft();
		if (isActiveBlockColiding())
		{
			activeBlock->rotateRight();
		}
	};

	void rotateActiveBlockRight()
	{
		activeBlock->rotateRight();
		if (isActiveBlockColiding())
		{
			activeBlock->rotateLeft();
		}
	};

	void moveActiveBlockLeft()
	{
		activeBlock->xPos -= 1;

		if (isActiveBlockColiding())
		{
			activeBlock->xPos += 1;
		}
	};

	void moveActiveBlockRight()
	{
		activeBlock->xPos += 1;

		if (isActiveBlockColiding())
		{
			activeBlock->xPos -= 1;
		}
	};

	void moveActiveBlockDown()
	{
		activeBlock->yPos += 1;
		if (isActiveBlockColiding())
		{
			onBlockLanded();
		}
	};

	void onBlockLanded()
	{
		activeBlock->yPos -= 1;
		//Blits active block into the blocksBlob
		blocksBlob->getSurf().blit(activeBlock->getSurf(), activeBlock->xPos - blocksBlob->xPos, activeBlock->yPos - blocksBlob->yPos);
		blocksBlob->updateCollider();
		removeObject(activeBlock);
		delete activeBlock;
		activeBlock = nextBlock;
		setActiveBlockToStartingPos();
		nextBlock = spawner.getBlock(color);
		nextBlock->xPos = NEXTBLOCK_XPOS + 1;
		nextBlock->yPos = NEXTBLOCK_YPOS + 1;
		addObject(nextBlock);
		if (isActiveBlockColiding())
		{
			gameOver = true;
		}
	}

	void setActiveBlockToStartingPos()
	{
		size_t blockWidth = activeBlock->getSurf().getWidth();
		activeBlock->xPos = BOARD_XPOS + (blockWidth == 4 ? 4 : 5);
		activeBlock->yPos = BOARD_YPOS;
	};

public:
	Tetris(short _level, unsigned short scale, bool _color)
		: Game(60, 22 * scale, 22 * scale, L"Tetris by Eroar", scale, false),
		  color(_color),
		  gameOver(false),
		  startLevel(_level),
		  level(_level),
		  score(0),
		  linesCleared(0),
		  leftHeldFor(0),
		  rightHeldFor(0),
		  downHeldFor(0),
		  lineClearAnimationState(0),
		  scoreCounter(new TextObject("scoreCounter", SCORE_XPOS * scale, SCORE_YPOS * scale + 2, "0", 8, 'R')),
		  levelCounter(new TextObject("levelCounter", LEVEL_XPOS * scale + 4, LEVEL_YPOS * scale + 2, "0", 2, 'R')),
		  upperBorder(new ScreenObject("upperBorder", BOARD_XPOS, BOARD_YPOS, Surface(1, 12, "============"))),
		  borders(new CollidableObject("borders", BOARD_XPOS, BOARD_YPOS + 1, Surface(21, 12, getBordersTexture()))),
		  blocksBlob(new CollidableObject("blocksBlob", BOARD_XPOS + 1, BOARD_YPOS + 1, Surface(20, 10))),
		  activeBlock(spawner.getBlock(color)),
		  nextBlock(spawner.getBlock(color)){};

	~Tetris() noexcept
	{

		delete scoreCounter;
		delete levelCounter;
		delete upperBorder;
		delete borders;
		delete blocksBlob;
		delete activeBlock;
		delete nextBlock;
		for (TextObject *obj : managedTextObjects)
		{
			delete obj;
		}
	}
};