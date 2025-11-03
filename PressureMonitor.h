/*
 * PressureMonitor.h
 * 
 * Monitors atmospheric pressure and detects significant changes
 * that may indicate severe weather conditions.
 */

#ifndef PRESSURE_MONITOR_H
#define PRESSURE_MONITOR_H

class PressureMonitor {
  private:
    float baselinePressure;      // Established baseline pressure
    float currentPressure;       // Current smoothed pressure
    float pressureThreshold;     // Threshold for alerts (hPa)
    bool baselineEstablished;    // Whether baseline has been set
    bool severeWeatherDetected;  // Alert flag
    float pressureChange;        // Change from baseline
    int readingsCount;           // Number of readings processed
    int minReadingsForBaseline;  // Minimum readings before baseline is set
    
  public:
    // Constructor
    PressureMonitor(float threshold) {
      pressureThreshold = threshold;
      baselinePressure = 0.0;
      currentPressure = 0.0;
      baselineEstablished = false;
      severeWeatherDetected = false;
      pressureChange = 0.0;
      readingsCount = 0;
      minReadingsForBaseline = 20; // Wait for full moving average
    }
    
    // Update with new pressure reading
    void updatePressure(float pressure) {
      currentPressure = pressure;
      readingsCount++;
      
      // Establish baseline after collecting enough readings
      if (!baselineEstablished && readingsCount >= minReadingsForBaseline) {
        baselinePressure = pressure;
        baselineEstablished = true;
      }
      
      // Check for significant pressure changes
      if (baselineEstablished) {
        pressureChange = currentPressure - baselinePressure;
        
        // Detect severe weather based on threshold
        if (abs(pressureChange) >= pressureThreshold) {
          severeWeatherDetected = true;
        } else {
          severeWeatherDetected = false;
        }
        
        // Update baseline gradually to adapt to seasonal changes
        // Use exponential moving average with alpha = 0.01 (1% weight to new value)
        baselinePressure = baselinePressure * 0.99 + currentPressure * 0.01;
      }
    }
    
    // Check if severe weather is detected
    bool isSevereWeatherDetected() {
      return severeWeatherDetected;
    }
    
    // Get the established baseline pressure
    float getBaselinePressure() {
      return baselinePressure;
    }
    
    // Get current pressure
    float getCurrentPressure() {
      return currentPressure;
    }
    
    // Get pressure change from baseline
    float getPressureChange() {
      return pressureChange;
    }
    
    // Check if baseline has been established
    bool isBaselineEstablished() {
      return baselineEstablished;
    }
    
    // Reset the monitor
    void reset() {
      baselinePressure = 0.0;
      currentPressure = 0.0;
      baselineEstablished = false;
      severeWeatherDetected = false;
      pressureChange = 0.0;
      readingsCount = 0;
    }
    
    // Manually set baseline (useful for testing or calibration)
    void setBaseline(float pressure) {
      baselinePressure = pressure;
      baselineEstablished = true;
    }
    
    // Set the alert threshold
    void setThreshold(float threshold) {
      pressureThreshold = threshold;
    }
    
    // Get the alert threshold
    float getThreshold() {
      return pressureThreshold;
    }
};

#endif // PRESSURE_MONITOR_H
