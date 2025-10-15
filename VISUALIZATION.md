# Moving Average Visual Examples

## Circular Buffer Operation

### Initial State (Empty Buffer, Size=20)
```
Index:  0   1   2   3   4  ...  18  19
Value:  -   -   -   -   -  ...   -   -
        ^
     current
```

### After Adding 5 Samples
```
Index:  0   1   2   3   4  ...  18  19
Value: 10  12  11  13  12  ...   -   -
                        ^
                    current
Count: 5, Sum: 58, Average: 11.6
```

### Buffer Full (20 Samples)
```
Index:   0    1    2    3   ...  18   19
Value: 1013 1014 1012 1013 ... 1015 1016
                                         ^
                                     current
Count: 20, Sum: 20265, Average: 1013.25
```

### Adding Sample #21 (Wraps Around)
```
Before:
Index:   0    1    2    3   ...  18   19
Value: 1013 1014 1012 1013 ... 1015 1016
        ^                                
     current (wraps to 0)

After (New Value: 1017):
Index:   0    1    2    3   ...  18   19
Value: 1017 1014 1012 1013 ... 1015 1016
             ^                            
         current

Old sum: 20265
Remove old value (1013): 20265 - 1013 = 19252
Add new value (1017):    19252 + 1017 = 20269
New average: 20269 / 20 = 1013.45
```

## Spike Smoothing Example

### Raw Data with Spike
```
Sample:  1    2    3    4    5    6    7    8    9   10
Raw:    1013 1013 1013 1025 1013 1013 1013 1013 1013 1013
                          ^
                       SPIKE!
```

### Moving Average Response (Window=5)
```
Sample:  1      2      3      4      5      6      7      8      9     10
Raw:    1013   1013   1013   1025   1013   1013   1013   1013   1013   1013
Avg:    1013.0 1013.0 1013.0 1015.5 1015.4 1015.4 1015.4 1013.6 1013.0 1013.0
                              ^^^^   ^^^^   ^^^^   ^^^^   ^^^^
                              Spike dampened and smoothed over window
```

Notice how:
- Spike at sample 4 (1025) is immediately dampened to 1015.5
- Effect persists for 5 samples (window size)
- By sample 10, spike is completely removed

### With 20-Sample Window
```
The spike effect is even more dampened:
Sample 4:  1013.6 (only +0.6 instead of +12)
Sample 24: 1013.0 (spike completely removed after 20 samples)
```

## Severe Weather Detection Example

### Storm Approaching Scenario
```
Time (min)  Raw (hPa)  Smoothed  Baseline  Change  Status
0-20        1013       1013      -         -       Establishing baseline
20-30       1013       1013      1013      0.0     ✓ Normal
30-40       1012       1013      1013     -0.5     ⚠ Slight drop
40-50       1010       1011      1013     -2.0     ⚠ Dropping
50-60       1008       1010      1013     -3.0     🚨 ALERT! Storm approaching
60-70       1007       1009      1013     -4.0     🚨 Severe weather
70-80       1006       1008      1013     -5.0     🚨 Major storm
```

### Visualization
```
Pressure (hPa)
1015 |     ________
1014 |    /        
1013 |___/          Baseline (adaptive)
1012 |              \
1011 |               \___
1010 |                   \___      Smoothed (moving avg)
1009 |                       \___
1008 |__________________________|
     |                          ~~~~ Raw (with spikes)
     0    20    40    60    80   Time (minutes)
                          ^
                       ALERT triggered at -3.0 hPa
```

## Buffer State Transitions

### Example: 5-Sample Buffer

```
Step 1: Add 1013
[1013, -, -, -, -]  Index=0→1, Count=1, Sum=1013, Avg=1013.0

Step 2: Add 1014
[1013, 1014, -, -, -]  Index=1→2, Count=2, Sum=2027, Avg=1013.5

Step 3: Add 1012
[1013, 1014, 1012, -, -]  Index=2→3, Count=3, Sum=3039, Avg=1013.0

Step 4: Add 1013
[1013, 1014, 1012, 1013, -]  Index=3→4, Count=4, Sum=4052, Avg=1013.0

Step 5: Add 1015
[1013, 1014, 1012, 1013, 1015]  Index=4→0, Count=5, Sum=5067, Avg=1013.4

Step 6: Add 1016 (Buffer Full - Replacement Begins)
Remove buffer[0]=1013: Sum = 5067 - 1013 = 4054
Add 1016:              Sum = 4054 + 1016 = 5070
[1016, 1014, 1012, 1013, 1015]  Index=0→1, Count=5, Sum=5070, Avg=1014.0

Step 7: Add 1017
Remove buffer[1]=1014: Sum = 5070 - 1014 = 4056
Add 1017:              Sum = 4056 + 1017 = 5073
[1016, 1017, 1012, 1013, 1015]  Index=1→2, Count=5, Sum=5073, Avg=1014.6
```

## Performance Characteristics

```
Operation        Time      Space     Notes
─────────────────────────────────────────────────────────
Initialize       O(N)      O(N)      One-time setup
Add Sample       O(1)      O(1)      Constant time update
Get Average      O(1)      O(1)      Pre-calculated
Check Full       O(1)      O(1)      Simple comparison
Reset            O(N)      O(1)      Clear buffer

N = buffer size (20 for this application)
```

## Memory Usage

```
Arduino (32-bit floats):
─────────────────────────
Buffer:        20 floats × 4 bytes = 80 bytes
Variables:     4 ints × 2 bytes    =  8 bytes
Total:                               88 bytes

Very efficient for embedded systems!
```

## Comparison: With vs Without Moving Average

### Without Moving Average (Raw Data)
```
Sample:  1    2    3    4    5    6    7    8    9   10
Value:  1013 1025 1012 1008 1015 1011 1009 1020 1013 1010
        
Issues:
- Spikes cause false alarms
- Hard to detect real trends
- Noisy signal
```

### With 20-Sample Moving Average
```
Sample:  1    2    3    4    5    6    7    8    9   10
Raw:    1013 1025 1012 1008 1015 1011 1009 1020 1013 1010
Smooth: 1013 1019 1017 1015 1015 1014 1013 1014 1014 1014

Benefits:
✓ Spikes smoothed
✓ Trends clear
✓ Reliable alerts
```
