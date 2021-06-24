#pragma once
class Button {
	bool prevState, currentState;
public:
	Button() : prevState(false), currentState(false) {};

	void newState(bool pressed) {
		prevState = currentState;
		currentState = pressed;
	}
	bool isPressed() const {
		return currentState;
	}

	bool released() const {
		return prevState == true && currentState == false;
	}

	bool justPressed() const {
		return prevState == false && currentState == true;
	}

	bool isHeld() const {
		return prevState == true && currentState == true;
	}

	bool wasPressed() const {
		return prevState;
	}

};