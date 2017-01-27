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

This is a nearly complete but ongoing project to prototype a set of wireless hotkeys based on open source hardware and software. The instructions here are not yet complete and further documentation of the custom electronic components is still needed but it should provide most of the info required to build one of your own.
The software is designed to use Adafruit BluefuitLE (specifically the Feather Bluefruit harware) with 6 digital momentary contact button inputs and a monochrome OLED display to provide basic hotkey input for drawing and other creative applications without the use of a full keyboard. The current set of hotkeys are designed for input to Photoshop and other applications that utilize general modifiers like Shift, Space, Ctrl, Alt, RMB, Ctrl+A, Ctrl+D, Ctrl+C Ctrl+X, Ctrl+V.

The design provides five hotkeys total. Two inputs (button1 and button2) are set for static functions Space and RMB respectively. The next three inputs (button3 button4 and button5) are toggled between three modes; Mode1: Shift, Ctrl Alt; Mode2: Copy, Cut, Paste; Mode3: SelectAll, DeSelect, NewDoc; respectively. The final button is reserved for changing mode and checking the level of the battery. Long Key Presses (3sec or more) of the mode button are interpreted to show the battery and short presses simply toggle the modes for the three swap-able hotkeys.

The OLED display shows the current mode and hotkey settings for each of the customizable keys for quick visual reference of the current output from the device. The keystrokes are sent over BTLE

Hardware Description: The device is based on the Adafruit Feather Bluefruit Board. It integrates Arduino compatibility with bluetooth connectivity. Other useful features of the board utilized here include On/Off via a toggle switch tied to the En pin and ground, Battery Charging and Power via a 3.3v lipo battery and the microUSB port on the board, digital inputs (A0 - A5) for the six keys and I2C interface with the 128x32 monochrome OLED display.

The momentary contact switches use a standard pull-down configuration. The buttons use an external pull-down resistor (10KOhm) tied to ground with the other two opposite legs tied to +3.3v and their input pin. 

UPDATE HISTORY:
1/27/2017 Repository created. The repo only contians the source code for the project and will be updated regularly with circuit diagrams and other documentation as time allows.

