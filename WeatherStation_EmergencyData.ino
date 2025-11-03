/*
 * WeatherStation Emergency Data Monitor
 * 
 * This sketch monitors atmospheric pressure and uses a moving average
 * to smooth out spikes in the data. It detects significant pressure
 * shifts that may indicate severe weather conditions.
 */

#include "MovingAverage.h"
#include "PressureMonitor.h"

// Configuration
const int SAMPLE_SIZE = 20;           // Number of samples for moving average
const float PRESSURE_THRESHOLD = 3.0; // hPa threshold for severe weather alert
const unsigned long SAMPLE_INTERVAL = 60000; // Sample every 60 seconds (1 minute)

// Global objects
MovingAverage pressureAverage(SAMPLE_SIZE);
PressureMonitor monitor(PRESSURE_THRESHOLD);

unsigned long lastSampleTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }
  
  Serial.println("Weather Station Emergency Data Monitor");
  Serial.println("======================================");
  Serial.println();
  Serial.println("Initializing pressure monitoring...");
  Serial.print("Moving average window: ");
  Serial.print(SAMPLE_SIZE);
  Serial.println(" samples");
  Serial.print("Pressure alert threshold: ");
  Serial.print(PRESSURE_THRESHOLD);
  Serial.println(" hPa");
  Serial.println();
  
  // Initialize with some readings to establish baseline
  Serial.println("Establishing baseline pressure...");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check if it's time to take a new sample
  if (currentTime - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = currentTime;
    
    // Read pressure from sensor (simulated for now)
    float rawPressure = readPressureSensor();
    
    // Add to moving average
    pressureAverage.addSample(rawPressure);
    
    // Get smoothed pressure value
    float smoothedPressure = pressureAverage.getAverage();
    
    // Update monitor with smoothed pressure
    monitor.updatePressure(smoothedPressure);
    
    // Display readings
    displayReadings(rawPressure, smoothedPressure);
    
    // Check for severe weather alerts
    if (monitor.isSevereWeatherDetected()) {
      displayAlert(monitor.getPressureChange());
    }
  }
}

// Read pressure from sensor
// This is a placeholder - implement actual sensor reading here
float readPressureSensor() {
  // For testing: simulate pressure readings
  // In production, replace with actual sensor code (e.g., BMP280, BME280)
  // Example: return bmp.readPressure() / 100.0F; // Convert Pa to hPa
  
  static float basePressure = 1013.25; // Standard sea level pressure
  static float variation = 0.0;
  
  // Simulate small variations
  variation += (random(-100, 100) / 100.0);
  if (variation > 5.0) variation = 5.0;
  if (variation < -5.0) variation = -5.0;
  
  return basePressure + variation;
}

void displayReadings(float raw, float smoothed) {
  Serial.print("Time: ");
  Serial.print(millis() / 1000);
  Serial.print("s | Raw: ");
  Serial.print(raw, 2);
  Serial.print(" hPa | Smoothed: ");
  Serial.print(smoothed, 2);
  Serial.print(" hPa");
  
  if (pressureAverage.isFull()) {
    float baseline = monitor.getBaselinePressure();
    float change = smoothed - baseline;
    Serial.print(" | Baseline: ");
    Serial.print(baseline, 2);
    Serial.print(" hPa | Change: ");
    if (change >= 0) Serial.print("+");
    Serial.print(change, 2);
    Serial.print(" hPa");
  } else {
    Serial.print(" | Collecting baseline data...");
  }
  
  Serial.println();
}

void displayAlert(float pressureChange) {
  Serial.println();
  Serial.println("*** SEVERE WEATHER ALERT ***");
  Serial.print("Significant pressure change detected: ");
  if (pressureChange >= 0) Serial.print("+");
  Serial.print(pressureChange, 2);
  Serial.println(" hPa");
  
  if (pressureChange < -PRESSURE_THRESHOLD) {
    Serial.println("Status: Rapid pressure DROP - Storm system approaching");
    Serial.println("Action: Seek shelter, severe weather likely");
  } else if (pressureChange > PRESSURE_THRESHOLD) {
    Serial.println("Status: Rapid pressure RISE - Weather improving");
    Serial.println("Action: Conditions stabilizing");
  }
  
  Serial.println("*****************************");
  Serial.println();
}
