# grbl for h-bridge

grbl is controller software that allows stage control using G-code commands.

This directory contains a patch to use the [grbl](https://github.com/gnea/grbl) G-code controller with the xy microscope stage.  This patch is needed because

- the steppers are connected to the controller pins as h-bridge, while grbl expects step/direction stepper drivers.
- the xy stage has homing switches at the center of the stage, while grbl prefers limit switches at the end of the stage.

## compilation

The flash upload command may differ for your setup. For compilation on linux, install arduino ide and set the path to use avr-gcc and avrdude from arduino:

```
export PATH=~/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/:~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/:$PATH
git clone https://github.com/koendv/xy-stage
git clone https://github.com/gnea/grbl
cd grbl/
patch -p1 < ../xy-stage/grbl/grbl_1_1h.patch
make
make flash
```

For compiling on Windows using the Arduino IDE, see [compiling GRBL](https://github.com/gnea/grbl/wiki/Compiling-Grbl)

## links

- [GRBL-H-Bridge](https://github.com/rakshithbk/GRBL-H-Bridge)
- [h-grbl](http://xn--zabaaganionemiejsce-8fd.pl/arduino/h-grbl/)
