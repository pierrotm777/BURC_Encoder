// Good TinyWireS lib: https://github.com/svoisen/TinyWire
#include <TinySoftPwm.h>
#if defined(__AVR_ATtiny85__)
#include <TinyWireS.h>
#define TINY_WIRE_S_STOP_CHECK()  TinyWireS_stop_check()
#define SERIAL_PRINT(s)
#define SERIAL_PRINTLN(s)
#else
/* UNO */
#warning Compiled for UNO!
#include <Wire.h>
#define TinyWireS                 Wire
#define TINY_WIRE_S_STOP_CHECK()
#define send                      write
#define receive                   read
#define SERIAL_PRINT(s, ...)      Serial.print(s, ##__VA_ARGS__)
#define SERIAL_PRINTLN(s, ...)    Serial.println(s, ##__VA_ARGS__)
#endif
#include <EEPROM.h>

/*
 This sketch transforms an analogic Angular Hall effect sensor (such as the P3022-V1-CW360) into an I2C Angular Hall effect sensor
 compatible with an I2C 12 bit A1335 sensor: 0°->0, 360°->4095)
 A push button allows:
 - calibrating the analog sensor (min and max): before powering the ATtiny85, press the Push button and hold it for  1 - 2 seconds
 - setting the I2C address: before powering the ATtiny85, press the Push button and hold it for more than 4 seconds
 Additionally, it can read an analogic input (0/5V)
 
 ATtiny85:
 ========
  Phys Avr  Arduino
  Pin1 PB5  5   A0 RESET
  Pin2 PB3  3   A3 PWM
  Pin3 PB4  4   A2 PWM
  Pin4 GND  GND
  Pin5 PB0  0      PWM SDA
  Pin6 PB1  1      PWM
  Pin7 PB2  2   A1     SCL
  Pin8 VCC  VCC
                       P3022-V1-CW360
                       ANA HALL SENSOR
                           ___^___
                          /       \
                  .--------------o------o--------------------------.
                  |              |      |                          |
                  |          VCC |      |    ,-------._.-------.   |                      _
                  |              O      |  x-X1 PB5       VDD 8+---o----------------o +5V  \
                  |                     |    |                 |                            \
                  |                     |    |                 |                            |
                  |                     #    |                 |                            |
                  |                 POT #<--->2 PB3       PB2 7>--------------------o SCL   |
          100nF =====                   #    |       AVR       |                            |
                =====  OUT O-----.      |    |     ATtiny85    |                           /
                  |              |      |    |                 |                          | COMPATIBLE A1335
                  |              '----------->3 PB4       PB1 6>---.                       > I2C INTERFACE
                  |                     |    |                 |   |                      |
                  |                     |    |                 |   V RED LED               \
                  |                     |    |                 |   |                        |
                  |              O------o----+4 GND       PB0 5X------ o------------o SDA   |
                  |          GND |           '-----------------'   |   |                    |
                  |              |                             680 #    D PUSH BUTTON       |
                  |              |                                 |   |     CAL/@          /
                  '--------------o---------------------------------o---o------------o GND _/
                                                                           
*/

#define EEP_I2C_SLAVE_ADDR_OFFSET        0 // 1 byte
#define EEP_ANA_MIN                      1 // 2 bytes
#define EEP_ANA_MAX                      3 // 2 bytes

#if defined(__AVR_ATtiny85__)
#define PUSH_BUTTON_PIN                  0
#define LED_PIN                          1
#define ANA_ANGLE_PIN                    A2
#define ANA_INPUT_PIN                    A3
#else
/* UNO */
#define PUSH_BUTTON_PIN                  2
#define LED_PIN                          3
#define ANA_ANGLE_PIN                    A0
#define ANA_INPUT_PIN                    A1
#endif

#define ANGLE_CALIBRATION_TIME_MS        12000UL
#define I2C_ADDR_OFFSET_SETTING_TIME_MS  12000UL

#define BUTTON_RELEASED()                digitalRead(PUSH_BUTTON_PIN)
#define BUTTON_PRESSED()                 !BUTTON_RELEASED()

#define LED_OFF()                        digitalWrite(LED_PIN, LOW);
#define LED_ON()                         digitalWrite(LED_PIN, HIGH);
#define LED_FLASH()                      LED_ON();delay(30);LED_OFF()
#define GAMMA_DEPTH                      1

