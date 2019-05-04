#include "UTenTrackIoT.h"

UTenTrackIoT A;

int ldr= 0;

void setup() {
  
  Serial.begin(115200);
  
}

void loop() {
  
  ldr= A.LDR(A0);
  
  A.LoRaSend(ldr);
  
  delay(5000);

}
