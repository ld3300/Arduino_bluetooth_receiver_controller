This sketch manipulates the on and off of an MPOW bluetooth receiver I use in my car to pair 
with my phone to get audio into the car system.  The Arduino will power up and startup the MPOW when
the car is started.  The MPOW has an odd mix of 5v and 1.8v on board, plus the on board battery.  
The on board tactical switches pull their respective pins high to trigger their functions.  The 
voltage measured at pins is 4.2 VDC, so just to be safe I put a diode between the Arduino pin and 
the wire I soldered to the pin side of the switches (even though I am fairly positive that it is 
just meant to be 5 volts).

The following connections were made without disabling the switches natural functions in case I need
to turn it on manually.

The hookup is: 
 - 5v from Arduino to USB 5v test point.  
 - From pin 3 through a resistor to the base of an NPN transistor.  
 - Collector of transmitter connected to the ground of the bluetooth module. 
 - The emitter of the transistor connected to ground on the Arduino.  
 - Pin 5 is connected to the Anode of a diode.  The cathode is connected to the pin side of the 
  power on tactile switch (which has to be held down for 3 seconds to turn unit on, more for off). 
 - Connected from VIN on the Arduino to A0 to sense when power to the USB input was cut.  This seems 
  to read accurately even though I have 6 x 2700uF capacitors connected to the 5V and GND pins to
  hold power long enough for the Arduino to control turning off the bluetooth (which has an internal
  battery and would stay on).  Forcing it off will keep it from remaining paired to my phone.

The MPOW receiver has a "feature" where if it is on battery power and USB power is applied it will
shut off and start charging.  You can then turn it on while charging and connect again.  It can be
irritating when you stop for gas, and the act of restarting the car will make the MPOW think it has 
had power killed are reapplied.  I am hoping that the capacitor array will enable the Arduino to 
keep the receiver live during such events.  Only shutting off when power is actually killed.

I will use this same feature to turn the bluetooth receiver off, since it requires less time than 
the 4 plus seconds that is required holding the power switch to turn it off.  All I need to do is 
turn off the Arduino pin controlling power, wait a short bit then turn it back on.  I loop through
this sequence infinitely to make sure the capacitors run low enough fast enough without restarting
the sketch and turning bluetooth back on (which was happening.)

I also connected wires to the following that I am not currently using but may some day:
 - The 5 volt line on the MPOW that is driven by the battery
 - The "+" switch that is mostly used for skipping tracks
 - The "-" switch that is mostly used for skipping back