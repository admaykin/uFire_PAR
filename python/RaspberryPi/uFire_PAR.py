import math
import struct
import time
import smbus  # pylint: disable=E0401

global i2c
UFIRE_PAR = 0x3B

PAR_MEASURE_PAR = 80
PAR_I2C = 40
PAR_READ = 20
PAR_WRITE = 10

PAR_VERSION_REGISTER = 0
PAR_FW_VERSION_REGISTER = 1
PAR_PAR_REGISTER = 2
PAR_BUFFER1_REGISTER = 6
PAR_BUFFER2_REGISTER = 10
PAR_TASK_REGISTER = 14

PAR_I2C_ADDRESS_REGISTER = 200

PAR_MEASUREMENT_TIME = 250


class uFire_PAR(object):
    ppfd = 0

    def __init__(self, address=UFIRE_PAR, i2c_bus=3, **kwargs):
        global i2c
        self.address = address
        i2c = smbus.SMBus(i2c_bus)

    def measurePAR(self):
        self._send_command(PAR_MEASURE_PAR)
        time.sleep(PAR_MEASUREMENT_TIME / 1000.0)
        self.ppfd = self._read_register(PAR_PAR_REGISTER)
        return self.ppfd

    def getVersion(self):
        return self._read_byte(PAR_VERSION_REGISTER)

    def getFirmware(self):
        return self._read_byte(PAR_FW_VERSION_REGISTER)

    def setI2CAddress(self, i2cAddress):
        if i2cAddress >= 1 and i2cAddress <= 127:
            self._write_register(PAR_BUFFER2_REGISTER, float(i2cAddress))
            self._send_command(PAR_I2C)
            self.address = int(i2cAddress)

    def connected(self):
        retval = self._read_byte(PAR_VERSION_REGISTER)

        if retval != 0xFF:
            return True
        else:
            return False

    def readEEPROM(self, address):
        self._write_register(PAR_BUFFER1_REGISTER, int(address))
        self._send_command(PAR_READ)
        return self._read_register(PAR_BUFFER2_REGISTER)

    def writeEEPROM(self, address, val):
        self._write_register(PAR_BUFFER1_REGISTER, int(address))
        self._write_register(PAR_BUFFER2_REGISTER, float(val))
        self._send_command(PAR_WRITE)

    def _bit_set(self, v, index, x):
        mask = 1 << index
        v &= ~mask
        if x:
            v |= mask
        return v

    def _change_register(self, r):
        global i2c
        i2c.write_byte(self.address, r)
        time.sleep(10 / 1000.0)

    def _send_command(self, command):
        global i2c
        i2c.write_byte_data(self.address, PAR_TASK_REGISTER, command)
        time.sleep(10 / 1000.0)

    def _write_register(self, reg, f):
        global i2c
        n = self.round_total_digits(f)
        fd = bytearray(struct.pack("f", n))
        data = [0, 0, 0, 0]
        data[0] = fd[0]
        data[1] = fd[1]
        data[2] = fd[2]
        data[3] = fd[3]
        self._change_register(reg)
        i2c.write_i2c_block_data(self.address, reg, data)
        time.sleep(10 / 1000.0)

    def _read_register(self, reg):
        global i2c
        data = [0, 0, 0, 0]
        self._change_register(reg)
        data[0] = i2c.read_byte(self.address)
        data[1] = i2c.read_byte(self.address)
        data[2] = i2c.read_byte(self.address)
        data[3] = i2c.read_byte(self.address)
        ba = bytearray(data)
        f = struct.unpack('f', ba)[0]
        return self.round_total_digits(f)

    def _write_byte(self, reg, val):
        global i2c
        i2c.write_byte_data(self.address, reg, val)
        time.sleep(10 / 1000.0)

    def _read_byte(self, reg):
        global i2c
        self._change_register(reg)
        time.sleep(10 / 1000.0)
        return i2c.read_byte(self.address)

    def magnitude(self, x):
        if math.isnan(x):
            return 0
        return 0 if x == 0 else int(math.floor(math.log10(abs(x)))) + 1

    def round_total_digits(self, x, digits=7):
        return round(x, digits - self.magnitude(x))
