//#include <RH_ASK.h>
// #ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
//#endif
#include <SoftwareSerial.h>
SoftwareSerial HC12(2, 3);
//const int RELAY_PIN = 3;
int RELAY = 4;
uint8_t LED = 13;
uint8_t sig;
//RH_ASK driver(2000, 2, 4, 5);
// RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()

    //pinMode(RELAY_PIN, OUTPUT);
{
// #ifdef RH_HAVE_SERIAL
  Serial.begin(9600); // Debugging only
  HC12.begin(9600);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
// #endif
//   if (!driver.init())
// #ifdef RH_HAVE_SERIAL
//     Serial.println("init failed");
// #else
//     ;
// #endif
// }
}

void loop()
{
  // uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  // uint8_t buflen = sizeof(buf);
    while (HC12.available()){
      //Serial.write(HC12.available());
      sig = HC12.read();
      Serial.write(sig+'0');
      digitalWrite(LED, sig);
      digitalWrite(RELAY, !sig);
    }
    delay(500);
    HC12.write((uint8_t)sig);
  // sig = HC12.read();
  // Serial.write(sig+'0');
  // digitalWrite(LED, sig);
  // digitalWrite(RELAY, sig);
  // delay(5000);
  // HC12.write((uint8_t)sig);
  
  //while (HC12.available()){
    //sig = HC12.read();
    //Serial.write(sig);
  
  //}
  // if (driver.recv(buf, &buflen)) // Non-blocking
  // {
  //   // Message with a good checksum received, dump it.
  //   driver.printBuffer("Got:", buf, buflen);
  //   if (buf == "vkl")
  //   {
  //     digitalWrite(LED_BUILTIN, HIGH);
  //     delay(50);
  //     digitalWrite(LED_BUILTIN, LOW);
  //     delay(50);
  //   }
  //   if (buf == "vkl")
  //   {
  //     digitalWrite(LED_BUILTIN, HIGH);
  //     delay(50);
  //     digitalWrite(LED_BUILTIN, LOW);
  //     delay(50);
  //   }

  //   Serial.println();
  // }

}