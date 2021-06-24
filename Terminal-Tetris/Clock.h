#pragma once
#include <chrono>
#include <thread>

class Clock {
	const long tickTime;
	std::chrono::high_resolution_clock::time_point lastTick;
public:
	long correction;
	Clock(int _tickRate) :lastTick(std::chrono::high_resolution_clock::now()), tickTime(1000000 / _tickRate), correction(0) {};

	void tick() {
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = now - lastTick;
		lastTick = now;
		const double usElapsedTime = elapsedTime.count() * 1000000;
		const double usDiff = tickTime - usElapsedTime;
		if (usDiff > 1000) {
			correction += 150;
		}
		else if (usDiff < -250) {
			correction -= 150;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tickTime + correction));

	}
};