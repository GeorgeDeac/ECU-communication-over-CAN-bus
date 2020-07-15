Sensor functionality:

1. Fuel level: The potentiometer is used to represent the fuel level of the car. It should be sent to the bus.
2. Indicator switches: 3 switches should be used to control the state of the indicators. There should be a switch for the left and right indicator, and the last switch should be used for the hazard light (left and right indicator at the same time). Note: To signalize that an indicator is switched on a logical ‘0’ should be sent. If it is switched off a logical ‘1’ must be sent.


ECU functionality:

1. This ECU receives the speed of the car and the state of the doors. The state of the doors must be sent to the bus. If the car moves faster than 5 km/h and a door is opened a warning signal should be sent additionally.
2. Moreover it must check if ECU4 – Engine Control is present. If ECU4 is not present LED7
should periodically blink. Additionally an error message must be sent.
