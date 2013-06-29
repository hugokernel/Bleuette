























.build/leonardo/Bleuette/bleuette.o : lib/Bleuette/bleuette.cpp
	@echo [93mBleuette/bleuette.cpp[0m
	@mkdir -p .build/leonardo/Bleuette
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c lib/Bleuette/bleuette.cpp
include .build/leonardo/Bleuette/bleuette.d



.build/leonardo/Bleuette/libBleuette.a : .build/leonardo/Bleuette/bleuette.o
	@echo [92mLinking libBleuette.a[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar rcs $@ $^












.build/leonardo/SerialCommand/SerialCommand.o : lib/SerialCommand/SerialCommand.cpp
	@echo [93mSerialCommand/SerialCommand.cpp[0m
	@mkdir -p .build/leonardo/SerialCommand
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c lib/SerialCommand/SerialCommand.cpp
include .build/leonardo/SerialCommand/SerialCommand.d



.build/leonardo/SerialCommand/libSerialCommand.a : .build/leonardo/SerialCommand/SerialCommand.o
	@echo [92mLinking libSerialCommand.a[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar rcs $@ $^












.build/leonardo/Sequencer/sequencer.o : lib/Sequencer/sequencer.cpp
	@echo [93mSequencer/sequencer.cpp[0m
	@mkdir -p .build/leonardo/Sequencer
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c lib/Sequencer/sequencer.cpp
include .build/leonardo/Sequencer/sequencer.d



.build/leonardo/Sequencer/libSequencer.a : .build/leonardo/Sequencer/sequencer.o
	@echo [92mLinking libSequencer.a[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar rcs $@ $^












.build/leonardo/ServoController/servocontroller.o : lib/ServoController/servocontroller.cpp
	@echo [93mServoController/servocontroller.cpp[0m
	@mkdir -p .build/leonardo/ServoController
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c lib/ServoController/servocontroller.cpp
include .build/leonardo/ServoController/servocontroller.d



.build/leonardo/ServoController/libServoController.a : .build/leonardo/ServoController/servocontroller.o
	@echo [92mLinking libServoController.a[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar rcs $@ $^







.build/leonardo/arduino/WInterrupts.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WInterrupts.c
	@echo [93marduino/WInterrupts.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WInterrupts.c
include .build/leonardo/arduino/WInterrupts.d

.build/leonardo/arduino/wiring.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring.c
	@echo [93marduino/wiring.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring.c
include .build/leonardo/arduino/wiring.d

.build/leonardo/arduino/wiring_analog.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_analog.c
	@echo [93marduino/wiring_analog.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_analog.c
include .build/leonardo/arduino/wiring_analog.d

.build/leonardo/arduino/wiring_digital.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_digital.c
	@echo [93marduino/wiring_digital.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_digital.c
include .build/leonardo/arduino/wiring_digital.d

.build/leonardo/arduino/wiring_pulse.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_pulse.c
	@echo [93marduino/wiring_pulse.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_pulse.c
include .build/leonardo/arduino/wiring_pulse.d

.build/leonardo/arduino/wiring_shift.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_shift.c
	@echo [93marduino/wiring_shift.c[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/wiring_shift.c
include .build/leonardo/arduino/wiring_shift.d






.build/leonardo/arduino/CDC.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/CDC.cpp
	@echo [93marduino/CDC.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/CDC.cpp
include .build/leonardo/arduino/CDC.d

.build/leonardo/arduino/HardwareSerial.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/HardwareSerial.cpp
	@echo [93marduino/HardwareSerial.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/HardwareSerial.cpp
include .build/leonardo/arduino/HardwareSerial.d

.build/leonardo/arduino/HID.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/HID.cpp
	@echo [93marduino/HID.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/HID.cpp
include .build/leonardo/arduino/HID.d

.build/leonardo/arduino/IPAddress.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/IPAddress.cpp
	@echo [93marduino/IPAddress.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/IPAddress.cpp
include .build/leonardo/arduino/IPAddress.d

.build/leonardo/arduino/main.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/main.cpp
	@echo [93marduino/main.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/main.cpp
include .build/leonardo/arduino/main.d

.build/leonardo/arduino/new.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/new.cpp
	@echo [93marduino/new.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/new.cpp
include .build/leonardo/arduino/new.d

.build/leonardo/arduino/Print.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Print.cpp
	@echo [93marduino/Print.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Print.cpp
include .build/leonardo/arduino/Print.d

.build/leonardo/arduino/Stream.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Stream.cpp
	@echo [93marduino/Stream.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Stream.cpp
include .build/leonardo/arduino/Stream.d

.build/leonardo/arduino/Tone.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Tone.cpp
	@echo [93marduino/Tone.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/Tone.cpp
include .build/leonardo/arduino/Tone.d

.build/leonardo/arduino/USBCore.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/USBCore.cpp
	@echo [93marduino/USBCore.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/USBCore.cpp
include .build/leonardo/arduino/USBCore.d

.build/leonardo/arduino/WMath.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WMath.cpp
	@echo [93marduino/WMath.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WMath.cpp
include .build/leonardo/arduino/WMath.d

.build/leonardo/arduino/WString.o : /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WString.cpp
	@echo [93marduino/WString.cpp[0m
	@mkdir -p .build/leonardo/arduino
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions  -o $@ -c /Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino/WString.cpp
include .build/leonardo/arduino/WString.d



.build/leonardo/arduino/libarduino.a : .build/leonardo/arduino/WInterrupts.o .build/leonardo/arduino/wiring.o .build/leonardo/arduino/wiring_analog.o .build/leonardo/arduino/wiring_digital.o .build/leonardo/arduino/wiring_pulse.o .build/leonardo/arduino/wiring_shift.o .build/leonardo/arduino/CDC.o .build/leonardo/arduino/HardwareSerial.o .build/leonardo/arduino/HID.o .build/leonardo/arduino/IPAddress.o .build/leonardo/arduino/main.o .build/leonardo/arduino/new.o .build/leonardo/arduino/Print.o .build/leonardo/arduino/Stream.o .build/leonardo/arduino/Tone.o .build/leonardo/arduino/USBCore.o .build/leonardo/arduino/WMath.o .build/leonardo/arduino/WString.o
	@echo [92mLinking libarduino.a[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar rcs $@ $^















.build/leonardo/src/bleuette.o : .build/leonardo/src/bleuette.cpp
	@echo [93msrc/bleuette.cpp[0m
	@mkdir -p .build/leonardo/src
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++ -mmcu=atmega32u4 -ffunction-sections -fdata-sections -g -Os -w -DF_CPU=16000000L -DARDUINO=100 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -DUSB_VID=0x2341 -DUSB_PID=0x8036 -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/leonardo -Ilib/Bleuette -Ilib/SerialCommand -Ilib/Sequencer -Ilib/ServoController -I/Applications/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino -fno-exceptions -iquote src  -o $@ -c .build/leonardo/src/bleuette.cpp
include .build/leonardo/src/bleuette.d







.build/leonardo/firmware.elf : .build/leonardo/src/bleuette.o .build/leonardo/Bleuette/libBleuette.a .build/leonardo/SerialCommand/libSerialCommand.a .build/leonardo/Sequencer/libSequencer.a .build/leonardo/ServoController/libServoController.a .build/leonardo/arduino/libarduino.a
	@echo [92mLinking firmware.elf[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega32u4 -o $@ $^ -lm


.build/leonardo/firmware.hex : .build/leonardo/firmware.elf
	@echo [92mConverting to firmware.hex[0m
	@/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-objcopy -O ihex -R .eeprom $^ $@

include .build/leonardo/src/dependencies.d .build/leonardo/arduino/dependencies.d .build/leonardo/Bleuette/dependencies.d .build/leonardo/ServoController/dependencies.d .build/leonardo/Sequencer/dependencies.d .build/leonardo/SerialCommand/dependencies.d

all : .build/leonardo/firmware.hex
	@true

