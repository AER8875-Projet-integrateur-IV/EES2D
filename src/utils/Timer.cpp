#include "Timer.h"
#include <iostream>
using ees2d::utils::Timer;

Timer::Timer(std::string tag)
    : m_tag(tag) {

	m_startTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {

	stop();
}


void Timer::stop() {

	auto endTime = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	std::cout << "[ " << m_tag << " ] "
	          << "Profiling results : " << duration << "us (" << ms << "ms)" << std::endl;
}