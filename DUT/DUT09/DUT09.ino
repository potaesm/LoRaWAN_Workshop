#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <CayenneLPP.h>
#include <SimpleDHT.h>

//Set device address
static const u4_t DEVADDR = 0x260412F0;
//Set network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x68, 0x55, 0xB0, 0x9E, 0x57, 0x7C, 0xEA, 0x44, 0xF1, 0x87, 0x54, 0xD3, 0x28, 0x7A, 0xAE, 0xDE };
//Set app session key
static const u1_t PROGMEM APPSKEY[16] = { 0x21, 0x35, 0x11, 0x1F, 0x85, 0x60, 0x33, 0x04, 0xC3, 0x5B, 0xA3, 0xBF, 0x9A, 0x03, 0x2C, 0x9E };

//Set data rate
u1_t DR = DR_SF11;
//Set Tx power
s1_t PW = 14;

SimpleDHT11 dht11;
CayenneLPP lpp(100);

byte temp = 0;
byte humid = 0;
float ldr = 0;

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static osjob_t sendjob;

const unsigned TX_INTERVAL = 30;

const lmic_pinmap lmic_pins = {
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = LMIC_UNUSED_PIN,
  .dio = {4, 5, 7},
};

void onEvent (ev_t ev) {
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
        Serial.println(LMIC.rps);
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

void do_send(osjob_t* j) {

  dht11.read(6, &temp, &humid, NULL);
  ldr = analogRead(A0)/10;

  lpp.reset();
  
  lpp.addTemperature(3, (float)temp);
  lpp.addRelativeHumidity(4, (float)humid);
  lpp.addAnalogInput(5, ldr);

  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);
    Serial.println(F("Packet queued"));
    Serial.print("Temperature : ");
    Serial.println(temp);
    Serial.print("Humidity : ");
    Serial.println(humid);
    Serial.print("LDR : ");
    Serial.println(ldr);
  }

}

void setup() {

  Serial.begin(9600);
  Serial.println(F("Starting"));

#ifdef VCC_ENABLE
  pinMode(VCC_ENABLE, OUTPUT);
  pinMode(A0, INPUT);
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

  LMIC_setupChannel(0, 923200000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 923400000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 922200000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 922400000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 922600000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 922800000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 923000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 922200000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(8, 921800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

  LMIC_setLinkCheckMode(0);

  LMIC.dn2Dr = DR_SF9;

  LMIC_setDrTxpow(DR, PW);

  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}
