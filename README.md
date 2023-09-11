# BURC_Encoder
 Universal RC Box System

## Rc-Navy
The BURC system is build by Philippe Loussouarn ([Rc-Navy](http://p.loussouarn.free.fr/index.html)). 

## Presentation of the BURC system
[What's BURC](https://p-loussouarn-free-fr.translate.goog/arduino/exemple/RCUL/RCUL.html?_x_tr_sch=http&_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=en)

## Compatibles modules usable with BURC
All these modules behind
- [Ms8 v2](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/MutltiSwitch_Sw8_V2)  
- [Ms16](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/MultiSwitch_Sw16-ProMicro)  
- [Sound&SmokeModule](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Sound%26SmokeModule)  
- [Xany2Msx](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Xany2Msx/Firmware_Msx)  
- [Xany2Misc](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Xany2Msx/Firmware_Misc)  
- Hall sensor:  
  * [Hall I2C sensor](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Capteur_Hall_I2C)  
  * [Hall I2C sensor mini](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/Capteur_Hall_I2C_Mini)  
- [MultiSwitch_MosFet](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/MultiSwitch_MosFet)  
- [Ms8 Pulseq](https://github.com/Ingwie/OpenAVRc_Hw/tree/V3/PulseSeq)  


## Build BURC
### Schematic  
Encoder:  
![Encoder](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/Angle%26Ana%26Sw.jpg)  
16 switchs:  
![16 Switchs](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch/MultiSwitch_Mcp23017_MiniSwitch.jpg)  

### Boards  
Two cards make up the BURC system.
The BURC Encoder is connected to the transmitter over a **CPPM** input or a **SBUS** input.
- The BURC Encoder:
  * Encoder board (include 8 switchs, 1 360° hall sensor and 1 slider, need one proportional channel) 
  Can command up to 128 fonctions (8x16) by exemple 8 Ms16 modules, use 1 channel for each 16 switchs module.  

  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/Angle%26Ana%26Sw_Top.jpg" border="0"/></td>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/Angle%26Ana%26Sw_Bottom.jpg" border="0"/></td>
  </tr>
  </table>
  
  The hall sensor command a Schottel motor type by rotation, and the slider command the motor's power.  
  This board is the **main board** system and can command **several 16 switchs** board.  
  The interface between all the boards use an **I2C** bus.  

  The same board can be used for the left or right motor, and assume to wire only one of the two hall sensor an the 8 first switchs.  
  The second board will command the 8 other switchs and the second slider (No need another arduino Pro Mini in the second circuit board) .  
  An Attiny85 receive these orders from the first board and command the second hall sensor and the second slider.  
  A dashboard exemple for two motors:  
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/shottle_Motors.jpg" border="0"/></td>
  </tr>
  </table>
  
  * 16 Switchs board (need one protportional channel) 
  Each switch can command 1 fonction.  
  The type of command depend of the [Ms8](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/MutltiSwitch_Sw8_V2/MS8-Xany-V2_Manuel_Utilisateur.pdf)/[Ms16](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/MultiSwitch_Sw16-ProMicro/MS16-Xany_Manuel_Utilisateur.pdf) possibilities.    
  This board read 16 switchs by 1 MCP23017 or 2 PCF8574 or 1 PCF8575.
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch/MultiSwitch_Mcp23017_MiniSwitch_3dTop.jpg" border="0"/></td>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch/MultiSwitch_Mcp23017_MiniSwitch_3dBottom.jpg" border="0"/></td>
  </tr>
  </table>  
  
  A main board coupled to  a second board and 2 16 switchs boards:
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/2_Encoder&32Switchs.jpg" border="0"/></td>
  </tr>
  </table>  
  
  A main board and a 48 switchs coupled:
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/Encoder%2648Switchs.jpg" border="0"/></td>
  </tr>
  </table>  
  
### Firmware upload
- Windows:  
The best method is to use [Xloader](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/XLoader.zip).  
The last firmware is the version [0.6](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/BURC_CODER_V0_6.hex).  
![Xloader](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/Xloader.jpg)  

- Other Os:
Use you Arduino IDE for find witch command to use.
In my case, the command is :  
![](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/ide_cmd.jpg)  
C:\Users\pierrot\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/bin/avrdude -CC:\Users\pierrot\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf -v -patmega328p -carduino -PCOM5 -b57600 -D -Uflash:w:C:\Users\pierrot\AppData\Local\Temp\arduino_build_879913/Blink.ino.hex:i  
  * replace the hex file path C:\Users\pierrot\AppData\Local\Temp\arduino_build_879913/Blink.ino.hex by your firmware path.  
  * select the good device port.  
  * create a script file with these two modofications.  


...