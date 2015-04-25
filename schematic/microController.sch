EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:MITEVT_interface
LIBS:MITEVT_mcontrollers
LIBS:MITEVT_OPTO
LIBS:MITEVT_REG
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LPC11C24 U?
U 1 1 553C4BFA
P 5750 3350
F 0 "U?" H 4600 5200 60  0000 C CNN
F 1 "LPC11C24" H 6550 1550 60  0000 C CNN
F 2 "" H 5900 2150 60  0000 C CNN
F 3 "" H 5900 2150 60  0000 C CNN
	1    5750 3350
	1    0    0    -1  
$EndComp
Text HLabel 9950 2600 2    60   Output ~ 0
CAN1_RX
Text HLabel 9950 2500 2    60   Output ~ 0
CAN1_TX
Text HLabel 7200 1850 2    60   Output ~ 0
CAN2_H
Text HLabel 7200 1750 2    60   Output ~ 0
CAN2_L
Wire Wire Line
	4350 3050 3500 3050
Text HLabel 3500 3050 0    60   Output ~ 0
HVSENSE_IN
Wire Wire Line
	7000 1850 7200 1850
Wire Wire Line
	7200 1750 7000 1750
$Comp
L MCP2515 U?
U 1 1 553C4E29
P 9150 3050
F 0 "U?" H 8700 3750 60  0000 C CNN
F 1 "MCP2515" H 9400 2400 60  0000 C CNN
F 2 "" H 9400 3050 60  0000 C CNN
F 3 "" H 9400 3050 60  0000 C CNN
	1    9150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 2500 9950 2500
Wire Wire Line
	9950 2600 9750 2600
$EndSCHEMATC
