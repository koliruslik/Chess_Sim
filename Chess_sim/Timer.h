#pragma once

#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

class Timer {
public:
    Timer();
    Timer(std::chrono::duration<double> duration);

    std::string GetCurrentTime() const;
    std::string GetRemainingTime();
    int GetRemainingSeconds();
    void setTime(int duration);
    void Start();
    void Stop();
    void Reset();

private:
    std::chrono::duration<double> remainingTime;
    std::chrono::duration<double> originalDuration;
    std::chrono::steady_clock::time_point lastUpdate;
    std::atomic<bool> running;
    std::thread timerThread;
    std::mutex mutex;

    void processTimer();
};

#endif // TIMER_H
