# Piscine Embedded
> Series of exercices to discover the basics of MCUs, embedded programming with an **ATMega328p** on a development board by ***42Chips*** and datasheets reading.

## Day00: The basics

- Notions:
  - Discovery of commands **avr-gcc**, **ovr-objcopy** and **avrdude**
  - Discovery of registers and documentation of the ATMega328p
  - Managing some LEDs using registers **DDRx**, **PORTx** and **PINx**
  - Bounce effect

## Day01: Timers

- Notions:
  - Set up timer for wanted frequency
  - Timers comparison and overflow flags
  - Duty cycles

## Day02: Protocol UART

- Notions:
  - Transmission
  - Reception

## Day03: RGB & advanced timers

- Notions:
  - Use of multiple timer for a unique application

## Day04: Interruptions

- Notions:
  - Set up interruptions
  - Catch and handle them

## Day05: Analog

- Notions:
  - ADC peripheral
  - Read multiple peripherals (**RV1**, **LDR** (R14), **NTC** (R20))
  - Set up the **ADC** peripheral
 
## Day06: I2C Protocol
- Notions:
  - Start **i2c** communication
  - Stop i2c communication
  - Read/Write
  - Data conversion for temperature and humidity
 
## Day07: EEPROM
- Notions:
  - Read/Write
  - Writing optimization
 
## Day08: SPI
- Notions:
  - Basics of the protocol
  - Control of RGB LEDs using **SPI**
 
## Day09: 7 segments display
- Notions:
  - Use of i2c in another context, following another datasheet (U4)
  - Configuration, output and input mode with the **PCA9555**
  - **7 segment display** setup and usages
