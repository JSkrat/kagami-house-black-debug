# kagami-house-black-debug
Debug board with uart and lcd, also it is a modem

## Project structure

- hw contains schematic and pcb
- fw contains the firmware
- fw/usb2nrf_tests contains unadapted unit tests (TODO: adapt to submoduled avr core)
- serial-debugger is gui qt app to control the board via usb (uart)