#define READ_ANA_10BIT_ANGLE()           analogRead(ANA_ANGLE_PIN)
#define READ_ANA_INPUT()                 analogRead(ANA_INPUT_PIN)
#define A1335_I2C_7B_ADDR                (0x0C + (uint8_t)EEPROM.read(EEP_I2C_SLAVE_ADDR_OFFSET))
#define A1335_ANG_REG_ADDR               (0x20)

// The default buffer size, Can't recall the scope of defines right now
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE               (16)
#endif

enum {ANA_10_BITS_RES =  0, ANA_11_BITS_RES, ANA_12_BITS_RES};
enum {CFG_INIT = 0, CFG_MEASURE_PUSH_DURATION_MS, CFG_CALIBRATE, CFG_CHANGE_I2C_ADDR, CFG_END};
enum {DISP_IDLE = 0, DISP_FLASH_START, DISP_FLASH_IN_PROGRESS, DISP_PAUSE, DISP_LED_START, DISP_LED_IN_PROGRESS, DISP_WAIT_FOR_2_SEC};

typedef struct{
  uint16_t AngleMin;
  uint16_t AngleMax;
}SensorSt_t;

static   SensorSt_t    Sensor;

volatile uint8_t i2c_regs[] =
{
    0, /* @ 0x20: MSB Angle */ 
    0, /* @ 0x21: LSB Angle */ 
    0, /* @ 0x22: Ana */ 
};

#define  I2C_REG_NB   sizeof(i2c_regs)

// Tracks the current register pointer position
volatile uint8_t  reg_idx  = 0;

volatile uint16_t A1335Angle   = 0;
volatile uint8_t  A1335Ana     = 0;
static   uint16_t CurrentAngle = 0;
static   uint8_t  DispState    = DISP_FLASH_START;

/* Private function prototypes */
static void      requestEvent(void);
static void      receiveEvent(uint8_t howMany);
static void      cfgManager(void);
static uint16_t  read11BitAngle(void);
static void      readAndUpdateAngle(void);
static void      displayI2cAddOffset(void);

#if !TINY_SOFT_PWM_USES_PIN1
#error TINY_SOFT_PWM_USES_PIN1 shall be uncommented in TinysoftPwm.h!
#endif

void setup()
{
#if !defined(__AVR_ATtiny85__)
  Serial.begin(115200);
  SERIAL_PRINT(F("I2C=0x0"));SERIAL_PRINTLN(A1335_I2C_7B_ADDR, HEX);
#endif

  TinySoftPwm_begin(255, 0); /* 255 x TinySoftPwm_process() calls before overlap (Frequency tuning), 0 = PWM init for all declared pins */

  pinMode(PUSH_BUTTON_PIN, INPUT);digitalWrite(PUSH_BUTTON_PIN, HIGH); // Input with Pull-up
  pinMode(LED_PIN, OUTPUT);digitalWrite(LED_PIN, LOW);
  
  analogReference(DEFAULT); // Ref = VCC = 5V

  cfgManager();
  
  /**
   * Reminder: taking care of pull-ups is the masters job
   */
  TinyWireS.begin(A1335_I2C_7B_ADDR);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  // Display the I2C Offset at startup of the loop
  displayI2cAddOffset();
  /**
   * This is the only way we can detect stop condition 
   * http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=984716&sid=82e9dc7299a8243b86cf7969dd41b5b5#984716
   * it needs to be called in a very tight loop in order not to miss any (REMINDER: Do *not* use delay() anywhere, use tws_delay() instead).
   * It will call the function registered via TinyWireS.onReceive(); if there is data in the buffer on stop.
   */
  TINY_WIRE_S_STOP_CHECK();
  // Read and load current Angle in I2C registers
  readAndUpdateAngle();
}

/*********************/
/* PRIVATE FUNCTIONS */
/*********************/

/**
 * This is called for each read request we receive, never put more than one byte of data (with TinyWireS.send) to the 
 * send-buffer when using this callback
 */
