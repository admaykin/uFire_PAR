### Photosynthetically Active Radiation Sensor
 - Spectral range: 400~700nm
 - Accuracy: ±5%
 - Range: 0 - 2500 μmol/m²/s
 - Response time: < 1 second
 - Interface: I2C
 - Voltage range: 3.3 - 5 V
 
### What it is

A PAR sensor in a waterproof aluminum enclosure with a 1.5m length of cable. The sensor comes pre-calibrated and doesn't require any subsequent recalibration. 

The device uses the Qwiic Connect System for wiring, it's an easy-to-use, polarized, push-to-fit connector. No more mistaken connections or soldering header pins.

It comes with a small tripod and bubble level for easy deployment. 

### Using it

The [documentation](https://ufire.co/docs/uFire_PAR/) on the [specification](https://ufire.co/docs/uFire_PAR/#characteristics), and [setup](https://ufire.co/docs/uFire_PAR/#getting-started) of the sensor is available.

The Arduino IDE, PlatformIO, and Particle.io library is in the respective library manager. Any Arduino compatible board will work. Any Raspberry Pi 3, 4 or Zero will work as well. 

A [python](https://github.com/u-fire/uFire_PAR/tree/master/python/RaspberryPi) and [rust](https://crates.io/crates/ufire_par) library is available for the Raspberry Pi.

```
#include <uFire_PAR.h>
uFire_PAR par;
par.measurePAR();
```

#### Buy it

Visit [ufire.co](http://ufire.co) and buy a board and probe.
