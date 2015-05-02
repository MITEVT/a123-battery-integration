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
Text HLabel 3400 4550 0    60   Input ~ 0
CAN2_TX
Text HLabel 3400 4450 0    60   Input ~ 0
CAN2_RX
Text HLabel 4850 4550 2    60   Input ~ 0
CAN2_L_I
Text HLabel 4850 4750 2    60   Input ~ 0
CAN2_H_I
$Comp
L GNDREF #PWR018
U 1 1 55452D77
P 4050 5300
F 0 "#PWR018" H 4050 5050 50  0001 C CNN
F 1 "GNDREF" H 4050 5150 50  0000 C CNN
F 2 "" H 4050 5300 60  0000 C CNN
F 3 "" H 4050 5300 60  0000 C CNN
	1    4050 5300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR019
U 1 1 55452D8F
P 4050 3900
F 0 "#PWR019" H 4050 3750 50  0001 C CNN
F 1 "+5V" H 4050 4040 50  0000 C CNN
F 2 "" H 4050 3900 60  0000 C CNN
F 3 "" H 4050 3900 60  0000 C CNN
	1    4050 3900
	1    0    0    -1  
$EndComp
Text HLabel 4350 3900 2    60   Output ~ 0
CAN_5V_I
$Comp
L ISO1050 U?
U 1 1 55453F28
P 4150 4650
F 0 "U?" H 3800 5000 60  0000 C CNN
F 1 "ISO1050" H 4450 5000 60  0000 C CNN
F 2 "" H 4450 5000 60  0000 C CNN
F 3 "" H 4450 5000 60  0000 C CNN
	1    4150 4650
	1    0    0    -1  
$EndComp
Text HLabel 4300 5300 2    60   Output ~ 0
CAN_GND_I
Wire Wire Line
	4250 4150 4250 3900
Wire Wire Line
	4250 3900 4350 3900
Wire Wire Line
	4050 4150 4050 3900
Wire Wire Line
	4750 4550 4850 4550
Wire Wire Line
	4750 4750 4850 4750
Wire Wire Line
	4250 5150 4250 5300
Wire Wire Line
	4250 5300 4300 5300
Wire Wire Line
	4050 5150 4050 5300
Wire Wire Line
	3400 4450 3550 4450
Wire Wire Line
	3550 4550 3400 4550
Text HLabel 3400 2550 0    60   Input ~ 0
CAN1_TX
Text HLabel 3400 2450 0    60   Input ~ 0
CAN1_RX
Text HLabel 4850 2550 2    60   Input ~ 0
CAN1_L_I
Text HLabel 4850 2750 2    60   Input ~ 0
CAN1_H_I
$Comp
L GNDREF #PWR?
U 1 1 5545502A
P 4050 3300
F 0 "#PWR?" H 4050 3050 50  0001 C CNN
F 1 "GNDREF" H 4050 3150 50  0000 C CNN
F 2 "" H 4050 3300 60  0000 C CNN
F 3 "" H 4050 3300 60  0000 C CNN
	1    4050 3300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 55455030
P 4050 1900
F 0 "#PWR?" H 4050 1750 50  0001 C CNN
F 1 "+5V" H 4050 2040 50  0000 C CNN
F 2 "" H 4050 1900 60  0000 C CNN
F 3 "" H 4050 1900 60  0000 C CNN
	1    4050 1900
	1    0    0    -1  
$EndComp
Text HLabel 4350 1900 2    60   Output ~ 0
CAN_5V_I
$Comp
L ISO1050 U?
U 1 1 55455037
P 4150 2650
F 0 "U?" H 3800 3000 60  0000 C CNN
F 1 "ISO1050" H 4450 3000 60  0000 C CNN
F 2 "" H 4450 3000 60  0000 C CNN
F 3 "" H 4450 3000 60  0000 C CNN
	1    4150 2650
	1    0    0    -1  
$EndComp
Text HLabel 4300 3300 2    60   Output ~ 0
CAN_GND_I
Wire Wire Line
	4250 2150 4250 1900
Wire Wire Line
	4250 1900 4350 1900
Wire Wire Line
	4050 2150 4050 1900
Wire Wire Line
	4750 2550 4850 2550
Wire Wire Line
	4750 2750 4850 2750
Wire Wire Line
	4250 3150 4250 3300
Wire Wire Line
	4250 3300 4300 3300
Wire Wire Line
	4050 3150 4050 3300
Wire Wire Line
	3400 2450 3550 2450
Wire Wire Line
	3550 2550 3400 2550
$EndSCHEMATC
