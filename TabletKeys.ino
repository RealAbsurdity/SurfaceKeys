/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/*
  This example shows how to send HID (keyboard/mouse/etc) data via BLE
  Note that not all devices support BLE keyboard! BLE Keyboard != Bluetooth Keyboard
*/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitConfig.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_RESET);

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         0
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
/*=========================================================================*/


// Create the bluefruit object
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* DEFINE YOUR PINS */
const int button1 = 18;      //(A0)18
const int button2 = 19;      //(A1)19
const int button3 = 20;      //(A2)20
const int button4 = 21;      //(A3)21
const int button5 = 22;      //(A4)22
const int toggleButton = 23; //(A5)23
/* DEFINE YOUR PINS */

/* DEFINE OTHER VARIABLES */
int buttonMode = 1;
bool screenState = true;
unsigned long battTimer = 0;
/* DEFINE OTHER VARIABLES */

void setup(void){
/* 
 *  SETUP AND INITIALIZE THE DISPLAY 
 */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.setRotation(0); //SET THE ROTATION, STANDARD IS 0, FOR VERTICAL DISPLAY MAKE IT 1, 2 ROTATES 180 deg, 3 ROTATES 270deg
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
/* 
 *  INITIALIZE YOUR PINS 
 */
  // initilize pins 18-22 as INPUT
  for(int i=18; i<23; i++){
    pinMode(i, INPUT);
  }  
/* 
 *  INITIALIZE THE BLE on FEATHER BOARD
 *  
 */
  //begin
  ble.begin(true);
  //check for factory reset
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    ble.factoryReset();
  }
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  /* Get BLE Info */
  ble.info();
  /* Change the device name to make it easier to find */
  ble.sendCommandCheckOK(F("AT+GAPDEVNAME=BLE Hotkeys"));  
  delay(500);  
  /* Enable HID Service */
  ble.sendCommandCheckOK(F("AT+BleHIDEn=On"));    
  ble.sendCommandCheckOK(F("AT+BleKeyboardEn=On"));
  delay(500);  
  ble.reset();
  display.display();
/*
 *SET VERTICAL ROTATION AND DRAW THE BUTTONS 
 */
  display.setRotation(1);
  drawBatt();
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void){

  if (digitalRead(button1) == HIGH) {             //Button1 Action (static)     
    while (digitalRead(button1) == HIGH) {
      ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00-2C-00-00-00-00");
    }
    delay(50);
    ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
    }

  if (digitalRead(button2) == HIGH) {             //Button2 Action (static)
    int reading = button2;    
    doRmb(reading);
    }

  if (digitalRead(button3) == HIGH) {             //Button3 Action(s) (case dependent)      
    while (digitalRead(button3) == HIGH) {
      if (buttonMode == 1) {
        doShift();
      }
      if (buttonMode == 2) {
        doCopy();
      }
      if (buttonMode == 3) {
        doSlctall();
      }
    }
    delay(50);
    ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  }
  
  if (digitalRead(button4) == HIGH) {             //Button4 Action(s) (case dependent)      
    while (digitalRead(button4) == HIGH) {
      if (buttonMode == 1) {
        doCtrl();
      }
      if (buttonMode == 2) {
        doCut();
      }
      if (buttonMode == 3) {
        doDeslct();
      }
    }
    delay(50);
    ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  }
  
  if (digitalRead(button5) == HIGH) {             //Button5 Action(s) (case dependent)      
    while (digitalRead(button5) == HIGH) {
      if (buttonMode == 1) {
        doAlt();
      }
      if (buttonMode == 2) {
        doPaste();
        }
      if (buttonMode == 3) {
        doNewdoc();
      }
    }
  }

  if (digitalRead(toggleButton) == HIGH) {        //If the button is pushed go in here
    unsigned long start = millis();
    int buttonState = digitalRead(toggleButton);
    unsigned long timer = 0;
    while (buttonState == 1 && timer < 4000) {
      delay(10);
      timer = millis() - start;
      buttonState = digitalRead(toggleButton);
    }
    if (timer > 2000) {
      screenToggle();
      delay(50);
    }
    else if (timer > 100 && timer < 500) {
      modeToggle();                                             //toggle the mode
    }
    else {
    }
  }

  if(battTimer>0){
    unsigned long battTimeout = millis()-battTimer;
    if(battTimeout > 5000){
      screenToggle();
    }
  }
}

/*
 * Button Push Codes
 * AT+BLEKEYBOARDCODE
 *Sends a raw hex sequence of USB HID keycodes to the BLE keyboard interface including key modifiers and up to six alpha-numeric characters.
 *This command accepts the following ascii-encoded HEX payload, matching the way HID over GATT sends keyboard data:
 *Byte 0: Modifier
 *Byte 1: Reserved (should always be 00)
 *Bytes 2..7: Hexadecimal values for ASCII-encoded characters (if no character is used you can enter '00' or leave trailing characters empty)
 *After a keycode sequence is sent with the AT+BLEKEYBOARDCODE command, you must send a second AT+BLEKEYBOARDCODE command with at least two 00 characters to indicate the keys were released!
 *Modifier Values
 *The modifier byte can have one or more of the following bits set:
 *Bit 0 (0x01): Left Control
 *Bit 1 (0x02): Left Shift
 *Bit 2 (0x04): Left Alt
 *Bit 3 (0x08): Left Window
 *Bit 4 (0x10): Right Control
 *Bit 5 (0x20): Right Shift
 *Bit 6 (0x40): Right Alt
 *Bit 7 (0x80): Right Window
 *
 *0x04  Keyboard a and A
 *0x06  Keyboard c and C
 *0x07  Keyboard d and D
 *0x11  Keyboard n and N
 *0x19  Keyboard v and V
 *0x1B  Keyboard x and X
 *0x2C  Keyboard Spacebar
 // send 'abc' with shift key --> 'ABC'
 *ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-04-05-06-00-00");
 // Indicate that the keys were released (mandatory!)
 *ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
 * 
 */

