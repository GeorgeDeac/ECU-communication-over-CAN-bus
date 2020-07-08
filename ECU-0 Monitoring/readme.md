Sensor functionality:
1. Light sensor: The light value received by the photo resistor connected to this board should be sent to the bus.
2. Door sensors: Two switches should represent sensors included in the doors. One switch for the left door and another switch for the right door. If the doors are opened, a logical ‘0’ should be written to the bus. A logical ‘1’ should signalize that the door is closed.

ECU functionality:
1. This ECU receives the fuel level and the engine coolant temperature. If the fuel level  reaches 1/10 th of the maximum a signal should be written to the bus. If the coolant temperature reaches 9/10 th of the maximum another warning signal written to the bus.
2. Moreover it must check if ECU1 – Stability Control is present. If ECU1 is not present LED7 should periodically blink. Additionally an error message must be sent.
