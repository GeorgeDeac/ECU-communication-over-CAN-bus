Sensor functionality:

1. RPM: The potentiometer of this ECU should be used to read the RPM value. This value must
be sent to the bus. Note: The value received by the potentiometer is 1/10 th of the actual
RPM. So a value of 410 would be 4100 RPM.
2. Gear sensor: Two buttons on this ECU should be used as gear sensors. One button should
indicate that the user shifted up. The other button is for down shifting. In every case a signal including the gear should be sent to the bus. Note: Let’s assume a 6-gear sequential manual transmission is used. At start up the gearbox is always in neutral (N). The 6-gear transmission should have the following gears: R – reverse, N – neutral, 1, 2, 3, 4, 5, 6. One byte should be used to send the data. Every bit in this byte represents one gear. So the reverse gear should be represented by ‘0b00000001’. The neutral gear is represented by ‘0b00000010’ and so on.


ECU functionality:
1. The Stability Control receives the speed of the drive wheels and the speed of the non-driven wheels. If the speed difference is bigger than 15 km/h a signal should be sent to the bus.

2. Moreover it must check if ECU2 – Light Control is present. If ECU2 is not present LED7 should periodically blink. Additionally an error message must be sent.
