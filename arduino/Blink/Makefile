BOARD=arduino:avr:pro:cpu=8MHzatmega328
TTY=/dev/ttyUSB0

all:
	arduino-cli compile -b $(BOARD) .

flash: all
#	arduino-cli upload -p net:192.168.1.178:2000 -b arduino:avr:uno . 
	arduino-cli upload -b $(BOARD) -p $(TTY) . -vv

output:
	stty -F $(TTY) 115200 min 1 time 0
	cat $(TTY)

flash_manual: all
	avrdude -patmega328p -carduino -P $(TTY) -b 57600 -D -Uflash:w:/tmp/arduino-sketch-5058F1AF8388633F609CADB75A75DC9D/Blink.ino.hex
