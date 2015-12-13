#include "FPSCounter.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleepMilliseconds(int milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
#else
	usleep(milliseconds * 1000);
#endif
}

FPSCounter::FPSCounter()
{
	m_lastFrameTime = std::chrono::system_clock::now();
}

void FPSCounter::update()
{
	auto curTime = std::chrono::system_clock::now();
	auto duration = curTime - m_lastFrameTime;
	int frameTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	m_microsecondsPassed += frameTimeMicroseconds;
	m_lastFrameTime = curTime;
	m_framesPassed++;

	if (m_microsecondsPassed > 1000000)
	{
		std::cout << "FPS: " << m_framesPassed << std::endl;
		m_microsecondsPassed -= 1000000;
		m_framesPassed = 0;
	}
}
