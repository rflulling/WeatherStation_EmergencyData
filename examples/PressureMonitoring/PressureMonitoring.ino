/*
 * PressureMonitoring.ino
 * 
 * Example sketch demonstrating pressure monitoring with moving average
 * and severe weather detection.
 * 
 * This example shows how to:
 * - Use the MovingAverage class to smooth sensor readings
 * - Establish a baseline pressure over time
 * - Detect significant pressure changes that may indicate severe weather
 * 
 * Hardware Requirements:
 * - Arduino board (Uno, Mega, ESP32, etc.)
 * - Pressure sensor (BMP280, BME280, or similar)
 * 
 * Connections:
 * - Connect pressure sensor to I2C pins (SDA/SCL)
 * - Power sensor with 3.3V or 5V as appropriate
 */

#include "../../MovingAverage.h"
#include "../../PressureMonitor.h"

// If using a BMP280 or BME280 sensor, uncomment and configure:
// #include <Adafruit_BMP280.h>
// Adafruit_BMP280 bmp; // I2C

// Configuration constants
const int MOVING_AVG_SIZE = 20;        // 20-sample moving average
const float ALERT_THRESHOLD = 3.0;     // 3 hPa change triggers alert
const unsigned long SAMPLE_PERIOD = 60000; // Sample every 60 seconds

// Global objects
MovingAverage pressureFilter(MOVING_AVG_SIZE);
PressureMonitor weatherMonitor(ALERT_THRESHOLD);

unsigned long lastSampleTime = 0;
int sampleNumber = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  
  Serial.println();
  Serial.println("Weather Station - Pressure Monitoring Example");
  Serial.println("=============================================");
  Serial.println();
  
  // Initialize pressure sensor
  // Uncomment if using real sensor:
  // if (!bmp.begin()) {
  //   Serial.println("Could not find BMP280 sensor!");
  //   while (1) delay(10);
  // }
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
  //                 Adafruit_BMP280::SAMPLING_X2,
  //                 Adafruit_BMP280::SAMPLING_X16,
  //                 Adafruit_BMP280::FILTER_X16,
  //                 Adafruit_BMP280::STANDBY_MS_500);
  
  Serial.println("Configuration:");
  Serial.print("  Moving average window: ");
  Serial.print(MOVING_AVG_SIZE);
  Serial.println(" samples");
  Serial.print("  Alert threshold: ");
  Serial.print(ALERT_THRESHOLD);
  Serial.println(" hPa");
  Serial.print("  Sample period: ");
  Serial.print(SAMPLE_PERIOD / 1000);
  Serial.println(" seconds");
  Serial.println();
  Serial.println("Collecting initial samples to establish baseline...");
  Serial.println();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Take sample at regular intervals
  if (currentTime - lastSampleTime >= SAMPLE_PERIOD) {
    lastSampleTime = currentTime;
    sampleNumber++;
    
    // Read raw pressure from sensor
    float rawPressure = readPressure();
    
    // Apply moving average filter
    pressureFilter.addSample(rawPressure);
    float smoothedPressure = pressureFilter.getAverage();
    
    // Update weather monitor
    weatherMonitor.updatePressure(smoothedPressure);
    
    // Display current readings
    printReadings(sampleNumber, rawPressure, smoothedPressure);
    
    // Check for alerts
    checkForAlerts();
  }
}

// Read pressure from sensor
float readPressure() {
  // For real sensor, use:
  // return bmp.readPressure() / 100.0F; // Convert Pa to hPa
  
  // Simulation for testing (remove when using real sensor)
  static float simulatedPressure = 1013.25;
  static int trendCounter = 0;
  
  // Simulate realistic pressure variations
  trendCounter++;
  
  // Simulate a pressure drop after 30 samples (storm approaching)
  if (trendCounter > 30 && trendCounter < 50) {
    simulatedPressure -= 0.2;
  }
  // Simulate recovery
  else if (trendCounter >= 50 && trendCounter < 70) {
    simulatedPressure += 0.15;
  }
  
  // Add small random noise
  float noise = (random(-20, 20) / 100.0);
  
  return simulatedPressure + noise;
}

void printReadings(int sample, float raw, float smoothed) {
  Serial.print("Sample #");
  Serial.print(sample);
  Serial.print(" | Raw: ");
  Serial.print(raw, 2);
  Serial.print(" hPa | Smoothed: ");
  Serial.print(smoothed, 2);
  Serial.print(" hPa");
  
  if (weatherMonitor.isBaselineEstablished()) {
    float baseline = weatherMonitor.getBaselinePressure();
    float change = weatherMonitor.getPressureChange();
    
    Serial.print(" | Baseline: ");
    Serial.print(baseline, 2);
    Serial.print(" hPa | Δ: ");
    if (change >= 0) Serial.print("+");
    Serial.print(change, 2);
    Serial.print(" hPa");
  } else {
    int samplesNeeded = MOVING_AVG_SIZE - pressureFilter.getSampleCount();
    Serial.print(" | Baseline in ");
    Serial.print(samplesNeeded);
    Serial.print(" samples");
  }
  
  Serial.println();
}

void checkForAlerts() {
  if (weatherMonitor.isSevereWeatherDetected()) {
    float change = weatherMonitor.getPressureChange();
    
    Serial.println();
    Serial.println("╔════════════════════════════════════════╗");
    Serial.println("║   ⚠️  SEVERE WEATHER ALERT  ⚠️         ║");
    Serial.println("╚════════════════════════════════════════╝");
    Serial.print("Pressure change: ");
    if (change >= 0) Serial.print("+");
    Serial.print(change, 2);
    Serial.println(" hPa");
    Serial.println();
    
    if (change < -ALERT_THRESHOLD) {
      Serial.println("⬇️  RAPID PRESSURE DROP DETECTED");
      Serial.println("Indication: Storm system approaching");
      Serial.println("Recommendation: Seek shelter immediately");
      Serial.println("Status: Severe weather likely");
    } else if (change > ALERT_THRESHOLD) {
      Serial.println("⬆️  RAPID PRESSURE RISE DETECTED");
      Serial.println("Indication: Weather system improving");
      Serial.println("Recommendation: Conditions stabilizing");
      Serial.println("Status: May be safe to assess situation");
    }
    
    Serial.println("════════════════════════════════════════");
    Serial.println();
  }
}
