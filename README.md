# WeatherStation_EmergencyData

It's easy enough to get weather data online, and to know when it's coming straight to you. But what about when it's on top of you? When is it safe to come out? This project aims to collect data useful in identifying when weather has normalized enough to leave shelter and return home.

## Overview

This weather station monitors atmospheric pressure using a **20-sample moving average** to smooth out spikes in the data. The system establishes a baseline for typical pressure in the installation location, then applies rules to detect severe weather based on significant pressure shifts.

## Features

- **Moving Average Filter**: 20-sample rolling window that smooths pressure data by continuously replacing the oldest sample with the newest
- **Baseline Establishment**: Automatically establishes typical operational pressure for the installation location
- **Severe Weather Detection**: Alerts when pressure changes exceed configurable thresholds
- **Real-time Monitoring**: Continuous pressure tracking with customizable sampling intervals
- **Room-relative Readings**: Measures pressure relative to the installation environment

## How It Works

### Moving Average (Marching Average)

The system uses a circular buffer to maintain the last 20 pressure samples. As each new sample arrives:
1. The oldest sample is removed from the buffer
2. The new sample is added
3. The average is recalculated

This "marching" or "rolling" average greatly smooths spikes in the data while remaining responsive to genuine trends.

### Baseline Pressure

Once 20 samples have been collected, the system establishes a baseline pressure representing typical conditions. This baseline slowly adapts over time to account for seasonal variations while remaining sensitive to rapid changes that indicate severe weather.

### Severe Weather Rules

Significant pressure changes warrant an on-screen notice:
- **Rapid Drop (>3 hPa)**: Indicates approaching storm system - seek shelter
- **Rapid Rise (>3 hPa)**: Indicates weather improvement - conditions stabilizing

These thresholds can be adjusted based on local conditions and requirements.

## Hardware Requirements

- Arduino board (Uno, Mega, ESP32, or similar)
- Atmospheric pressure sensor (BMP280, BME280, or equivalent)
- I2C connection for sensor communication

## Installation

1. Clone this repository
2. Open `WeatherStation_EmergencyData.ino` in Arduino IDE
3. Install required sensor libraries (e.g., Adafruit BMP280)
4. Connect your pressure sensor to the I2C pins
5. Upload to your Arduino board

## Files

- `WeatherStation_EmergencyData.ino` - Main sketch
- `MovingAverage.h` - Moving average filter implementation
- `PressureMonitor.h` - Pressure monitoring and alert system
- `test_moving_average.ino` - Test suite for moving average functionality
- `examples/PressureMonitoring/` - Complete working example

## Configuration

Edit these constants in the main sketch to customize behavior:

```cpp
const int SAMPLE_SIZE = 20;              // Number of samples for moving average
const float PRESSURE_THRESHOLD = 3.0;    // hPa threshold for alerts
const unsigned long SAMPLE_INTERVAL = 60000; // Sample every 60 seconds
```

## Usage

### Basic Example

```cpp
#include "MovingAverage.h"
#include "PressureMonitor.h"

MovingAverage pressureAvg(20);
PressureMonitor monitor(3.0);

void loop() {
  float rawPressure = readSensor();
  pressureAvg.addSample(rawPressure);
  float smoothed = pressureAvg.getAverage();
  
  monitor.updatePressure(smoothed);
  if (monitor.isSevereWeatherDetected()) {
    // Alert user
  }
}
```

### Running Tests

To test the moving average implementation:
1. Rename `test_moving_average.ino` to a sketch folder
2. Upload to your Arduino
3. Open Serial Monitor (9600 baud) to view test results

## Algorithm Details

### Moving Average Calculation

The moving average is calculated using a circular buffer:

```
Average = Sum of all samples / Number of samples
```

When the buffer is full (20 samples), adding a new sample:
1. Subtracts the oldest value from the running sum
2. Adds the new value to the running sum
3. Moves the index forward (wraps around at buffer end)

This provides O(1) time complexity for each update.

### Pressure Change Detection

Pressure change is calculated as:

```
Change = Current Smoothed Pressure - Baseline Pressure
```

An alert is triggered when: `|Change| >= Threshold`

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is open source and available for use in emergency preparedness applications.
