#ifndef UFIRE_PAR
#define UFIRE_PAR

#include <math.h>

#if defined(PARTICLE)
# include "application.h"
# define bitRead(value, bit) (((value) >> (bit)) & 0x01)
# define bitSet(value, bit) ((value) |= (1UL << (bit)))
# define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
# define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#else // if defined(PARTICLE)
# include <Arduino.h>
# include <Wire.h>
#endif // if defined(PARTICLE)

//uint8_t UFIRE_PAR_I2C = 0x3B;
#define PAR_MEASURE_PAR 80
#define PAR_I2C 40
#define PAR_READ 20
#define PAR_WRITE 10

#define PAR_VERSION_REGISTER 0
#define PAR_FW_VERSION_REGISTER 1
#define PAR_PAR_REGISTER 2
#define PAR_BUFFER1_REGISTER 6
#define PAR_BUFFER2_REGISTER 10
#define PAR_TASK_REGISTER 14

#define PAR_I2C_ADDRESS_REGISTER 200

#define PAR_MEASUREMENT_TIME 250

class uFire_PAR
{
public:
 float ppfd;
 uFire_PAR(uint8_t i2c_address);
  uFire_PAR();
  #ifdef ESP32
  uFire_PAR(uint8_t sda,
              uint8_t scl,
              uint8_t i2c_address);
  uFire_PAR(uint8_t sda,
              uint8_t scl);
  #endif // ifndef ESP32
  ~uFire_PAR();
  float measurePAR();
  uint8_t getVersion();
  uint8_t getFirmware();
  void    setI2CAddress(uint8_t i2cAddress);
  bool    connected();
  void    writeEEPROM(uint8_t address,
                      float   value);
  float   readEEPROM(uint8_t address);

private:

  uint8_t _address;
  void    _change_register(uint8_t register);
  void    _send_command(uint8_t command);
  void    _write_register(uint8_t reg,
                          float   f);
  void    _write_byte(uint8_t reg,
                      uint8_t val);
  float   _read_register(uint8_t reg);
  uint8_t _read_byte(uint8_t reg);
};

#endif // UFIRE_PAR