#ifndef TenTrackLMIC_h
#define TenTrackLMIC_h
#include "Arduino.h"
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "SimpleDHT.h"

//Set device address
static const u4_t DEVADDR = 0x26041753;
//Set network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x90, 0x5F, 0x5E, 0x5D, 0x4A, 0xB3, 0x83, 0xC5, 0xFD, 0x66, 0x6D, 0x9E, 0xFF, 0x1C, 0xFE, 0x93 };
//Set app session key
static const u1_t PROGMEM APPSKEY[16] = { 0x1F, 0x00, 0x8D, 0x6E, 0x34, 0xC5, 0x59, 0x88, 0x52, 0x2B, 0x73, 0xCE, 0xD1, 0x6F, 0x22, 0x7A };

class TenTrackLMIC {
    public:
    TenTrackLMIC();
    void LoRaSend(uint8_t data, u1_t dr, s1_t power);
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
