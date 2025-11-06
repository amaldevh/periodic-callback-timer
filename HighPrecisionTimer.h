#ifndef HIGH_PRECISION_TIMER_H
#define HIGH_PRECISION_TIMER_H

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <stdexcept>

class HighPrecisionTimer {
public:
    /**
     * Constructor
     * @param frequency_hz The frequency in Hz at which to call the callback
     * @param callback The function to call at the specified frequency
     * @throws std::invalid_argument if frequency is <= 0
     */
    HighPrecisionTimer(double frequency_hz, std::function<void()> callback)
        : frequency_hz_(frequency_hz)
        , callback_(std::move(callback))
        , running_(false)
    {
        if (frequency_hz_ <= 0.0) {
            throw std::invalid_argument("Frequency must be positive");
        }
        
        if (!callback_) {
            throw std::invalid_argument("Callback function cannot be null");
        }
        
        // Calculate the period in nanoseconds
        period_ns_ = static_cast<long long>(1e9 / frequency_hz_);
        
        // Start the timer thread
        running_ = true;
        timer_thread_ = std::thread(&HighPrecisionTimer::timerLoop, this);
    }
    
    /**
     * Destructor - stops the timer and joins the thread
     */
    ~HighPrecisionTimer() {
        stop();
    }
    
    // Delete copy constructor and assignment operator
    HighPrecisionTimer(const HighPrecisionTimer&) = delete;
    HighPrecisionTimer& operator=(const HighPrecisionTimer&) = delete;
    
    // Delete move constructor and assignment operator (optional, but recommended for this use case)
    HighPrecisionTimer(HighPrecisionTimer&&) = delete;
    HighPrecisionTimer& operator=(HighPrecisionTimer&&) = delete;
    
    /**
     * Stop the timer (called automatically by destructor)
     */
    void stop() {
        if (running_.exchange(false)) {
            if (timer_thread_.joinable()) {
                timer_thread_.join();
            }
        }
    }
    
    /**
     * Get the configured frequency
     */
    double getFrequency() const {
        return frequency_hz_;
    }
    
    /**
     * Check if the timer is currently running
     */
    bool isRunning() const {
        return running_;
    }

private:
    void timerLoop() {
        using namespace std::chrono;
        
        auto next_wakeup = steady_clock::now();
        const auto period = nanoseconds(period_ns_);
        
        while (running_) {
            // Calculate next wakeup time
            next_wakeup += period;
            
            // Call the callback function
            try {
                callback_();
            } catch (...) {
                // Catch any exceptions from the callback to prevent thread termination
                // In production code, you might want to log this or handle it differently
            }
            
            // Sleep until the next scheduled time
            // Using sleep_until provides better precision than sleep_for
            std::this_thread::sleep_until(next_wakeup);
            
            // If we've fallen behind schedule, reset to current time
            auto now = steady_clock::now();
            if (now > next_wakeup + period) {
                next_wakeup = now;
            }
        }
    }
    
    double frequency_hz_;
    long long period_ns_;
    std::function<void()> callback_;
    std::atomic<bool> running_;
    std::thread timer_thread_;
};

#endif // HIGH_PRECISION_TIMER_H