#define NEW_VERSION
static void requestEvent(void) /* This code is executed when the Master performs Read (Wire.requestFrom(@, HowMany)) */
{
#ifdef NEW_VERSION
    if(reg_idx == 0)
    {
      /* Load values in regsiters */
      i2c_regs[0] = A1335Angle >> 8;     // MSB
      i2c_regs[1] = A1335Angle & 0x00FF; // LSB
    }
    TinyWireS.send(i2c_regs[reg_idx]); /* Send the Byte at the pointed Register */
    // Increment the reg position on each read, and loop back to zero
    reg_idx = (reg_idx + 1) % I2C_REG_NB;
#else
  TinyWireS.send(i2c_regs[reg_idx]);
  // Increment the reg position on each read, and loop back to zero
  reg_idx++;
  if (reg_idx >= reg_size)
  {
    i2c_regs[0] = A1335Angle >> 8;     // MSB
    i2c_regs[1] = A1335Angle & 0x00FF; // LSB
    i2c_regs[1] = A1335Ana;
    reg_idx = 0;
  }
#endif
}

/**
 * The I2C data received -handler
 *
 * This needs to complete before the next incoming transaction (start, data, restart/stop) on the bus does
 * so be quick, set flags for long running tasks to be called from the mainloop instead of running them directly,
 */
static void receiveEvent(uint8_t howMany) /* This code is executed when the Master performs Wire.Write() */
{
#ifdef NEW_VERSION
    if (howMany < 1)
    {
        // Sanity-check
        return;
    }
    if (howMany > TWI_RX_BUFFER_SIZE)
    {
        // Also insane number
        return;
    }

    reg_idx = TinyWireS.receive();
    if (reg_idx >= A1335_ANG_REG_ADDR) reg_idx -= A1335_ANG_REG_ADDR;
    if (reg_idx >= I2C_REG_NB)   reg_idx  = 0;
    howMany--;
    if (!howMany)
    {
        // This write was only to set the buffer for next read
        return;
    }
    while(howMany--)
    {
        i2c_regs[reg_idx % I2C_REG_NB] = TinyWireS.receive();
        reg_idx++;
    }
#else
  TinyWireS.receive();
  howMany--;
  reg_idx = 0;
#endif
}

