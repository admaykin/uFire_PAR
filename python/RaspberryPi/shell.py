import cmd
from uFire_PAR import uFire_PAR

par = uFire_PAR()


class PARShell(cmd.Cmd):
    prompt = '> '

    def do_config(self, a):
        """prints out all the configuration data\nparameters: none"""
        print("PAR Interface Config: " + 'connected' if ec.connected() else '**disconnected**')
        print("\tversion: " + (str(par.getVersion())) + "." + (str(par.getFirmware())))

    def do_par(self, line):
        """starts an PAR measurement"""
        par.measurePAR()
        print("ppdf: " + str(par.ppdf))

    def do_version(self, a):
        """prints the version register"""
        print("\tversion: " + (str(par.getVersion())) + "." + (str(par.getFirmware())))

    def do_i2c(self, i2cAddress):
        """changes the I2C address"""
        par.setI2CAddress(i2cAddress)

    def do_read(self, address):
        print(par.readEEPROM(address))

    def do_write(self, arg):
        a = arg.split()
        par.writeEEPROM(a[0], a[1])

    def do_EOF(self, line):
        return True


ECShell().cmdloop()
