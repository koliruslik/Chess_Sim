
#include "Timer.h"


std::string Timer::GetCurrentTime()
{
	time_t now = time(0);

	struct tm* timeInfo = localtime(&now);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo);
	return std::string(buffer);
}