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
![Schematic](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/Angle%26Ana%26Sw.jpg)  

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
  The interface use an **I2C** bus.  
  
  A dashboard exemple for two motors:  
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw/shottle_Motors.jpg" border="0"/></td>
  </tr>
  </table>
  
  * 16 Switchs board (need one protportional channel) 
  Each switch can command 1 fonction.  
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch/MultiSwitch_Mcp23017_MiniSwitch_3dTop.jpg" border="0"/></td>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch/MultiSwitch_Mcp23017_MiniSwitch_3dBottom.jpg" border="0"/></td>
  </tr>
  </table>  
...