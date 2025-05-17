# Barille Writter 

This library was created for a project for the course EECS2032. The project is about creating a braille writer device, which can receive and read English text, and translate it to the American braille language, using solenoids and a custom driver circuit to mimic bumps and dots of the braille language.

NOTE: Create an IRdecoder file in Arduino/libraries and add IRdecoder.h and IRdecoder.cpp to it
This branch attempts to improve the input system by using interrupts

The sketch that should be uploaded to an Arduino to mimic the final functionality is the translation.ino file.
It is needed to have all files in the translation folder in the same directory.

The demo video can be seen in this think https://www.youtube.com/watch?v=A-jqoaMqUTU.

# Video Demo
[![Watch the video](Reports/Images/circuit_driver.PNG)](https://www.youtube.com/watch?v=A-jqoaMqUTU)

## 🛠️ Technologies & Tools
![C++](https://img.shields.io/badge/C++-00599C?style=plastic&logo=c%2B%2B&logoColor=white)  
![Embedded C](https://img.shields.io/badge/Embedded%20C-00599C?style=plastic&logo=c&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=plastic&logo=arduino&logoColor=white)  
![Arduino IDE](https://img.shields.io/badge/Arduino_IDE-00979D?style=plastic&logo=arduino&logoColor=white)  
![Embedded Systems](https://img.shields.io/badge/Embedded-Systems-blue?style=plastic)  
![Arduino CLI](https://img.shields.io/badge/Arduino_CLI-00979D?style=plastic&logo=arduino&logoColor=white)
![Git CLI](https://img.shields.io/badge/Git-CLI-F05032?style=plastic&logo=git&logoColor=white)  
![Makefile](https://img.shields.io/badge/Makefile-Dev-blue?style=plastic)
![Vim](https://img.shields.io/badge/Vim-019733?style=plastic&logo=vim&logoColor=white)  
![Neovim](https://img.shields.io/badge/Neovim-57A143?style=plastic&logo=neovim&logoColor=white)

- Developed a custom **Arduino C++ library** to encapsulate the control logic for a circuit driver  
- Employed **Git CLI** and **Arduino CLI** for streamlined development and automation 
- Edited and tested code in **Vim/Neovim**, using serial monitor tools for debugging

## 🧱 Hardware Platform
- 🔌 **Microcontroller:** [Arduino Mega 2560](https://store.arduino.cc/products/arduino-mega-2560-rev3) (ATmega2560)  
- ⚙️ **Circuit Driver:** Custom actuator control circuit  
- 🧩 **Interface:** Digital I/O for control signaling and feedback  

## License
[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
