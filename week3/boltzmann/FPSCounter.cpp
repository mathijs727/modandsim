#include "FPSCounter.h"
#include <iostream>

FPSCounter::FPSCounter()
{
	m_lastFrameTime = std::chrono::system_clock::now();
}

void FPSCounter::update()
{
	auto curTime = std::chrono::system_clock::now();
	auto duration = curTime - m_lastFrameTime;
	m_microsecondsPassed += std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	m_lastFrameTime = curTime;
	m_framesPassed++;

	if (m_microsecondsPassed > 1000000)
	{
		std::cout << "FPS: " << m_framesPassed << std::endl;
		m_microsecondsPassed -= 1000000;
		m_framesPassed = 0;
	}
}
