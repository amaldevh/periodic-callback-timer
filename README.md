# periodic-callback-timer

**High-precision C++ timer for executing callbacks at specified frequencies**

A lightweight, header-only C++ class that provides high-precision timing for periodic callback execution.

## Features

- **High Precision**: Uses `std::chrono::steady_clock` for accurate timing
- **Thread-Safe**: Uses atomic operations for safe start/stop
- **RAII Compliant**: Automatically stops and joins thread on destruction
- **Frequency-Based**: Specify timing in Hz (more intuitive than period)
- **Drift Compensation**: Adjusts for execution time and system latency
- **Exception Safe**: Catches exceptions in callbacks to prevent thread termination

## Requirements

- C++11 or later
- pthread library (on Linux/Unix systems)

## Usage

### Basic Example

```cpp
#include "HighPrecisionTimer.h"
#include <iostream>

int main() {
    // Create a timer that calls the lambda 10 times per second
    HighPrecisionTimer timer(10.0, []() {
        std::cout << "Tick!" << std::endl;
    });
    
    // Timer runs in background thread
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Timer automatically stops when destroyed
    return 0;
}
```

### Constructor Parameters

```cpp
HighPrecisionTimer(double frequency_hz, std::function<void()> callback)
```

- `frequency_hz`: The frequency in Hz at which to call the callback (must be > 0)
- `callback`: The function to execute at each timer tick

### Methods

- `stop()`: Manually stop the timer (also called automatically by destructor)
- `getFrequency()`: Returns the configured frequency in Hz
- `isRunning()`: Returns true if the timer is currently running

## Compilation

### Using Make
```bash
make
./timer_example
```

### Using CMake
```bash
mkdir build && cd build
cmake ..
make
./timer_example
```

### Manual Compilation
```bash
g++ -std=c++11 -pthread example.cpp -o timer_example
./timer_example
```

## Design Notes

### Precision Characteristics

The timer uses `sleep_until()` with compensation for drift, providing better accuracy than `sleep_for()`. Actual precision depends on:

- System scheduler granularity (typically 1-15ms on Linux)
- System load
- Callback execution time

For frequencies above ~100 Hz, expect slight variations due to OS scheduling. For very high frequencies (>1000 Hz), consider using real-time scheduling or dedicated hardware timers.

### Thread Safety

- The timer class itself is **not copyable or movable**
- The callback is executed on a dedicated thread
- Use appropriate synchronization (mutexes, atomics) if the callback accesses shared data

### Exception Handling

Exceptions thrown in the callback function are caught to prevent thread termination. In production code, you may want to modify the `timerLoop()` method to log or handle exceptions according to your needs.

## Example Output

```
Example 1: Counter at 10 Hz for 3 seconds
Tick 1
Tick 2
...
Tick 30

Example 2: High frequency timer at 1000 Hz for 2 seconds
Total callbacks: 2000

Example 3: Measuring actual callback timing at 100 Hz
Callback #50 at 500 ms
Callback #100 at 1000 ms
Callback #150 at 1500 ms
Callback #200 at 2000 ms
Expected callbacks: ~200, Actual: 200
```

## License

This code is provided as-is for educational and commercial use.

## Contributing

Feel free to submit issues or pull requests for improvements!
