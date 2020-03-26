# arduino_i2c_slave
Turns your arduino into a basic read/write I2C device for testing purposes.

This Arduino sketch lets you turn your Arduino into an I2C testing unit.
Here is its minimal feature list:

1) Can work on any 7-bit address. Just change in the file
2) Automatically populates an array using a value generator for testing
3) User can write any 8-bit value to any of the 256 registers
4) Written values remain on-device until restart
5) User can read any value that has been written
6) More complicated transactions will be copied directly to serial output
