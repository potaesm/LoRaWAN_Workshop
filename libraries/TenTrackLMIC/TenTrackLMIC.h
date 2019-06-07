#ifndef TenTrackLMIC_h
#define TenTrackLMIC_h
#include "Arduino.h"
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "SimpleDHT.h"

class TenTrackLMIC {
    public:
    TenTrackLMIC();
    void LoRaSend(uint8_t data, uint32_t DEVADDR, uint8_t NWKSKEY, uint8_t APPSKEY, u1_t dr, s1_t power);
    uint8_t DHT11Temp(byte dht_input_pin);
    uint8_t DHT11Humid(byte dht_input_pin);
    uint8_t LDR(byte ldr_input_pin);
    private:
    SimpleDHT11 dht11;
    byte temperature = 0;
    byte humidity = 0;
    long dhtTemp_previousMillis = 0;
    long dhtHumid_previousMillis = 0;
};

#endif
