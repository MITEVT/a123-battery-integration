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
LIBS:MITEVT_ANALOG
LIBS:BCM-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3400 2600 0    60   Input ~ 0
CAN1_TX
Text HLabel 3400 2500 0    60   Input ~ 0
CAN1_RX
Text HLabel 4850 2600 2    60   Input ~ 0
CAN1_L_I
Text HLabel 4850 2800 2    60   Input ~ 0
CAN1_H_I
$Comp
L GNDREF #PWR018
U 1 1 55452D77
P 4050 3350
F 0 "#PWR018" H 4050 3100 50  0001 C CNN
F 1 "GNDREF" H 4050 3200 50  0000 C CNN
F 2 "" H 4050 3350 60  0000 C CNN
F 3 "" H 4050 3350 60  0000 C CNN
	1    4050 3350
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR019
U 1 1 55452D8F
P 4050 1950
F 0 "#PWR019" H 4050 1800 50  0001 C CNN
F 1 "+5V" H 4050 2090 50  0000 C CNN
F 2 "" H 4050 1950 60  0000 C CNN
F 3 "" H 4050 1950 60  0000 C CNN
	1    4050 1950
	1    0    0    -1  
$EndComp
Text HLabel 4350 1950 2    60   Output ~ 0
CAN_5V_I
$Comp
L ISO1050 U?
U 1 1 55453F28
P 4150 2700
F 0 "U?" H 3800 3050 60  0000 C CNN
F 1 "ISO1050" H 4450 3050 60  0000 C CNN
F 2 "" H 4450 3050 60  0000 C CNN
F 3 "" H 4450 3050 60  0000 C CNN
	1    4150 2700
	1    0    0    -1  
$EndComp
Text HLabel 4300 3350 2    60   Output ~ 0
CAN_GND_I
Wire Wire Line
	4250 2200 4250 1950
Wire Wire Line
	4250 1950 4350 1950
Wire Wire Line
	4050 2200 4050 1950
Wire Wire Line
	4750 2600 4850 2600
Wire Wire Line
	4750 2800 4850 2800
Wire Wire Line
	4250 3200 4250 3350
Wire Wire Line
	4250 3350 4300 3350
Wire Wire Line
	4050 3200 4050 3350
Wire Wire Line
	3400 2500 3550 2500
Wire Wire Line
	3550 2600 3400 2600
$EndSCHEMATC
