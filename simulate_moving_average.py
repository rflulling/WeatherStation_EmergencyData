#!/usr/bin/env python3
"""
Moving Average Simulation

This script demonstrates how the 20-sample moving average smooths
pressure data and how the system detects severe weather conditions.
"""

import random
import math

class MovingAverage:
    """Implements a moving average with a circular buffer."""
    
    def __init__(self, size=20):
        self.size = size
        self.buffer = [0.0] * size
        self.index = 0
        self.count = 0
        self.sum = 0.0
    
    def add_sample(self, value):
        """Add a new sample to the moving average."""
        # Subtract old value if buffer is full
        if self.count >= self.size:
            self.sum -= self.buffer[self.index]
        
        # Add new value
        self.buffer[self.index] = value
        self.sum += value
        
        # Update index and count
        self.index = (self.index + 1) % self.size
        if self.count < self.size:
            self.count += 1
    
    def get_average(self):
        """Get the current average."""
        if self.count == 0:
            return 0.0
        return self.sum / self.count
    
    def is_full(self):
        """Check if buffer is full."""
        return self.count >= self.size


class PressureMonitor:
    """Monitors pressure and detects severe weather."""
    
    def __init__(self, threshold=3.0):
        self.threshold = threshold
        self.baseline = None
        self.current = 0.0
        self.readings_count = 0
        self.min_readings = 20
    
    def update_pressure(self, pressure):
        """Update with new pressure reading."""
        self.current = pressure
        self.readings_count += 1
        
        # Establish baseline
        if self.baseline is None and self.readings_count >= self.min_readings:
            self.baseline = pressure
        
        # Gradually adapt baseline
        if self.baseline is not None:
            self.baseline = self.baseline * 0.99 + pressure * 0.01
    
    def get_change(self):
        """Get pressure change from baseline."""
        if self.baseline is None:
            return 0.0
        return self.current - self.baseline
    
    def is_severe_weather(self):
        """Check if severe weather is detected."""
        if self.baseline is None:
            return False
        return abs(self.get_change()) >= self.threshold


def simulate_pressure_data(num_samples=100):
    """Generate simulated pressure data with variations and spikes."""
    data = []
    base_pressure = 1013.25  # Standard sea level pressure (hPa)
    
    for i in range(num_samples):
        pressure = base_pressure
        
        # Simulate a storm approaching (samples 30-50)
        if 30 <= i < 50:
            pressure -= (i - 30) * 0.25
        # Simulate recovery (samples 50-70)
        elif 50 <= i < 70:
            pressure = base_pressure - 5.0 + (i - 50) * 0.25
        
        # Add random spikes occasionally
        if i in [15, 16, 65]:
            pressure += random.uniform(5, 10)
        
        # Add small random noise
        pressure += random.uniform(-0.3, 0.3)
        
        data.append(pressure)
    
    return data


def main():
    print("Weather Station - Moving Average Simulation")
    print("=" * 50)
    print()
    
    # Configuration
    window_size = 20
    threshold = 3.0
    
    print(f"Configuration:")
    print(f"  Moving average window: {window_size} samples")
    print(f"  Alert threshold: {threshold} hPa")
    print()
    
    # Initialize
    moving_avg = MovingAverage(window_size)
    monitor = PressureMonitor(threshold)
    
    # Generate simulated data
    pressure_data = simulate_pressure_data(100)
    
    print("Simulation Results:")
    print("-" * 80)
    print(f"{'Sample':<8} {'Raw':<10} {'Smoothed':<10} {'Baseline':<10} {'Change':<10} {'Alert'}")
    print("-" * 80)
    
    alerts_triggered = []
    
    for i, raw_pressure in enumerate(pressure_data):
        # Apply moving average
        moving_avg.add_sample(raw_pressure)
        smoothed = moving_avg.get_average()
        
        # Update monitor
        monitor.update_pressure(smoothed)
        
        # Display results
        sample_num = i + 1
        baseline = monitor.baseline if monitor.baseline else 0.0
        change = monitor.get_change()
        alert = "*** ALERT ***" if monitor.is_severe_weather() else ""
        
        if alert:
            alerts_triggered.append((sample_num, change))
        
        # Print every 5th sample to keep output manageable
        if i % 5 == 0 or alert:
            print(f"{sample_num:<8} {raw_pressure:<10.2f} {smoothed:<10.2f} "
                  f"{baseline:<10.2f} {change:+<10.2f} {alert}")
    
    print("-" * 80)
    print()
    
    # Summary
    print("Summary:")
    print(f"  Total samples: {len(pressure_data)}")
    print(f"  Alerts triggered: {len(alerts_triggered)}")
    
    if alerts_triggered:
        print()
        print("  Alert details:")
        for sample, change in alerts_triggered:
            direction = "DROP" if change < 0 else "RISE"
            print(f"    Sample {sample}: {change:+.2f} hPa ({direction})")
    
    print()
    print("Simulation demonstrates:")
    print("  ✓ Moving average smooths out data spikes")
    print("  ✓ Baseline pressure is established after 20 samples")
    print("  ✓ Severe weather is detected when pressure changes exceed threshold")
    print("  ✓ System adapts baseline gradually over time")


if __name__ == "__main__":
    main()
