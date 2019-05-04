#include "Arduino.h"
#include "UTenTrackIoT.h"

static const PROGMEM u1_t NWKSKEY[16] = { 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00, 0x80, 0x0A };
static const u1_t PROGMEM APPSKEY[16] = { 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00, 0x80, 0x0A };
static const u4_t DEVADDR = 0xABCD1111;

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

char buff[15];

//Original data type
static uint8_t mydata[] = "";
static osjob_t sendjob;

const unsigned TX_INTERVAL = 60;
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = {4, 5, 7},
};

UTenTrackIoT::UTenTrackIoT() {

}

void do_send(osjob_t* j, int rData) {
    
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        Serial.print("Send : ");
        Serial.println(rData);
        dtostrf(rData, 5, 1, buff);
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, buff, strlen(buff), 0);
        Serial.print(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
    
}

void onEvent(ev_t ev) {
    
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
                Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
                Serial.println(F("Received "));
                Serial.println(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        default:
            Serial.println(F("Unknown event"));
            break;
    }
    
}

void UTenTrackIoT::LoRaSend(int value) {

    Serial.println(F("Starting"));
    
    #ifdef VCC_ENABLE
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif
    
    os_init();
    LMIC_reset();
    
    #ifdef PROGMEM
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif
    
    #if defined(CFG_eu868)
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    #elif defined(CFG_us915)
    LMIC_selectSubBand(1);
    #endif
    
    LMIC_setLinkCheckMode(0);
    LMIC.dn2Dr = DR_SF9;
    LMIC_setDrTxpow(DR_SF7, 14);

    //Send the data
    do_send(&sendjob, value);

    os_runloop_once();

}

int UTenTrackIoT::DHT11Temp(byte dht_input_pin) {

    int localTemp = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - dhtTemp_previousMillis > 1000) {
        dht11.read(dht_input_pin, &temperature, &humidity, NULL);
        localTemp = (int)temperature;
        dhtTemp_previousMillis = currentMillis;
    }

    return localTemp;
    
}

int UTenTrackIoT::DHT11Humid(byte dht_input_pin) {

    int localHumid = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - dhtHumid_previousMillis > 1000) {
        dht11.read(dht_input_pin, &temperature, &humidity, NULL);
        localHumid = (int)humidity;
        dhtHumid_previousMillis = currentMillis;
    }

    return localHumid;
    
}

int UTenTrackIoT::LDR(byte ldr_input_pin) {

    int localLDR = 0;
    if (LDRsetup != true) {
        pinMode(ldr_input_pin, INPUT);
        LDRsetup = true;
    }

    localLDR = analogRead(ldr_input_pin);

    return localLDR;

}
