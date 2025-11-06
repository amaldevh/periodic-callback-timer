#include "HighPrecisionTimer.h"
#include <iostream>
#include <chrono>
#include <iomanip>

int main() {
    std::cout << "High Precision Timer Example\n";
    std::cout << "=============================\n\n";
    
    // Example 1: Simple counter at 10 Hz
    {
        std::cout << "Example 1: Counter at 10 Hz for 3 seconds\n";
        int counter = 0;
        
        HighPrecisionTimer timer(10.0, [&counter]() {
            std::cout << "Tick " << ++counter << std::endl;
        });
        
        std::this_thread::sleep_for(std::chrono::seconds(3));
        // Timer automatically stops when it goes out of scope
    }
    
    std::cout << "\n";
    
    // Example 2: High frequency timer (1000 Hz)
    {
        std::cout << "Example 2: High frequency timer at 1000 Hz for 2 seconds\n";
        std::atomic<int> counter(0);
        
        HighPrecisionTimer timer(1000.0, [&counter]() {
            counter++;
        });
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Total callbacks: " << counter << std::endl;
    }
    
    std::cout << "\n";
    
    // Example 3: Timer with timestamp measurement
    {
        std::cout << "Example 3: Measuring actual callback timing at 100 Hz\n";
        auto start_time = std::chrono::steady_clock::now();
        int callback_count = 0;
        
        HighPrecisionTimer timer(100.0, [&start_time, &callback_count]() {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - start_time).count();
            
            callback_count++;
            if (callback_count % 50 == 0) {
                std::cout << "Callback #" << callback_count 
                          << " at " << elapsed << " ms" << std::endl;
            }
        });
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Expected callbacks: ~200, Actual: " << callback_count << std::endl;
    }
    
    std::cout << "\n";
    
    // Example 4: Manual stop
    {
        std::cout << "Example 4: Manually stopping the timer\n";
        int counter = 0;
        
        HighPrecisionTimer timer(5.0, [&counter]() {
            std::cout << "Callback " << ++counter << std::endl;
        });
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Stopping timer manually..." << std::endl;
        timer.stop();
        
        std::cout << "Timer stopped. Waiting 2 more seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "No more callbacks should appear." << std::endl;
    }
    
    std::cout << "\nAll examples completed!\n";
    return 0;
}
