#pragma once
#include <chrono>

class FPSCounter
{
private:
	std::chrono::time_point<std::chrono::system_clock> m_lastFrameTime;
	long long m_microsecondsPassed = 0;
	int m_framesPassed = 0;
public:
	FPSCounter();
	void update();
};

