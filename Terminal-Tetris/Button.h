/** @file */
#pragma once

/** @brief Class representing a button
 * @details Class represenging a button in a keyboard or gamepad
 *  @author Eryk Kściuczyk
 *   @date June 2021
*/
class Button
{
	bool prevState, currentState;

public:
	Button() : prevState(false), currentState(false){};

	/** Method to set new state of the button
     * @param pressed new stated of the button, should be true if the button is pressed
	 * @remark Should be called every time the input is handled
    */
	void newState(bool pressed)
	{
		prevState = currentState;
		currentState = pressed;
	}

	bool isPressed() const
	{
		return currentState;
	}

	bool justReleased() const
	{
		return prevState == true && currentState == false;
	}

	bool justPressed() const
	{
		return prevState == false && currentState == true;
	}

	bool isHeld() const
	{
		return prevState == true && currentState == true;
	}

	bool wasPressed() const
	{
		return prevState;
	}
};