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

const int msglength = 32;
char recievemsg[msglength];

bool skip_flag_1 = 1;

bool skip_flag_2 = 1;

bool recv_sig = 0;

int num_checks = 5;

char resmsg[8] = {'A', 'B', 'C', '0', '0', 'D', 'E', 'F'};
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
    oled.setCursor(0, 4);
    oled.print("Station 2:");
// #endif
//     if (!driver.init())
// #ifdef RH_HAVE_SERIAL
//          Serial.println("init failed");
// #else
// 	;
// #endif
}

bool strncmp(char* strA, char* strB, int n){

  for (int i = 0; i < n; i++){
    if (strA[i] != strB[i]){
      return 0;
    }
  }
  return 1;
}

bool recievemsg_f(){
  int recieveStage = 0; //start - 0, data - 1, end - 2
  int cidx = 0;
  char startstr[3] = {'A', 'B', 'C'};
  char endstr[3] = {'D', 'E', 'F'};
  char rc;
  // oled.setCursor(10, 4);
  // oled.print("Check");
  while(HC12.available() > 0){
    rc = HC12.read();
    // oled.setCursor(10, 4);
    // oled.print("Check1");
    // oled.print((char)rc);
    //delay(500);
    if (recieveStage == 0 && cidx == 0 && rc == startstr[0]){ //Start check
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
    } else if (recieveStage == 0 && cidx == 1 && rc == startstr[1]){
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
    } else if (recieveStage == 0 && cidx == 2 && rc == startstr[2]){
      recievemsg[cidx] = rc;
      cidx++;
      recieveStage++;
      delay(10);
    } else if (recieveStage == 1 && cidx == 3 && (rc == '1' || rc == '2')){  //Sender id check
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
    } else if (recieveStage == 1 && cidx == 4 && (rc == '0' || rc == '1')){ // Toggle switch check
      recievemsg[cidx] = rc;
      cidx++;
      recieveStage++;
      delay(10);
    } else if (recieveStage == 2 && cidx == 5 && rc == endstr[0]){ //End check 
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
    } else if (recieveStage == 2 && cidx == 6 && rc == endstr[1]){ 
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
    } else if (recieveStage == 2 && cidx == 7 && rc == endstr[2]){ 
      recievemsg[cidx] = rc;
      cidx++;
      delay(10);
      return 1;
    } else {
      return 0;
    }
  }
  return 0;

}

void sendmsg_f(){
  // for (int i = 0; i < 8; i++){
  //   HC12.write((uint8_t)resmsg[i]);
  // }
  HC12.print(resmsg);
}

void loop()
{
    // HC12.write((uint8_t)(!digitalRead(BUTTON)));
    // delay(500);
    // Serial.write(HC12.available());
    
    // while (HC12.available()){
    //     sig = HC12.read();
    //     if (sig != 1 && sig !=0){
    //         // digitalWrite(LED, 1);
    //         // delay(1000);
    //         // digitalWrite(LED, 0);
    //         // lcd.setCursor(10, 0);
    //         // lcd.print("No con");

    //         oled.setCursor(10, 2);
    //         oled.print("No con");
    //     } else {
    //         //digitalWrite(LED, sig);
    //         //digitalWrite(LED, !digitalRead(BUTTON));
    //         if (sig) {
    //             // lcd.setCursor(10, 0);
    //             // lcd.print("ON    ");
    //             //digitalWrite(LED, 1);
    //             oled.setCursor(10, 2);
    //             oled.print("ON    ");
    //         } else {
    //             // lcd.setCursor(10, 0);
    //             // lcd.print("OFF   ");
    //             //digitalWrite(LED, 0);
    //             oled.setCursor(10, 2);
    //             oled.print("OFF   ");
    //         } 
    //     }
    //         // Serial.write(sig+'0');   
    // }

//     HC12.write(2+'0');
//     delay(500);
//     HC12.write(1+'0');
    if ((!digitalRead(BUTTON)) == 1) {
        resmsg[4] = '1';
    } else {
        resmsg[4] = '0';
    }

    sendmsg_f();

    for (int i = 0; i < 5; i++){
        //  if (recievemsg_f() == 1 && recievemsg[3] == '1'){
        //      skip_flag_1 = 0;
        //      // oled.setCursor(10, 4);
        //      // oled.print("Check2");
        //      break;
        //  }
        //  skip_flag_1 = 1;
        //  delay(500);

        recv_sig = recievemsg_f();
        if (recievemsg[3] == '1' && recv_sig == 1){
          skip_flag_1 = 0;
        } else if (recievemsg[3] == '2' && recv_sig == 1){
          skip_flag_2 = 0;
        }
        delay(500);
     }

    // for (int i = 0; i < 5; i++){
    //     if (recievemsg_f() == 1 && recievemsg[5] == '2'){
    //         skip_flag_2 = 0;
    //         // oled.setCursor(10, 4);
    //         // oled.print("Check2");
    //         break;
    //     }
    //     skip_flag_2 = 1;
    //     delay(500);
    // }
    if (skip_flag_1 == 1){
        oled.setCursor(0, 2);
        oled.print("No con");
        // oled.setCursor(10, 4);
        // oled.print("Check3");
    } else {
        if (recievemsg[4] == '0'){
            oled.setCursor(0, 2);
            oled.print("OFF   ");
        } else if (recievemsg[4] == '1'){
            oled.setCursor(0, 2);
            oled.print("ON    ");
        } else {
            oled.setCursor(0, 2);
            oled.print("ERR   ");
            oled.print(recievemsg[4]);
        }
    }

    if (skip_flag_2 == 1){
        oled.setCursor(0, 6);
        oled.print("No con");
        // oled.setCursor(10, 4);
        // oled.print("Check3");
    } else {
        if (recievemsg[4] == '0'){
            oled.setCursor(0, 6);
            oled.print("OFF   ");
        } else if (recievemsg[4] == '1'){
            oled.setCursor(0, 6);
            oled.print("ON    ");
        } else {
            oled.setCursor(0, 6);
            oled.print("ERR   ");
            oled.print(recievemsg[4]);
        }
    }
    // oled.setCursor(10, 4);
    // oled.print("Check4");
    skip_flag_1 = 1;
    skip_flag_2 = 1;
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