void doRmb(int readPin) {
  while(digitalRead(readPin) == HIGH){
    unsigned long timer = millis();
    }
    delay(50);
  ble.sendCommandCheckOK("AT+BleHidMouseButton=R,click");
  delay(50);  
}

void doShift() {                        
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=02-00-00-00-00-00-00");
}

void doCtrl() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-00-00-00-00-00");
}

void doAlt() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=04-00-00-00-00-00-00");
}

void doCopy() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-06-00-00-00-00");
  delay(100);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(500);  
}

void doCut() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-1B-00-00-00-00");
  delay(100);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(500);  
}

void doPaste() {  
  while (digitalRead(button5) == HIGH){
    unsigned long timer = millis();    
    }
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-19-00-00-00-00");
  delay(10);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(10);  
}

void doSlctall() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-04-00-00-00-00");
  delay(100);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(200);  
}

void doDeslct() {  
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-07-00-00-00-00");
  delay(100);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(200);  
}

void doNewdoc() {
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=01-00-11-00-00-00-00");
  delay(100);
  ble.sendCommandCheckOK("AT+BLEKEYBOARDCODE=00-00");
  delay(500);  
}

/*
 * FUNCTIONS TO WRITE THE SCREEN AND TOGGLE MODES
 */
void drawBatt(){                                           //function to draw batt screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  float battRead=analogRead(A9);
  battRead *= 2;
  battRead *= 3.3;
  battRead /= 1024;
  float battPercent=(battRead-3)/.7*100; ///3.7*100//
  uint16_t battY=110-battPercent;
  display.setCursor(0,111);
  display.println(" Batt");
  display.println(battPercent);
  display.drawRoundRect(10,0,10,10,3,WHITE);
  display.drawRoundRect(0,9,32,100,3,WHITE);
  display.fillRoundRect(0,battY,32,battPercent,3,WHITE);
  display.display();
  battTimer = millis();
}

void drawKeys() {                                   //fucntion to draw keys screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  uint16_t x=0;
  uint16_t height=22;
  int buttonCount=5; 
  for (uint16_t y=0;y<115;y+=23){
    display.drawRoundRect(x,y,32,height,3,WHITE);
    uint16_t circleY=y+4;
    uint16_t cursorLine=y+10;
    display.setCursor(4,cursorLine);
    if (buttonCount==5){
      for(uint16_t c3 = 8; c3<25; c3+=8){
          display.fillCircle(c3, circleY, 2, WHITE);
          }
          if(buttonMode == 1){
            display.println(F("ALT"));
          }
          else if(buttonMode == 2){
            display.println(F("Pste"));
          }
          else if(buttonMode == 3){
            display.println(F("New"));
          }
        }
        else if(buttonCount==4){
          for(uint16_t c2 = 11; c2<25; c2+=11){
          display.fillCircle(c2, circleY, 2, WHITE);
          }
          if(buttonMode == 1){
            display.println(F("CTRL"));  
          }
          else if(buttonMode == 2){
            display.println(F("Cut"));   
          }
          else if(buttonMode == 3){
            display.println(F("Dsel"));  
          }
        }
        else if(buttonCount==3){
          display.fillCircle(16, circleY, 2, WHITE);
          if(buttonMode == 1){
            display.println(F("SHFT"));  
          }
          else if(buttonMode == 2){
            display.println(F("Copy"));   
          }
          else if(buttonMode == 3){
            display.println(F("SelA"));  
          }
        }
        else if(buttonCount==2){
          display.println(F("RMB"));
        }
        else if(buttonCount==1){
          display.println(F("SPC"));
        }
    buttonCount = buttonCount-1;
  }
  display.setCursor(0,120);
  if(buttonMode == 1){
    display.println(F("Mode1"));
  }
  else if(buttonMode ==2){
    display.println(F("Mode2"));
  }
  else if(buttonMode ==3){
    display.println(F("Mode3"));
  }
  display.display();   
}
    
void modeToggle() {          //switch case to toggle mode variable and display status
  switch (buttonMode) {
    case 1:
      //mode2();
      buttonMode = 2;
      drawKeys();       
      delay(100);
      break;
    case 2:
      //mode3();
      buttonMode = 3;
      drawKeys();       
      delay(100);
      break;
    case 3:
      //mode1();
      buttonMode = 1;
      drawKeys();   
      delay(100);
      break;
  }
}

void screenToggle() {               //function to toggle screen mode between batt read and cribsheet
  if(screenState == true) {
    screenState = false;
    drawBatt();
  }
  else if(screenState == false) {
    screenState = true;
    battTimer = 0;
    drawKeys();
  }
  else{
  }
}

