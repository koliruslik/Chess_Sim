#include "Timer.h"
#include <ctime>
#include <cstdio>

Timer::Timer()
    : remainingTime(std::chrono::seconds(0)), originalDuration(std::chrono::seconds(0)), running(false)
{
}

Timer::Timer(std::chrono::duration<double> duration)
    : remainingTime(duration), originalDuration(duration), running(false)
{
}

std::string Timer::GetCurrentTime() const
{
    time_t now = time(0);
    struct tm* timeInfo = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo);
    return std::string(buffer);
}

std::string Timer::GetRemainingTime()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(remainingTime).count();
    if (secondsLeft < 0) secondsLeft = 0;

    int minutes = secondsLeft / 60;
    int seconds = secondsLeft % 60;

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    return std::string(buffer);
}

int Timer::GetRemainingSeconds()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto secondsLeft = std::chrono::duration_cast<std::chrono::seconds>(remainingTime).count();
    return (secondsLeft < 0) ? 0 : static_cast<int>(secondsLeft);
}

void Timer::Start()
{
    if (running) return;
    running = true;
    lastUpdate = std::chrono::steady_clock::now();
    timerThread = std::thread(&Timer::processTimer, this);
    timerThread.detach();
}

void Timer::Stop()
{
    running = false;
}

void Timer::Reset()
{
    std::lock_guard<std::mutex> lock(mutex);
    running = false;
    remainingTime = originalDuration;
    lastUpdate = std::chrono::steady_clock::now();
}

void Timer::processTimer()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto now = std::chrono::steady_clock::now();
        std::lock_guard<std::mutex> lock(mutex);

        if (!running) break;

        auto delta = now - lastUpdate;
        lastUpdate = now;

        remainingTime -= std::chrono::duration_cast<std::chrono::duration<double>>(delta);
        if (remainingTime.count() <= 0)
        {
            remainingTime = std::chrono::duration<double>(0);
            running = false;
        }
    }
}

void Timer::setTime(int duration)
{
	std::lock_guard<std::mutex> lock(mutex);
	originalDuration = std::chrono::seconds(duration);
	remainingTime = originalDuration;
	lastUpdate = std::chrono::steady_clock::now();
}
