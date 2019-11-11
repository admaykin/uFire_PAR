//! # PAR Sensor
//!
//! * measure PAR in μmol/m²/s

use std::f32;
use std::thread;
use std::time::Duration;
use std::mem;

extern crate byteorder;
use byteorder::{ByteOrder, LittleEndian};

extern crate i2cdev;
use self::i2cdev::core::*;
use self::i2cdev::linux::{LinuxI2CDevice, LinuxI2CError};

const PAR_MEASURE_PAR: u8 =80;
const PAR_I2C: u8 = 40;
const PAR_READ: u8 = 20;
const PAR_WRITE: u8 = 10;

const PAR_VERSION_REGISTER: u8 = 0;
const PAR_FW_VERSION_REGISTER: u8 = 1;
const PAR_PAR_REGISTER: u8 = 2;
const PAR_BUFFER1_REGISTER: u8 = 6;
const PAR_BUFFER2_REGISTER: u8 = 10;
const PAR_TASK_REGISTER: u8 = 14;

const PAR_I2C_ADDRESS_REGISTER: u8 = 200;

const PAR_MEASUREMENT_TIME: u8 = 250;

pub struct ParSensor {
    dev: Box<LinuxI2CDevice>,
}

impl ParSensor {
    /// Create a new ParSensor object
    ///
    /// Pass the i2c port to use, it must be a software overlay device, and I2C address.
    /// # Example
    /// ```
    /// let mut par = ufire_par::ParSensor::new("/dev/i2c-3", 0x3b).unwrap();
    /// ```
    pub fn new(filename: &'static str, address: u16) -> Result<Self, Box<LinuxI2CError>> {
        let dev = LinuxI2CDevice::new(filename, address)?;
        Ok(ParSensor { dev: Box::new(dev) })
    }

    /// Starts an PAR measurement
    ///
    /// # Example
    /// ```
    /// let mut ec = ufire_par::ParSensor::new("/dev/i2c-3", 0x3b).unwrap();
    /// ec.measure_par();
    /// ```
    pub fn measure_ec(&mut self, temp_c: f32) -> Result<(f32), Box<LinuxI2CError>> {
        self.dev.smbus_write_byte_data(EC_TASK_REGISTER, PAR_MEASURE_PAR)?;
        thread::sleep(Duration::from_millis(EC_EC_MEASUREMENT_TIME));

        Ok(self._read_register(PAR_PAR_REGISTER)?)
    }


    /// Returns the firmware version of the device.
    ///
    /// # Example
    /// ```
    /// let mut ec = ufire_ec::EcProbe::new("/dev/i2c-3", 0x3c).unwrap();
    /// assert_eq!(0x1c, ec.get_version().unwrap());
    /// ```
    pub fn get_version(&mut self) -> Result<(u8), Box<LinuxI2CError>> {
        self._change_register(PAR_VERSION_REGISTER)?;
        Ok(self.dev.smbus_read_byte()?)
    }

    /// Returns the firmware version of the device.
    ///
    /// # Example
    /// ```
    /// let mut ec = ufire_ec::EcProbe::new("/dev/i2c-3", 0x3c).unwrap();
    /// assert_eq!(0x1, ec.get_version().unwrap());
    /// ```
    pub fn get_firmware(&mut self) -> Result<(u8), Box<LinuxI2CError>> {
        self._change_register(PAR_FW_VERSION_REGISTER)?;
        Ok(self.dev.smbus_read_byte()?)
    }

    /// Sets the I2C address of the device.
    ///
    /// # Example
    /// ```
    /// let mut ec = ufire_ec::EcProbe::new("/dev/i2c-3", 0x3c).unwrap();
    /// // ec.set_i2c_address(0x4f);
    /// ```
    pub fn set_i2c_address(&mut self, i2c_address: u16) -> Result<(), Box<LinuxI2CError>> {
        self._write_register(PAR_BUFFER1_REGISTER, i2c_address as f32)?;
        self.dev.smbus_write_byte_data(PAR_TASK_REGISTER, PAR_I2C)?;

        Ok(())
    }
    
    pub fn _write_register(&mut self, register: u8, f_val: f32) -> Result<(), Box<LinuxI2CError>> {
        unsafe {
            let buf: [u8; 4] = mem::transmute(f_val);
            self._change_register(register)?;
            self.dev.smbus_write_byte_data(register + 0, buf[0])?;
            thread::sleep(Duration::from_millis(10));
            self.dev.smbus_write_byte_data(register + 1, buf[1])?;
            thread::sleep(Duration::from_millis(10));
            self.dev.smbus_write_byte_data(register + 2, buf[2])?;
            thread::sleep(Duration::from_millis(10));
            self.dev.smbus_write_byte_data(register + 3, buf[3])?;
            thread::sleep(Duration::from_millis(10));
            Ok(())
        }
    }

    pub fn _read_register(&mut self, register: u8) -> Result<(f32), Box<LinuxI2CError>> {
        let mut buf: [u8; 4] = [0; 4];
        self._change_register(register)?;
        buf[0] = self.dev.smbus_read_byte()?;
        thread::sleep(Duration::from_millis(10));
        buf[1] = self.dev.smbus_read_byte()?;
        thread::sleep(Duration::from_millis(10));
        buf[2] = self.dev.smbus_read_byte()?;
        thread::sleep(Duration::from_millis(10));
        buf[3] = self.dev.smbus_read_byte()?;
        thread::sleep(Duration::from_millis(10));
        Ok(LittleEndian::read_f32(&buf))
    }

    pub fn _change_register(&mut self, register: u8) -> Result<(), Box<LinuxI2CError>> {
        self.dev.smbus_write_byte(register)?;
        thread::sleep(Duration::from_millis(10));
        Ok(())
    }
}
