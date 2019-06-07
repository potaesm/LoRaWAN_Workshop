#include "TenTrackLMIC.h"

//Set device address
uint8_t NWKSKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
//Set network session key
uint8_t APPSKEY[16] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
//Set app session key
uint32_t DEVADDR = 0x03FF0001 ;

//Set data rate (DR_SF7 - DR_SF12)
u1_t DR = DR_SF10;
//Set Tx power (dBm)
s1_t PW = 14;

uint8_t data = 0;

TenTrackLMIC Node;

bool SWCheck = 0;

void setup() {

  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(115200);

}

void loop() {

  data = Node.DHT11Humid(6);
  // data = Node.DHT11Temp(6);
  // data = Node.LDR(A0);

  if (digitalRead(3) == HIGH) {
    
    if (SWCheck == 0) {
      
      Serial.println("Switch is pressed");
      Serial.println("Sending data ...");
      digitalWrite(2, HIGH);
      Node.LoRaSend(data, DEVADDR, NWKSKEY, APPSKEY, DR, PW);
      SWCheck = 1;
      
    }
    
  } else {
    
    Serial.println("Switch is released");
    digitalWrite(2, LOW);
    SWCheck = 0;
    
  }

}
