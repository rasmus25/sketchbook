/*
Using MCP3553 ADC in continuous mode with Arduino Uno
Author: Rasmus Raag
Tallinn University of Technology
 */

#include <SPI.h>

#define DATIN 12  // pin 12 : SPI data input
#define SCLOCK 13 // pin 13 : SPI clock pin
#define CHIPSEL 10 // pin 10 : Slave Chip Select 1

union SPI_data {
  byte packet[4];
  long value;
}data;

void mcp3553_single_read() {
        data.packet[3] = SPI.transfer(0x55);
	data.packet[2] = SPI.transfer(0xAA);
	data.packet[1] = SPI.transfer(0x55);
	data.packet[0] = SPI.transfer(0xAA);
        data.value >>= 7;
        // move the sign bit and clear overflow bits
        if(data.value & (1L<<21))
          data.value |= 0xFFC00000;
        else
          data.value &= ~0xFFC00000;
}
void mcp3553_setup() {
	pinMode(DATIN, INPUT);
	pinMode(SCLOCK, OUTPUT);
	pinMode(CHIPSEL, OUTPUT);
        SPI.begin();
        SPI.setDataMode(SPI_MODE0);
        // start continuous conversion
       	digitalWrite(CHIPSEL, LOW);
}

void setup() {
	Serial.begin(115200);
        mcp3553_setup();
}

void loop() {
	Serial.println((long) data.value, DEC);
	delay(100);
        mcp3553_single_read();
}

