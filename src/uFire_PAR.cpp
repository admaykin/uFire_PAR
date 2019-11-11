#include "uFire_PAR.h"

uFire_PAR::uFire_PAR(uint8_t i2c_address)
{
  _address = i2c_address;
  #ifndef ARDUINO_SAMD_ZERO
  Wire.begin();
  #endif // ifndef ARDUINO_SAMD_ZERO
}

uFire_PAR::uFire_PAR()
{
  _address = 0x3B;
  #ifndef ARDUINO_SAMD_ZERO
  Wire.begin();
  #endif // ifndef ARDUINO_SAMD_ZERO
}

#ifdef ESP32
uFire_PAR::uFire_PAR(uint8_t sda, uint8_t scl, uint8_t i2c_address)
{
  _address = i2c_address;
  Wire.begin(sda, scl, 100000);
}

uFire_PAR::uFire_PAR(uint8_t sda, uint8_t scl)
{
  _address = 0X3B;
  Wire.begin(sda, scl, 100000);
}

#endif // ifndef ESP32

uFire_PAR::~uFire_PAR()
{}

float uFire_PAR::measurePAR()
{
  _send_command(PAR_MEASURE_PAR);
  delay(PAR_MEASUREMENT_TIME);
  ppfd = _read_register(PAR_PAR_REGISTER);

  return ppfd;
}

uint8_t uFire_PAR::getVersion()
{
  return _read_byte(PAR_VERSION_REGISTER);
}

uint8_t uFire_PAR::getFirmware()
{
  return _read_byte(PAR_FW_VERSION_REGISTER);
}

void uFire_PAR::setI2CAddress(uint8_t i2cAddress)
{
  _write_register(PAR_BUFFER1_REGISTER, i2cAddress);
  _send_command(PAR_I2C);
  _address = i2cAddress;
}

bool uFire_PAR::connected()
{
  uint8_t retval;

  retval = _read_byte(PAR_VERSION_REGISTER);
  if (retval != 0xFF) {
    return true;
  }
  else {
    return false;
  }
}

float uFire_PAR::readEEPROM(uint8_t address)
{
  _write_register(PAR_BUFFER1_REGISTER, address);
  _send_command(PAR_READ);
  return _read_register(PAR_BUFFER2_REGISTER);
}

void uFire_PAR::writeEEPROM(uint8_t address, float value)
{
  _write_register(PAR_BUFFER1_REGISTER, address);
  _write_register(PAR_BUFFER2_REGISTER,   value);
  _send_command(PAR_WRITE);
}

void uFire_PAR::_change_register(uint8_t r)
{
  Wire.beginTransmission(_address);
  Wire.write(r);
  Wire.endTransmission();
  delay(10);
}

void uFire_PAR::_send_command(uint8_t command)
{
  Wire.beginTransmission(_address);
  Wire.write(PAR_TASK_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
  delay(10);
}

void uFire_PAR::_write_register(uint8_t reg, float f)
{
  uint8_t b[5];
  float   f_val = f;

  b[0] = reg;
  b[1] = *((uint8_t *)&f_val);
  b[2] = *((uint8_t *)&f_val + 1);
  b[3] = *((uint8_t *)&f_val + 2);
  b[4] = *((uint8_t *)&f_val + 3);
  Wire.beginTransmission(_address);
  Wire.write(b, 5);
  Wire.endTransmission();
  delay(10);
}

float uFire_PAR::_read_register(uint8_t reg)
{
  float retval;

  _change_register(reg);
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 1) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 2) = Wire.read();
  Wire.requestFrom(_address, (uint8_t)1);
  *((uint8_t *)&retval + 3) = Wire.read();
  delay(10);
  return retval;
}

void uFire_PAR::_write_byte(uint8_t reg, uint8_t val)
{
  uint8_t b[5];

  b[0] = reg;
  b[1] = val;
  Wire.beginTransmission(_address);
  Wire.write(b, 2);
  Wire.endTransmission();
  delay(10);
}

uint8_t uFire_PAR::_read_byte(uint8_t reg)
{
  uint8_t retval;

  _change_register(reg);
  Wire.requestFrom(_address, (uint8_t)1);
  retval = Wire.read();
  delay(10);
  return retval;
}
