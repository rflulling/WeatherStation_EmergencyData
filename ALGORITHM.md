# Moving Average Algorithm Documentation

## Overview

This document explains the moving average algorithm used to normalize pressure samples over a 20-sample window.

## Marching (Rolling) Average

A marching average, also known as a rolling or sliding window average, is a calculation that analyzes data points by creating a series of averages of different subsets of the full dataset.

### Algorithm

The moving average uses a circular buffer implementation:

1. **Buffer Initialization**: Create an array of size N (20 samples)
2. **Sample Addition**: 
   - When a new sample arrives, it's added to the current position
   - If the buffer is full, the old value at that position is first removed from the sum
   - The new value is added to the running sum
3. **Index Management**: 
   - Index advances by 1 after each addition
   - When index reaches the end (N), it wraps to 0 (circular buffer)
4. **Average Calculation**: Average = Sum / Count

### Pseudocode

```
class MovingAverage:
    buffer[N]  // Array of N samples
    index = 0  // Current position in buffer
    count = 0  // Number of samples (up to N)
    sum = 0    // Running sum
    
    function addSample(value):
        if count >= N:
            sum = sum - buffer[index]  // Remove old value
        
        buffer[index] = value
        sum = sum + value
        
        index = (index + 1) mod N  // Wrap around
        if count < N:
            count = count + 1
    
    function getAverage():
        return sum / count
```

### Time Complexity

- **Add Sample**: O(1) - constant time
- **Get Average**: O(1) - constant time
- **Space**: O(N) - stores N samples

### Benefits

1. **Spike Smoothing**: Outlier values are diluted across N samples
2. **Memory Efficient**: Only stores N samples, not entire history
3. **Computationally Efficient**: No need to recalculate entire average
4. **Responsive**: Adapts to trends as old samples are replaced

## Baseline Establishment

### Initial Baseline

The baseline is established after collecting the first 20 samples (when the moving average buffer is full):

```
baseline = average of first 20 samples
```

### Adaptive Baseline

To account for gradual environmental changes (seasonal pressure variations, altitude changes), the baseline slowly adapts using an exponential moving average:

```
baseline = baseline × 0.99 + current_pressure × 0.01
```

This gives 99% weight to the existing baseline and 1% to the new reading, allowing:
- Long-term adaptation to seasonal changes
- Resistance to short-term fluctuations
- Sensitivity to genuine severe weather events

## Severe Weather Detection

### Pressure Change Calculation

```
pressure_change = current_smoothed_pressure - baseline_pressure
```

### Alert Thresholds

Default threshold: **3.0 hPa** (30 millibars)

- **Rapid Drop** (change < -3.0 hPa): Storm approaching
  - Indicates low-pressure system moving in
  - Associated with severe weather, storms, hurricanes
  
- **Rapid Rise** (change > +3.0 hPa): Weather improving
  - Indicates high-pressure system moving in
  - Associated with clearing conditions

### Threshold Justification

The 3 hPa threshold is based on meteorological standards:

- Normal diurnal pressure variation: ~1-2 hPa
- Significant weather system: 3-5 hPa over several hours
- Major storm system: >5 hPa
- Hurricane eye wall: 10-50+ hPa

A 3 hPa threshold provides:
- Low false positives from normal variations
- Early warning for approaching severe weather
- Time to take protective action

## Example Scenarios

### Scenario 1: Approaching Storm

```
Sample  Raw     Smoothed  Baseline  Change  Status
1-20    1013    1013      -         -       Establishing baseline
21-30   1013    1013      1013      0       Normal
31-40   1010    1012      1013      -1      Pressure dropping
41-50   1008    1010      1013      -3      ALERT! Storm approaching
51-60   1007    1009      1013      -4      Severe weather
```

### Scenario 2: Spike Smoothing

```
Sample  Raw     Smoothed  Effect
1       1013    1013      -
2       1025    1019      Spike dampened by 50%
3       1013    1017      Continuing to smooth
4       1013    1016      -
...
20      1013    1013.6    Spike effect minimal
21      1013    1013      Spike completely removed
```

## Implementation Notes

### Arduino Considerations

1. **Memory**: Each float (4 bytes) × 20 samples = 80 bytes
2. **Processing**: All operations are O(1), suitable for embedded systems
3. **Floating Point**: Uses standard float arithmetic (sufficient precision)

### Sensor Integration

For actual deployment:

```cpp
// Example with BMP280 sensor
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

float readPressure() {
    return bmp.readPressure() / 100.0F; // Convert Pa to hPa
}
```

### Calibration

1. Let system run for 20+ samples to establish baseline
2. Verify baseline matches local conditions
3. Adjust threshold if needed for local weather patterns
4. Consider altitude compensation if applicable

## Testing

The `test_moving_average.ino` and `simulate_moving_average.py` files provide:

- Unit tests for the moving average algorithm
- Spike smoothing demonstrations
- Severe weather detection validation
- Performance verification

## References

- Moving Average: https://en.wikipedia.org/wiki/Moving_average
- Atmospheric Pressure: https://en.wikipedia.org/wiki/Atmospheric_pressure
- Weather Prediction: https://www.weather.gov/source/zhu/ZHU_Training_Page/pressure_weather_relationship/pressure_weather.htm
