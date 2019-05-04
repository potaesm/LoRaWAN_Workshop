#ifndef UTenTrackIoT_h
#define UTenTrackIoT_h
#include "Arduino.h"
#include "lmic.h"
#include "hal/hal.h"
#include "SPI.h"

#include "SimpleDHT.h"
#include "AccelUNO.h"

class UTenTrackIoT {

  public:
    UTenTrackIoT();
    void LoRaSend(int value);
    int DHT11Temp(byte dht_input_pin);
    int DHT11Humid(byte dht_input_pin);
    int LDR(byte ldr_input_pin);
  private:
    SimpleDHT11 dht11;
    AccelUNO mpu;
    bool LDRsetup = false;
    byte temperature = 0;
    byte humidity = 0;
    long dhtTemp_previousMillis = 0;
    long lora_previousMillis = 0;
    long dhtHumid_previousMillis = 0;
};

#endif
