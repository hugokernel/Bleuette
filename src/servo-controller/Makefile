# MPLAB IDE generated this makefile for use with GNU make.
# Project: bleuette.mcp
# Date: Sat Jan 19 17:23:07 2013

AS = MPASMWIN.exe
CC = 
LD = mplink.exe
AR = mplib.exe
RM = rm

main.cof : main.o
	$(CC) /p18F452 "main.o" /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"main.cof" /M"main.map" /W /x

main.o : main.asm ../../../../../MPASM\ Suite/p18F452.inc macro.inc init.inc init2.inc
	$(AS) /q /p18F452 "main.asm" /l"main.lst" /e"main.err" /d__DEBUG=1

clean : 
	$(CC) "main.o" "main.hex" "main.err" "main.lst" "main.cof"

