#include "UTenTrackIoT.h"

UTenTrackIoT A;

int temp = 0;

void setup() {
  
  Serial.begin(115200);
  
}

void loop() {
  
  temp = A.DHT11Temp(6);
  
  A.LoRaSend(temp);
  
  delay(5000);

}
