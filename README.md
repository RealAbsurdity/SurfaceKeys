# TabletKeys
Arduino/BTle based hotkeys to enhance usability of tablet devices in drawing and art applications

This software relies on the Arduino platform and software including SPI.h and WIRE.h and utilizes the BluefruitLE, OLED, and AdafruitGFX libraries from Adafruit.

https://github.com/arduino/Arduino 

https://github.com/adafruit/Adafruit_BluefruitLE_nRF51

https://github.com/adafruit/Adafruit_SSD1306 

It is designed for the Feather Bluefruit and 128x32 monochome OLED (I2C) board from Adafruit:

https://github.com/adafruit/Adafruit-128x32-I2C-OLED-Breakout-PCB

https://github.com/adafruit/Adafruit-128x64-Monochrome-OLED-PCB 

For photos of the project as it is currently built see the ProjectPhotos folder in the repository.

This software is designed to use Adafruit BluefuitLE (specifically the Feather Bluefruit harware) with 6 digtial momentary contant button inputs and a monochrome OLED display to provide basic hotkey input for drawing and other creative appliations without the use of a full keyboard. The current set of hotkeys are designed for input to Photoshop and other applications that utilize general modifiers like Shift, Sapce, Ctrl, Alt, RMB, Ctrl+A, Ctrl+D, Ctrl+C Ctrl+X, Ctrl+V. 

The design provdes five hotkeys total. Two inputs (button1 and button2) are set for static functions Space and RMB respectively. The next three inputs (button3 button4 and button5) are toggled between three modes; Mode1: Shift, Ctrl Alt; Mode2: Copy, Cut, Paste; Mode3: SelectAll, DeSelect, NewDoc; respectively. The final button is reserved for changing mode and checking the level of the battery. Long Key Presses (3sec or more) of the mode button are interpreted to show the battery and short presses simply toggle the modes for the three swappable hotkeys.
 
The oled display shows the current mode and hotkey settings for each of the customizble keys for quick visual reference of the current output from the device. The keystrokes are sent over BTLE 

Hardware Description:
The device is based on the Adafruit Feather Bluefruit Board. It integrates Arduino compatibility with bluetooth connectivity. Other useful features of the board utilized here include On/Off via a toggle switch tied to the En pin and ground, Battery Charging and Power via a 3.3v lipo battery and the microUSB port on the board, digital inputs (A0 - A5) for the six keys and I2C interface with the 128x32 monochrome OLED display. 

The momentary contact switchs use a standard pulldown configuration. The buttons use an external pulldown resistor (10KOhm) tied to ground with the other two opposite legs tied to +3.3v and their input pin. 

UPDATE HISTORY:
1/27/2017 Repository created. The repo only contians the source code for the project and will be updated regularly with circuit diagrams and other documentation as time allows.

