/** @file */
#pragma once
#include <tuple>
#include <set>
#include <iterator>
#include "Surface.h"
#include "Matrix2D.h"
#include "ScreenObject.h"

/** @brief Object that contains collider
 *  @remark Use it if you want detection of collision of two objects
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class CollidableObject : public ScreenObject
{
	Matrix2D<bool> collider;

public:
	CollidableObject(std::string _id, int _xPos, int _yPos, const Surface &source) : ScreenObject(_id, _xPos, _yPos, source), collider(source.getHeight(), source.getWidth())
	{
		updateCollider();
	};

	void updateCollider(char alphaChar = ' ')
	{
		for (size_t row = 0; row < surf.getHeight(); row++)
		{
			for (size_t col = 0; col < surf.getWidth(); col++)
			{
				if (surf.getCharAt(row, col).getGlyph() == alphaChar)
				{
					collider.setAt(row, col, false);
				}
				else
				{
					collider.setAt(row, col, true);
				}
			}
		}
	};

	virtual void rotateRight()
	{
		collider.rotateRight();
		surf.rotateRight();
	};

	virtual void rotateLeft()
	{
		collider.rotateLeft();
		surf.rotateLeft();
	};

	bool isColliding(const CollidableObject &c)
	{
		// Adds colliding points to the set
		std::set<std::tuple<int, int>> thisCollidingPoints;
		for (int y = yPos; y < (long)surf.getHeight() + yPos; y++)
		{
			for (int x = xPos; x < (long)surf.getWidth() + xPos; x++)
			{
				if (collider.getAt(y - yPos, x - xPos) == true)
				{
					auto point = std::tuple<int, int>{y, x};
					thisCollidingPoints.insert(point);
				}
			}
		}
		for (int y = c.yPos; y < (long)c.getSurf().getHeight() + c.yPos; y++)
		{
			for (int x = c.xPos; x < (long)c.getSurf().getWidth() + c.xPos; x++)
			{
				if (c.collider.getAt(y - c.yPos, x - c.xPos) == true)
				{
					auto point = std::tuple<int, int>{y, x};
					if (thisCollidingPoints.find(point) != thisCollidingPoints.end())
					{
						return true;
					}
				}
			}
		}
		return false;
	};
};
