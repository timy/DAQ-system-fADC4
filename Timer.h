#pragma once

#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

class Timer {
public:
    Timer() : isStopped(true), bStop(false) {}

    Timer(const Timer& timer) {
        isStopped = timer.isStopped.load();
        bStop = timer.bStop.load();
    }

    virtual ~Timer() {
        stop();
    }

    void start(int interval, std::function<void()> func) {
        // if started, don't start again
        if (!isStopped)
            return;

        // if not started, start the timer, and set 
        // the current status isStopped = false
        isStopped = false;

        std::thread([this, interval, func]() {
            while (!bStop) { // if no bStop signal is emitted, continue the loop
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                func();
            }

            {
                // when receive bStop signal and the func run naturally ends,
                // set the current status isStopped = true, 
                // and notify the control thread to continue
                std::lock_guard<std::mutex> lock(_mutex);
                isStopped = true;
                cond.notify_one();
            }

            }).detach();
    }

    void startOnce(int delay, std::function<void()> func) {
    }

    void stop() {
        // if not running, cannot stop
        if (isStopped)
            return;

        // if bStop signal has already emitted, don't stop again
        if (bStop)
            return;

        // emit bStop signal
        bStop = true; // stop the timer while loop

        {
            // wait until the working thread naturally ends and 
            // returns the notification that the work is done

            std::unique_lock<std::mutex> lock(_mutex);
            cond.wait(lock, [this] {
                return isStopped == true;
                });

            // reset the timer
            if (isStopped)
                bStop = false;
        }
    }

private:
    std::atomic<bool> isStopped; // status if the timer is running
    std::atomic<bool> bStop; // control signal/flag to stop the timer
    std::mutex _mutex;
    std::condition_variable cond;
};