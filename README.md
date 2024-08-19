# BURC_Encoder
 Universal RC Box System.  
 
 The BURC modules use the RCUL protocol by Rc-Navy.
 ![RCUL](https://github.com/pierrotm777/BURC_Encoder/blob/main/RCUL.jpg)  

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
- [Servo 360°]() 


## Build BURC
### Schematic  
Encoder:  
![Encoder](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw.jpg)  
16 switchs by I2C bus:  
![16 Switchs](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch.jpg)  
16 switchs on Arduino:  
![16 Switchs](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/8_16-Simple_Encoder.png) 

### Boards  
Two cards make up the BURC system.
The BURC Encoder is connected to the transmitter over a **CPPM** input or a **SBUS** input.
- The BURC Encoder:
  * Encoder board (include 8 switchs, 1 360° hall sensor and 1 slider, need one proportional channel) 
  Can command up to 128 fonctions (8x16) by exemple 8 Ms16 modules, use 1 channel for each 16 switchs module.  

  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw_Top.jpg" border="0"/></td>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/Angle%26Ana%26Sw_Bottom.jpg" border="0"/></td>
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
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/shottle_Motors.jpg" border="0"/></td>
  </tr>
  </table>
    
  * Encoder symplified board (include 16 switchs or 8 switchs and 6 potentiometers, need one proportional channel) 
  Can command up to 128 fonctions (8x16) by exemple 8 Ms16 modules, use 1 channel for each 16 switchs module.  

  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/8_16-Simple_Encoder_Top.jpg" border="0"/></td>
  </tr>
  </table>
  
  You need to remove the resistor's LED on pin D13 (C10 signal).  
  ![R to remove](https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/R_To_Remove.jpg) 
  
 - The Switchs board: 
  16 Switchs board (need one protportional channel) 
  Each switch can command 1 fonction.  
  The type of command depend of the [Ms8](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/MutltiSwitch_Sw8_V2/MS8-Xany-V2_Manuel_Utilisateur.pdf)/[Ms16](https://github.com/Ingwie/OpenAVRc_Hw/blob/V3/MultiSwitch_Sw16-ProMicro/MS16-Xany_Manuel_Utilisateur.pdf) possibilities.    
  This board read 16 switchs by 1 MCP23017 or 2 PCF8574 or 1 PCF8575.
  <table border="2">
  <tr>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch_3dTop.jpg" border="0"/></td>
  <td><img src="https://github.com/pierrotm777/BURC_Encoder/blob/main/BURC_For_All_Transmitters/MultiSwitch_Mcp23017_MiniSwitch_3dBottom.jpg" border="0"/></td>
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

  
### Firmware upload for Pro Mini
- Windows:  
The best method is to use [Xloader](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/XLoader.zip).  
The last firmware is the version [v0.14](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/BURC_CODER_V0_14.hex).  

![Xloader](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/Xloader.jpg)  

- Other Os:
Use you Arduino IDE for find witch command to use.
In my case, the command is :  
![](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/ide_cmd.jpg)  
  * replace the hex file path C:\Users\pierrot\AppData\Local\Temp\arduino_build_879913/Blink.ino.hex by your firmware path.  
  * create a script file with this change.  

### Firmware upload for Attiny85
  * use an USBAsp programmer and [ARDUDESS Gui](https://github.com/ZakKemble/AVRDUDESS/releases/) or an [Arduino Uno board](https://www.instructables.com/Programming-ATtinys-Micro-Controllers-With-Arduino)
  * define the fuses (for 16Mhz internal mode), low=0xF1, high=0xDF, extended=0xFF
  * last firmware is the version [v2.2](https://github.com/pierrotm777/BURC_Encoder/blob/main/PROG/Windows/HallAna2A1335_V2_2.hex).  

### Exemple BURC build
- You will find [here](https://cults3d.com/fr/mod%C3%A8le-3d/jeu/burc-pad) an exemple of 3d files by **croky_b** witch accept all the BURC's boards.  

## Configure the BURC Encoder
  * Pro Mini:
    * Connect a FTDI interface to the Arduino Pro Mini and your PC.
    * Use a terminal for send your commands.
    * Set your terminal with 115200 bauds, 8 bits, no parity, 1 bit stop and CR or CRLF (see LIGNE.TERM).
    * Type ENTER key for see the module's version.

  * Attiny85 (360° encoder) A push button allows:
    * calibrating the analog sensor (min and max): before powering the ATtiny85, press the Push button and hold it for  1 - 2 seconds.
    * setting the I2C address: before powering the ATtiny85, press the Push button and hold it for more than 4 seconds.  
      Additionally, it can read an analogic input (0/5V)

## Settings usables
The list of messages that can be transported in each RCUL instance is:  
- SW1-SW8:   the state of 8 contacts of the 8 contacts of an 8 I/O I/O extender or a 16 I/O extender or the first 8 contacts of the 16 native contacts of the Arduino
- SW9-SW16:  the state of the last 8 contacts of a 16 I/O extender or the last 8 contacts of the 16 native contacts of the Arduino
- SW1-SW16:  the state of 16 contacts of a 16 I/O extender or of the 16 native contacts of the Arduino
- ANGLE:     the value of the angle of a 360° angle sensor
- ANGLE+ANA: the value of the angle of a 360° angular sensor + an analog value  

On the encoder there are 16 native contacts from C1 to C16.  
Among C8 to C16, 6 are also analog inputs which correspond to pins C11 to C16.  
So C11 to C16 are also ANA1 to ANA6 (these are my names, not the names of the "arduino" pins):  
We therefore have 6 analog channels at our disposal (just as we wire a pot there instead of an switch or push button).  

We can make extended RCUL messages of the type:  
**RCUL1.MESSAGE=C1-C8@0x00+ANA1** (which will transmit 8 contact states + an 8-bit ana value:  
You obviously have to wire a knob to pin C11).  
**RCUL2.MESSAGE=C9-C16@0x24+ANA2** (which will transmit 16 contact states + an 8-bit ana value:  
You obviously have to wire a knob to pin C12).  

So, with **RCUL1.MESSAGE=C1-C8@0x00+ANA1** or **RCUL2.MESSAGE=C9-C16@0x24+ANA2**,  
for an MS8 (V1 or V2), you can control the 8 outputs + the additional proportional output.  
You can also order the Sound&Smoke module which uses the 8 contacts to launch 8  
different sounds and the ana value to adjust the volume of the DF Player from the transmitter.  

We can also do:
**RCUL3.MESSAGE=C1-C16@0x25+ANA3** (which will transmit 16 contact states + an 8-bit analog value:  
You obviously have to wire a knob to pin C12)
So, with **RCUL3.MESSAGE=C1-C16@0x25+ANA3** for an MS16, you can control the 16 outputs + the additional proportional output.  

So, if we want to use the first 8 contacts C1 to C8 of your 16-input I2C extender at address 0x24,  
we just have to create another RCUL instance in which we refer to the first 8 contacts: **RCUL5.MESSAGE=C1-C8@0x24**
And with RCUL5 we can, for example, order another MS8.
So we will have the first 8 contacts of the I2C extender with 16 inputs (at address 0x24) used by RCUL5 to control an MS8 (for example) 
and the last 8 contacts of the 16-input I2C extender (at address 0x24) used by RCUL2 (which also uses ANA2) to control a Sound&Smoke.

It is possible to directly map an analog value (ANA1 to AN6), inverted or not, to a channel.  
To do this, you must use the command **RCULx.MESSAGE=RC.ANA[_INV]y** with x=RCUL instance number and y is the ANA channel number (from 1 to 6):  
Example: **RCUL4.MESSAGE=RC.ANA_INV1**  
A simple solution for build a little 6 channels hanset WITH BURC features.  

Debug method:  
To test an RCUL channel in CPPM, we loop pin8 back to pin9 and we do an **RX.MODE=RCUL4@CPPM4** and after an **RX.DBG=1** command.  


Exemple of configuration:
```
CONF?
LANG=FR
LIGNE.TERM=CRLF
ECOLAGE.MODE=CPPM_OUT
CPPM.MODU=NEG
CPPM.ENTETE=300
CPPM.VOIE.NB=8
CPPM.PERIODE=22500
RCUL.REPET=2
RCUL1.VOIE=5
RCUL1.MESSAGE=ANGLE+ANA@0x0C
RCUL1.TEST=OFF
RCUL2.VOIE=6
RCUL2.MESSAGE=C9-C16@0x24
RCUL2.TEST=OFF
RCUL3.VOIE=8
RCUL3.MESSAGE=C1-C8@0x24+ANA1
RCUL3.TEST=OFF
RCUL4.VOIE=7
RCUL4.MESSAGE=RC.ANA_INV1
RCUL4.TEST=OFF
RCUL5.VOIE=OFF
RCUL5.MESSAGE=C1-C8@0x00
RCUL5.TEST=OFF
RCUL6.VOIE=OFF
RCUL6.MESSAGE=C1-C8@0x00
RCUL6.TEST=OFF
RX.MODE=RCUL6@CPPM6
RX.DBG=0
```

<pre>
- LANG=FR                       define language (FR or UK)  
- LIGNE.TERM=CRLF               define terminal return (CR or CRLF)  
- ECOLAGE.MODE=CPPM_OUT         CPPM_OUT+CPPM_IN/CPPM_OUT/SBUS_OUT;FUTABA[_INV] Defines the transmitter training mode
- CPPM.MODU=NEG                 CPPM signal is build with negative pulses (POS is possible)  
- CPPM.ENTETE=300               300 first width CPPM pulse  
- CPPM.VOIE.NB=8                CPPM build 8 channels  
- CPPM.PERIODE=22500            CPPM periode in mS
- SBUS.SPEED=NORMAL				NORMAL/FAST Defines the SBUS speed (NORMAL=14ms, FAST=7ms)  
- RCUL.REPET=2                  the encoder repeats the same thing twice for a better transmission result  
- RCUL1.MESSAGE=ANGLE+ANA@0x0C  for command an azimutal motor (direction and motor)  
- RCUL2.MESSAGE=C9-C16@0x24     8 buttons on MCP23017 (address 0x24)  
- RCUL3.MESSAGE=C1-C8@0x24+ANA1 8 buttons on MCP23017 and analog button (module Sound&Smoke for 8 sounds et volume)
- RCUL4.MESSAGE=RC.ANA_INV2     1 analogique channel inverted  
</pre>