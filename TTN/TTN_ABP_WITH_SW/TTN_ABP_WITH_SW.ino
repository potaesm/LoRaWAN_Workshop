#include "TenTrackLMIC.h"

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

  if (digitalRead(3) == HIGH) {

    Serial.println("Sending data ...");

    // data = Node.DHT11Humid(6);
    data = Node.DHT11Temp(6);
    // data = Node.LDR(A0);

    delay(800);

    if (SWCheck == 0) {

      digitalWrite(2, HIGH);
      Node.LoRaSend(data, DR, PW);
      SWCheck = 1;

    }

  } else {

    digitalWrite(2, LOW);
    SWCheck = 0;

  }

}
