ESP8266 Temperature Sensor Prototype
====================================

![alt text](https://raw.githubusercontent.com/ShawnHymel/esp8266-temp-proto/master/Images/esp8266-throwie-programming.jpg)

This is a quick prototpye designed for the Bantam Tools Desktop PCB Milling Machine. It uses an ESP8266-WROOM-02 module connected to an MCP9808 temperature sensor. It will run off a single-cell LiPo battery, but there is no protection, undervoltage detection, or charge circuitry onboard. The idea is simple: read from temperature sensor and post to a server at regular intervals. It should work as a simple, IoT Temperature Sensor Throwie.

To Build
--------

Gerbers are found in the *Hardware/gerbers* folder. Mill them or send them off to a PCB fab house. If you want solder mask and silk screen, you'll have to run your own Gerbers from within KiCad. Also, I didn't move the REFDES labels around to make them look nice (since I was making this board on a mill). You'll probably want to do that, too.

Bill of Materials (BOM) is *Hardware/esp8266-temp-proto_BOM.xlsx*. Buy stuff listed in that. Assemble board.

To Program ESP8266
------------------

Connect 3.3V USB-to-Serial adapter to header on board:

| USB-to-Serial | ESP8266 Board |
| ------------- | ------------- |
|     3.3 V     |     3.3 V     |
|      GND      |      GND      |
|       TX      |       RX      |
|       RX      |       TX      |

Press and hold both buttons. Release RESET button first for 1 second and then release PROG button. This will put the ESP8266 into programming/bootloader mode.

Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the Additional Boards Manager URLs in *File > Preferences in Arduino*. Install *esp8266 by ESP8266 Community* in Boards Manager.

Open *Firmware/esp8266-temp-get/esp8266-temp-get.ino* and change SSID and WiFi password to your network's credentials. Change `hostname` to your computer's IP address. Upload to ESP8266-WROOM-02.

To Run Server
-------------

Install Python (this was tested on Python 3.7). Run *Software/test-srever.py*. With your server running and ESP8266 powered, you should see temperature posted every ~10 seconds.

