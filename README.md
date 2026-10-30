# morse-laser-link

Built in Sagomungchi (Samoong), the invention club at Seoul Science High School, 2024.

```
transmitter/transmitter.ino   serial text  ->  laser pulses
receiver/receiver.ino         laser pulses ->  serial text + LCD
```

## Hardware

| | |
|---|---|
| Transmitter | Arduino, laser diode module on pin 12, tripod mount |
| Receiver | Arduino, photoresistor on A0, 16×2 I²C LCD at `0x27`, pinhole in a hinged box lid |

## How it works

To adapt to changing ambient light, the receiver updates its own threshold. At start-up it
reads the photoresistor once with the beam blocked, waits for `rebooting` over serial,
reads once more with the beam on the sensor, and takes the midpoint of the two as the
threshold.

```cpp
referenceValue = (sensorValueBeforeRebooting + sensorValueAfterRebooting) / 2;
```

One unit of detection is `dotDelay` = 40 ms. The receiver samples A0 every 20 ms — half a
unit — and counts how many samples the signal stays on one side of the threshold. Whenever
the signal changes side, the run that just ended is classified by the nearest expected
duration.

| run | beam on | beam off |
|---|---|---|
| 1 unit | dot | gap inside a letter |
| 3 units | dash | |
| 4 units | | end of letter — look the pattern up, emit a character |
| 5 units | | end of word — emit the character, then a space |
| 6 units | | end of sentence — print the whole line and clear the buffer |

Nearest-match classification lets the receiver also read a hand-keyed or slightly drifting
signal.

## Running it

1. I'd recommend aiming first and uploading afterwards. While lining the laser up on the
   receiver's pinhole, check that it does not wobble.
2. Upload both sketches. The receiver in particular has to be uploaded with the beam
   blocked, so that its first reading is the dark one and the calibration comes out right.
3. Open the receiver's Serial Monitor at 9600 baud, put the beam back on the sensor, and
   send `rebooting`. The threshold is calibrated at that moment and the receiver starts
   receiving properly.
4. Type into the transmitter's Serial Monitor. Decoded text appears on the receiver's
   Serial Monitor and LCD.

`receiver.ino` is the version with the LCD. Remove the `LiquidCrystal_I2C` calls and it
prints to serial only.


## Special Thanks

To Seoul Global High School (SGHS), for participating communication event and making their rooftop accessible to test and run the laser morse 
transceivers over a long distance.


## Licence

MIT. See `LICENSE`.
