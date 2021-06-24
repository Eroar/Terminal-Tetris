/** @file */
#pragma once
#include <vector>
#include <stdexcept>
#include "ScreenFactory.h"
#include "KeyboardFactory.h"
#include "VirtualKeys.h"
#include "ScreenObject.h"
#include "CollidableObject.h"
#include "TextObject.h"
#include "Clock.h"
#include "PlatformIdentifier.h"

/** @brief Abstract object that represents a game
 *  @details Handles timing between frames, initialization of inputs and screen
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Game
{
	const short framerate;
	bool debug;
	std::wstring title;
	Screen *screen;
	Keyboard *keyboard;
	std::vector<ScreenObject *> screenObjects;
	std::vector<TextObject *> textObjects;

protected:
	unsigned short scale;
	unsigned long frameCount;
	bool updateDisplay;
	const Button getButton(int buttonID) const
	{
		return keyboard->getButton(buttonID);
	}

	void addObject(ScreenObject *obj)
	{
		screenObjects.push_back(obj);
	}

	void addObject(TextObject *obj)
	{
		textObjects.push_back(obj);
	}

	void removeObject(ScreenObject *obj)
	{
		auto it = find(screenObjects.begin(), screenObjects.end(), obj);
		screenObjects.erase(it);
	}

	void removeObject(TextObject *obj)
	{
		auto it = find(textObjects.begin(), textObjects.end(), obj);
		textObjects.erase(it);
	}

	ScreenObject *getObjectWithID(std::string s, size_t startPos = 0) const
	{
		ScreenObject *obj = getObjectWithIDContaining(s, startPos);
		if (obj->id == s)
		{
			return obj;
		}
		else
		{
			throw std::runtime_error("Object with given ID couldn\'t be found");
		}
	}

	ScreenObject *getObjectWithIDContaining(std::string s, size_t startPos = 0) const
	{
		for (size_t i = startPos; i < screenObjects.size(); i++)
		{
			ScreenObject *obj = screenObjects.at(i);
			if (obj->id.find(s) != std::string::npos)
			{
				return obj;
			}
		}
		for (size_t i = startPos; i < textObjects.size(); i++)
		{
			ScreenObject *obj = textObjects.at(i);
			if (obj->id.find(s) != std::string::npos)
			{
				return obj;
			}
		}

		throw std::runtime_error("Object with given ID couldn\'t be found");
	}

	void displayObjects()
	{
		// Displays the changes to the console screen
		screen->clear();
		for (ScreenObject *obj : screenObjects)
		{
			screen->blit(*obj, scale);
		}

		for (TextObject *obj : textObjects)
		{
			screen->blit(*obj);
		}
		screen->refresh();
	}

	// user has to override these
	virtual void onStart() = 0;

	virtual void onUpdate() = 0;

	virtual void onClose() = 0;

public:
	Game(int fps, int _ScreenWidth, const int _ScreenHeight, std::wstring _title, unsigned short _scale = 1, bool _debug = false)
		: scale(_scale),
		  title(_title),
		  framerate(fps),
		  debug(_debug),
		  screen(ScreenFactory::makeScreen(_ScreenWidth, _ScreenHeight)),
		  keyboard(KeyboardFactory::makeKeyboard()),
		  frameCount(0),
		  updateDisplay(true)
	{
		screen->setTitle(_title);
	};

	bool start()
	{
		// returns whether the game should be restarted
		bool restart = false;
		onStart();
		// GAME LOOP

		auto t = std::chrono::system_clock::now();
		auto t_prev = t;

		Clock c(framerate);
		std::chrono::duration<float> elapsedTime = std::chrono::system_clock::now() - t;
		float fElapsedTime = 0.0f;
		while (true)
		{
			if (updateDisplay)
			{
				displayObjects();
			}
			updateDisplay = false;
			c.tick();

			if (debug)
			{
				t_prev = t;
				t = std::chrono::system_clock::now();
				wchar_t s[256];
				std::chrono::duration<float> sinceLastUpdate = t - t_prev;
				swprintf(s, 256, L"%s FPS: %3.0f frame time: %3.2fms Correction: %i", title.c_str(), 1.0f / sinceLastUpdate.count(), sinceLastUpdate.count() * 1000.0f, c.getCorrection());
				screen->setTitle(s);
			}
			keyboard->checkKeys();
			if (keyboard->getButton(VK_ESCAPE).isPressed() && keyboard->getButton(VK_LSHIFT).isPressed())
			{
				restart = true;
				break;
			}
			else if (keyboard->getButton(VK_ESCAPE).isPressed())
			{
				break;
			}
			onUpdate();
			frameCount++;
		}
		screen->clear();
		onClose();
		return restart;
	}

	~Game() noexcept
	{
		if (PLATFORM == WINDOWS_PLATFORM)
		{
			WindowsCMDScreen *s = (WindowsCMDScreen *)screen;
			delete s;
		}
		delete keyboard;
	}
};