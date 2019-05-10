#include "UTenTrackIoT.h"

static const PROGMEM u1_t NWKSKEY[16] = { 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00, 0x80, 0x0A };
static const u1_t PROGMEM APPSKEY[16] = { 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00, 0x80, 0x0A };
static const u4_t DEVADDR = 0xABCD1111;

u1_t DR = DR_SF9;
s1_t PW = 14;

UTenTrackIoT A;

int LDR= 0;
int TEMP = 0;
int HUMID = 0;

int DATA = 0;

void setup() {
  
  Serial.begin(115200);
  
}

void loop() {
  
  LDR = A.LDR(A0);
  TEMP = A.DHT11Temp(6);
  HUMID = A.DHT11Humid(6);

  delay(2500);

  DATA = LDR;
  
  A.LoRaSend(DATA, NWKSKEY, APPSKEY, DEVADDR, DR, PW);

  delay(1000);

}