static void cfgManager(void)
{
  static uint8_t  CfgState = CFG_INIT;
  uint32_t        StartMs = millis(), StartMs2 = millis();
  uint8_t         CfgInProgress = 1;
  uint32_t        PushDurationMs;
  uint16_t        Angle;
  uint8_t         I2cSlaveAddrOffset = 0;
  
  while(CfgInProgress)
  {
    switch(CfgState)
    {
      case CFG_INIT:
      SERIAL_PRINTLN(F("CFG_INIT"));
      delay(10); // Wait for the pin setup
      if((uint8_t)EEPROM.read(EEP_I2C_SLAVE_ADDR_OFFSET) == 0xFF)
      {
        EEPROM.write(EEP_I2C_SLAVE_ADDR_OFFSET, I2cSlaveAddrOffset);
      }
      if(BUTTON_PRESSED())
      {
        // Push button is pressed at start up: measure the time window [1sec-3sec] -> Cal, more than 4 sec-> I2C @
        StartMs = millis();
        CfgState = CFG_MEASURE_PUSH_DURATION_MS;
        SERIAL_PRINTLN(F("CFG_INIT->CFG_MEASURE_PUSH_DURATION_MS"));
      }
      else
      {
        CfgState = CFG_END;
        SERIAL_PRINTLN(F("CFG_INIT->CFG_END"));
      }
      break;

      case CFG_MEASURE_PUSH_DURATION_MS:
      while(BUTTON_PRESSED())
      {
        // Push button is released
        PushDurationMs = millis() - StartMs;
        if((PushDurationMs >= 1000UL) && (PushDurationMs <= 3000UL))
        {
          // Calibration request: Generate one flash
          if(CfgState != CFG_CALIBRATE)
          {
            LED_FLASH();
          }
          CfgState = CFG_CALIBRATE;
        }
        else
        {
          if(PushDurationMs >= 4000UL)
          {
            // I2C Address change request: Generate one flash again
            if(CfgState != CFG_CHANGE_I2C_ADDR)
            {
              LED_FLASH();
            }
            CfgState = CFG_CHANGE_I2C_ADDR;
          }
          else
          {
            CfgState = CFG_END;
          }
        }
      }
      /* Released*/
      delay(30); // Simple debounce
      switch(CfgState)
      {
        case CFG_CALIBRATE:
        SERIAL_PRINTLN(F("CFG_MEASURE_PUSH_DURATION_MS->CFG_CALIBRATE"));
        break;

        case CFG_CHANGE_I2C_ADDR:
        SERIAL_PRINTLN(F("CFG_MEASURE_PUSH_DURATION_MS->CFG_CHANGE_I2C_ADDR"));
        break;

        case CFG_END:
        SERIAL_PRINTLN(F("CFG_MEASURE_PUSH_DURATION_MS->CFG_END"));
        break;
      }
      break;

      case CFG_CALIBRATE:
      StartMs = millis();
      Sensor.AngleMin = 4095;
      Sensor.AngleMax = 0;
      do{
        // Here, the user shall turn slowly and continuously the shaft of the analog sensor for at least 360°
        Angle = read12BitAngle(ANA_11_BITS_RES);
        SERIAL_PRINTLN(Angle);
        if(Angle < Sensor.AngleMin) Sensor.AngleMin = Angle;
        if(Angle > Sensor.AngleMax) Sensor.AngleMax = Angle;
        if(Sensor.AngleMin < Sensor.AngleMax)
        {
          if((millis() - StartMs2) >= 100)
          {
            StartMs2 = millis();
            Angle = map(Angle, Sensor.AngleMin, Sensor.AngleMax, 0, 4095);
            Angle = constrain(Angle, 0, 4095);
            TinySoftPwm_analogWrite(LED_PIN, GammaCorrection(Angle >> 4, GAMMA_DEPTH));
          }
          TinySoftPwm_process(); /* Here ana conversion Time base is very approximative, but sufficient for a soft PWM */
        }
      }while((millis() - StartMs) < ANGLE_CALIBRATION_TIME_MS);
      LED_OFF();
      // Adjust to reach max range
      Sensor.AngleMin++;
      Sensor.AngleMax--;
      // Save min and max in EEPROM
      EEPROM.write(EEP_ANA_MIN, highByte(Sensor.AngleMin));EEPROM.write(EEP_ANA_MIN + 1, lowByte(Sensor.AngleMin));
      EEPROM.write(EEP_ANA_MAX, highByte(Sensor.AngleMax));EEPROM.write(EEP_ANA_MAX + 1, lowByte(Sensor.AngleMax));
      CfgState = CFG_END;
      break;

      case CFG_CHANGE_I2C_ADDR:
      StartMs = millis();
      do{
        // Here, the user has 10 seconds to push the button: 0 time -> Offset=0, 1 time -> Offset=1, 2 times -> I2cSlaveAddrOffset=2, 3 times -> Offset=3
        if(BUTTON_PRESSED())
        {
          delay(50); // Simple debounce
          while(BUTTON_PRESSED());// Wait for release
          delay(50); // Simple debounce
          I2cSlaveAddrOffset++;
          SERIAL_PRINT(F("I2cSlaveAddrOffset="));SERIAL_PRINTLN(I2cSlaveAddrOffset);
        }
      }while((millis() - StartMs) < I2C_ADDR_OFFSET_SETTING_TIME_MS);
      // Save I2C Offset in EEPROM
      EEPROM.write(EEP_I2C_SLAVE_ADDR_OFFSET, I2cSlaveAddrOffset);
      CfgState = CFG_END;
      break;
      
      case CFG_END:
      CfgInProgress = 0;
      // Load min and max values in ram in order to avoid accessing to the EEPROM during I2C interrupts
      Sensor.AngleMin = ((uint8_t)EEPROM.read(EEP_ANA_MIN) << 8) + (uint8_t)EEPROM.read(EEP_ANA_MIN + 1);
      Sensor.AngleMax = ((uint8_t)EEPROM.read(EEP_ANA_MAX) << 8) + (uint8_t)EEPROM.read(EEP_ANA_MAX + 1);
      SERIAL_PRINT(F("Sensor.AngleMin="));SERIAL_PRINTLN(Sensor.AngleMin);
      SERIAL_PRINT(F("Sensor.AngleMax="));SERIAL_PRINTLN(Sensor.AngleMax);
      break;
    }
  }
}

