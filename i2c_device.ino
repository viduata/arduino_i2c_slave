// i2c_tester
//
// This program is fairly uninteresting
// All it does is maintain 256 bytes of data.
//
// At startup, every byte's value is populated by a generator
// Connected devices can test their I2C units by requesting 
// the value at a register and comparing to what should be
// generated.
//
// Additionally, any register can be written, then read back
// to find the assigned value where it belongs.

#include <Wire.h>

const unsigned char SLAVE_ADDRESS = 0x23; // Use whatever 7-bit address you want here
byte regs[256];                           // Holder array for 256 values
byte reg_address;                         // Global that is used for read.

           
void setup() 
{
  reg_address = 0;
  init_regs();                    // Put test values into all registers
  Serial.begin(115200);           // Speaks serial
  Wire.begin(SLAVE_ADDRESS);      // 7-bit address
  Wire.onRequest(requestEvent);   // Start the request handler
  Wire.onReceive(receiveEvent);   // Start the receive handler
  Serial.println("Starting");     // Start message
}

void loop() 
{
  1; // Main loop does nothing. This is all event-based
}


// This is executed when the master uses our read address
void requestEvent() 
{
  Wire.write(regs[reg_address]);
}


// This is executed when the master uses our write address
void receiveEvent(int numBytes) 
{
  int buff     = 0;
  int counter  = 0;
  int my_reg   = 0;
  int my_value = 0;
  char msg[10];

  // We're only handling two cases: Basic reg write and read.
  // In a write, we send the reg address and the value
  
  // In a read, we send the reg address to the write address,
  // then we come back and make a request to the read address.

  if(numBytes == 1) // This is the case of a read
  {
    reg_address = Wire.read(); // Update the global
    sprintf(msg, "Requesting %p\r\n", reg_address); // Write the received byte to serial
    Serial.print(msg);
  }


  else if (numBytes ==2 ) // The is the case of a write
  {
    my_reg = Wire.read();   // There are only two bytes in the buffer
    my_value = Wire.read(); // so I think this is safe
    regs[my_reg] = my_value;
    sprintf(msg, "Wrote %p to %p\r\n", my_value, my_reg); // Write the received byte to serial
    Serial.print(msg);
  }

  else  // Any other case, we just print the hex values to serial
  {
    while (Wire.available()>0)
    {
    buff = Wire.read();
    Serial.println(buff, HEX);
    }
  }
}


// This is a simple array-filler that you can use to verify
// that the device is returning the correct values of a basic
// read function.
//
// When you request the value in a newly-initialized array,
// you should get the reg number xor'ed against 0x55.
// This is easy to verify on the master side and can be 
// implemented in only a couple lines of code.
void init_regs()
{
  int x;
  for (x=0; x<256; x++)
  {
    regs[x]=x^0x55; // Flip every other bit
  }
}
