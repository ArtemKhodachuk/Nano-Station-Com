#include <SoftwareSerial.h>
#include <SPI.h> // Not actually used but needed to compile
#include <LiquidCrystal_I2C.h>
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

LiquidCrystal_I2C lcd(0x3F,16,2);
// #endif
uint8_t BUTTON = 5;
uint8_t LED = 13;
SoftwareSerial HC12(2, 3);
uint8_t sig;
// RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

void setup()
{
// #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);	  // Debugging only
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    HC12.begin(9600);
    // lcd.init();                     
    // lcd.backlight();
    // lcd.clear(); 
    // lcd.setCursor(0, 0);
    // lcd.print("Station 1:");
    oled.init();        
    oled.clear();       
    oled.setScale(2);   
    oled.home();        
    oled.print("Station 1:");
// #endif
//     if (!driver.init())
// #ifdef RH_HAVE_SERIAL
//          Serial.println("init failed");
// #else
// 	;
// #endif
}

void loop()
{
    HC12.write((uint8_t)(!digitalRead(BUTTON)));
    delay(500);
    Serial.write(HC12.available());
    
    while (HC12.available()){
        sig = HC12.read();
        if (sig != 1 && sig !=0){
            // digitalWrite(LED, 1);
            // delay(1000);
            // digitalWrite(LED, 0);
            // lcd.setCursor(10, 0);
            // lcd.print("No con");

            oled.setCursor(10, 2);
            oled.print("No con");
        } else {
            //digitalWrite(LED, sig);
            //digitalWrite(LED, !digitalRead(BUTTON));
            if (sig) {
                // lcd.setCursor(10, 0);
                // lcd.print("ON    ");
                //digitalWrite(LED, 1);
                oled.setCursor(10, 2);
                oled.print("ON    ");
            } else {
                // lcd.setCursor(10, 0);
                // lcd.print("OFF   ");
                //digitalWrite(LED, 0);
                oled.setCursor(10, 2);
                oled.print("OFF   ");
            } 
        }
            // Serial.write(sig+'0');   
    }
//     HC12.write(2+'0');
//     delay(500);
//     HC12.write(1+'0');
 }



// #include <SPI.h> // Not actually used but needed to compile
// #include <Wire.h>        /*include Wire.h library*/
// void setup()
// {
//   Wire.begin();           /*Wire I2C communication START*/
//   Serial.begin(9600);    /*baud rate set for Serial Communication*/
//   while (!Serial);       /*Waiting for Serial output on Serial Monitor*/
//   Serial.println("\nI2C Scanner");
// }
// void loop()
// {
//   byte err, adr;       /*variable error is defined with address of I2C*/
//   int number_of_devices;
//   Serial.println("Scanning.");
//   number_of_devices = 0;
//   for (adr = 1; adr < 127; adr++ )
//   {
//     Wire.beginTransmission(adr);
//     err = Wire.endTransmission();

//     if (err == 0)
//     {
//       Serial.print("I2C device at address 0x");
//       if (adr < 16)
//         Serial.print("0");
//       Serial.print(adr, HEX);
//       Serial.println("  !");
//       number_of_devices++;
//     }
//     else if (err == 4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (adr < 16)
//         Serial.print("0");
//       Serial.println(adr, HEX);
//     }
//   }
//   if (number_of_devices == 0)
//     Serial.println("No I2C devices attached\n");
//   else
//     Serial.println("done\n");
//   delay(5000);             /*wait 5 seconds for the next I2C scan*/
// }