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
const int msglength = 32;
char recievemsg[msglength];

char resmsg[8] = {'A', 'B', 'C', '2', '0', 'D', 'E', 'F'};
//RH_ASK driver(2000, 2, 4, 5);
// RH_ASK driver;
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS
//https://forum.arduino.cc/t/serial-input-basics-updated/382007/2
void setup()

    //pinMode(RELAY_PIN, OUTPUT);
{
// #ifdef RH_HAVE_SERIAL
  Serial.begin(9600); // Debugging only
  HC12.begin(9600);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, 1);
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
  //Serial.write("start recieve");
  while(HC12.available() > 0){
    rc = HC12.read();
    Serial.write(rc);
    if (recieveStage == 0 && cidx == 0 && rc == startstr[0]){ //Start check
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 0 && cidx == 1 && rc == startstr[1]){
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 0 && cidx == 2 && rc == startstr[2]){
      recievemsg[cidx] = rc;
      cidx++;
      recieveStage++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 1 && cidx == 3 && rc == '0'){  //Sender id check
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 1 && cidx == 4 && (rc == '0' || rc == '1')){ // Toggle switch check
      recievemsg[cidx] = rc;
      cidx++;
      recieveStage++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 2 && cidx == 5 && rc == endstr[0]){ //End check 
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 2 && cidx == 6 && rc == endstr[1]){ 
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      delay(10);
    } else if (recieveStage == 2 && cidx == 7 && rc == endstr[2]){ 
      recievemsg[cidx] = rc;
      cidx++;
      //Serial.write("1 char\n");
      //Serial.write("1 char\n");
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
  // uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  // uint8_t buflen = sizeof(buf);

  
    // while (HC12.available()){
    //   //Serial.write(HC12.available());
    //   sig = HC12.read();
    //   Serial.write(sig+'0');
    //   digitalWrite(LED, sig);
    //   digitalWrite(RELAY, !sig);
    // }
    // delay(500);
    // HC12.write((uint8_t)sig);
    //Serial.write("start of loop");
  if (recievemsg_f() == 1){
    if (recievemsg[4] == '0'){
      digitalWrite(LED, 0);
      digitalWrite(RELAY, 1);
      resmsg[4] = '0';
      delay(250);
      sendmsg_f();
    } else {
      digitalWrite(LED, 1);
      digitalWrite(RELAY, 0);
      resmsg[4] = '1';
      delay(250);
      sendmsg_f();
    }
  }
  
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