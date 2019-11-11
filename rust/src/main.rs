extern crate ufire_iso_ec;
extern crate shrust;
use shrust::{Shell, ShellIO};
use std::io::prelude::*;
use ufire_iso_ec::*;

fn main() {
	println!("uFire PAR Sensor shell");
	println!("type `help` for a list of commands");
	let v = vec![0x3b];
	let mut shell = Shell::new(v);
	shell.new_command_noargs("config", "Prints configuration data", move |io, v| {
		let mut par = ParSensor::new("/dev/i2c-3", v[0]).unwrap();
		try!(writeln!(io, "	version hw.fw: {}.{}", par.get_version().unwrap(), par.get_firmware().unwrap()));
		Ok(())
	});

	shell.new_command_noargs("par", "Takes a PAR measurement", move |io, v| {
		let mut par = ParSensor::new("/dev/i2c-3", v[0]).unwrap();
		let ppfd = par.measure_par().unwrap();
		try!(writeln!(io, "μmol/m²/s: {}", ppfd));
		Ok(())
	});

	shell.new_command_noargs("ver", "Hardware/Firmware of device", move |io, v| {
		let mut par = ParSensor::new("/dev/i2c-3", v[0]).unwrap();
		try!(writeln!(io, "	version: {}.{}", par.get_version().unwrap(),par.get_firmware().unwrap()));
		Ok(())
	});

	shell.new_command("i2c", "`i2c <decimal I2C address>` : Changes the I2C address", 1, move |io, v, s| {
		let mut par = ParSensor::new("/dev/i2c-3", v[0]).unwrap();
		v[0] = s[0].parse().unwrap();
		par.set_i2c_address(v[0]).unwrap();
		try!(writeln!(io, "	I2C Address: {}", v[0]));
		Ok(())
	});

    shell.run_loop(&mut ShellIO::default());
}
