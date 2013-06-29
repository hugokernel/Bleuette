#!/bin/bash

device=$(sed -n 's/.*serial-port *= *\([^ ]*.*\)/\1/p' < ino.ini | tail -n 1)

picocom --omap crlf $device -baud 9600