/*Oversampling: http://genelaix.free.fr/IMG/pdf/CANover.pdf
  K = Additonal bits of resolution
  M = Needed samples
  M = 4 power K
  Ex: Adding 2 bits of resolution -> N = 2 -> M = 4 pow 2 = 16
*/
static uint16_t read12BitAngle(uint8_t AnaRes)
{
  uint8_t  NeededSample = 1;
  uint16_t TwelveBitAngle = 0;
  
  for(uint8_t Pow = 0; Pow < AnaRes; Pow++)
  {
    NeededSample <<= 2; // x 4
  }
  for(uint8_t Iter = 0; Iter < NeededSample; Iter++)
  {
    TwelveBitAngle += READ_ANA_10BIT_ANGLE();
    TINY_WIRE_S_STOP_CHECK();
    if(DispState == DISP_IDLE)
    {
      TinySoftPwm_process(); /* Here ana conversion Time base is very approximative, but sufficient for a soft PWM */
    }
  }
  if(!AnaRes) TwelveBitAngle <<= 2;
  else
  {
    if(AnaRes == ANA_12_BITS_RES) TwelveBitAngle >>= (AnaRes);
  }
  return(TwelveBitAngle); // Result always on 12 bits
}

static void readAndUpdateAngle(void)
{
  static uint32_t StartMs = millis();
  uint16_t RawAngle;

  RawAngle = read12BitAngle(ANA_12_BITS_RES);
  if(RawAngle < Sensor.AngleMin) RawAngle = Sensor.AngleMin;
  if(RawAngle > Sensor.AngleMax) RawAngle = Sensor.AngleMax;
  CurrentAngle = map(RawAngle, Sensor.AngleMin, Sensor.AngleMax, 0, 4095);
  CurrentAngle = constrain(CurrentAngle, 0, 4095);
  if((millis() - StartMs) >= 250UL)
  {
    StartMs = millis();
    TinySoftPwm_analogWrite(LED_PIN, GammaCorrection(CurrentAngle >> 4, GAMMA_DEPTH));
    SERIAL_PRINT(RawAngle);SERIAL_PRINT(F(" "));SERIAL_PRINTLN(CurrentAngle);
  }
  cli();
  A1335Angle = CurrentAngle;
  sei();
  READ_ANA_INPUT();            /* Dummy read to prepare correct Ana Read */
  A1335Ana = READ_ANA_INPUT();
  READ_ANA_10BIT_ANGLE();      /* Dummy read to prepare correct Angle Read */
  i2c_regs[2] = A1335Ana;      /* Can be loaded directly in register since 8 bits */
}

static void displayI2cAddOffset(void)
{
  static uint32_t StartMs;
  static uint8_t  I2CAddOffset = (uint8_t)EEPROM.read(EEP_I2C_SLAVE_ADDR_OFFSET);
  
  switch(DispState)
  {
    case DISP_FLASH_START:
    SERIAL_PRINT(F("I2CAddOffset="));SERIAL_PRINTLN(I2CAddOffset);
    LED_ON();
    StartMs = millis();
    DispState = DISP_FLASH_IN_PROGRESS;
    break;
    
    case DISP_FLASH_IN_PROGRESS:
    if(millis() - StartMs >= 30UL)
    {
      LED_OFF();
      StartMs = millis();
      if(I2CAddOffset) DispState = DISP_PAUSE;
      else             DispState = DISP_WAIT_FOR_2_SEC;
    }
    break;

    case DISP_PAUSE:
    if((millis() - StartMs) >= 1000UL)
    {
      DispState = DISP_LED_START;
    }
    break;

    case DISP_LED_START:
    LED_ON();
    StartMs = millis();
    DispState = DISP_LED_IN_PROGRESS;
    break;

    case DISP_LED_IN_PROGRESS:
    if((millis() - StartMs) >= 500UL)
    {
      LED_OFF();
      StartMs = millis();
      I2CAddOffset--;
      if(I2CAddOffset) DispState = DISP_PAUSE;
      else             DispState = DISP_WAIT_FOR_2_SEC;
    }
    break;

    case DISP_WAIT_FOR_2_SEC:
    if((millis() - StartMs) >= 2000UL)
    {
      DispState = DISP_IDLE;
    }
    break;
    
    case DISP_IDLE:
    default:
    break;
  }
}

uint8_t GammaCorrection(uint8_t Intensity, uint8_t Depth)
{
  uint8_t CorrectedIntensity = Intensity;
  while(Depth--)
  {
    CorrectedIntensity = (CorrectedIntensity * CorrectedIntensity) >> 8;
  }
  return(CorrectedIntensity);
}
