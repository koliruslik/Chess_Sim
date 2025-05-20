#pragma once

#ifndef TIMER_H
#define TIMER_H

#include "time.h"
#include <string>
#include <ctime>
#include <chrono>
#include <thread>

class Timer
{
public:
	std::string GetCurrentTime();
};



#endif // TIMER_H