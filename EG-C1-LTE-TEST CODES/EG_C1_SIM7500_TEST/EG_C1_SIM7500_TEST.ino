
#define MODEM_RESET 32
#define MODEM_FLIGHT 22
#define MODEM_RX 26
#define MODEM_TX 25

#include <Adafruit_ADS1015.h>
#include "HX711.h"

Adafruit_ADS1115 ads1(0x49);
String adcString[8];
HX711 scale;

const int LOADCELL_DOUT_PIN = 33;
const int LOADCELL_SCK_PIN = 32;

long timer1;

void setup() {
  // Initialize both serial ports:
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  pinMode(MODEM_FLIGHT, OUTPUT); // FLIGHT MODE ENABLE
  pinMode(MODEM_RESET, OUTPUT);  // MODEM RESET PIN
  digitalWrite(MODEM_FLIGHT, HIGH); // FLIGHT MODE

  MODEM_RESET_CYC();
  delay(2000);

  Serial.println("SIM AT ATART >>>>>>>>>>>>>>");
  delay(2000);

  Serial2.println("AT");
  delay(2000);

  Serial2.println("AT+CPIN?");
  delay(2000);

  Serial2.println("AT+CNMP?");

  Wire.begin(16, 17);
  ads1.begin();
  ads1.setGain(GAIN_ONE);
  Serial.println("The device is powered up");
  Serial.println("Initialized analog inputs");
}

void loop() {
  // Communication with Modem
  timer1 = millis();
  Serial2.println("AT");
  while (millis() < timer1 + 10000) {
    while (Serial2.available()) {
      int inByte = Serial2.read();
      Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while (millis() < timer1 + 10000) {
    while (Serial2.available()) {
      int inByte = Serial2.read();
      Serial.write(inByte);
    }
  }

  // Read from RS-485
  Serial1.println("RS-485 TX TEST");

  // Read from Modem and send to Serial Monitor
  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }

  // Read from Serial Monitor and send to Modem
  while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  // Digital Input Test
  Serial.print("I1: ");
  Serial.println(digitalRead(35));
  Serial.print("I2: ");
  Serial.println(digitalRead(34));

  delay(800);

  // Analog Input Reading
  printanalog();

  delay(800);
}

int readBattery() {
  unsigned int analog_value;
  analog_value = analogRead(36);
  return analog_value;
}

void printanalog() {
  for (int i = 0; i < 4; i++) {
    adcString[i] = String(ads1.readADC_SingleEnded(i));
    adcString[i] = String(ads1.readADC_SingleEnded(i));
    delay(10);
    Serial.print("A" + String(i + 1) + ": ");
    Serial.print(adcString[i]);
    Serial.print("  ");
  }

  Serial.println("____________________________________");
}

void MODEM_RESET_CYC() {
  digitalWrite(MODEM_RESET, HIGH);
  delay(1000);
  digitalWrite(MODEM_RESET, LOW);
  delay(1000);
  digitalWrite(MODEM_RESET, HIGH);
}
