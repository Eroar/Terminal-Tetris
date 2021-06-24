/** @file */
#pragma once
#include <chrono>
#include <thread>

/** @brief Class used for sleeping between frames
 *  @remark The timing between ticks is not precise
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
class Clock
{
	const long tickTime;
	std::chrono::high_resolution_clock::time_point lastTick;
	long correction;

public:
	Clock(int _tickRate) : lastTick(std::chrono::high_resolution_clock::now()), tickTime(1000000 / _tickRate), correction(0){};

	long getCorrection() const
	{
		return correction;
	}

	void tick()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = now - lastTick;
		lastTick = now;
		const double usElapsedTime = elapsedTime.count() * 1000000;
		const double usDiff = tickTime - usElapsedTime;
		if (usDiff > 1000)
		{
			correction += 150;
		}
		else if (usDiff < -250)
		{
			correction -= 150;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(tickTime + correction));
	}
};