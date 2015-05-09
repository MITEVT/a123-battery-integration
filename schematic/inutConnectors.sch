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
Sheet 6 7
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
L SPST SW?
U 1 1 554E6376
P 4900 2400
F 0 "SW?" H 4900 2500 50  0000 C CNN
F 1 "SPST" H 4900 2300 50  0000 C CNN
F 2 "" H 4900 2400 60  0000 C CNN
F 3 "" H 4900 2400 60  0000 C CNN
	1    4900 2400
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 554E644F
P 5700 2550
F 0 "C?" H 5725 2650 50  0000 L CNN
F 1 "C" H 5725 2450 50  0000 L CNN
F 2 "" H 5738 2400 30  0000 C CNN
F 3 "" H 5700 2550 60  0000 C CNN
	1    5700 2550
	1    0    0    -1  
$EndComp
$Comp
L 74LS14 U?
U 1 1 554E647B
P 6350 2400
F 0 "U?" H 6500 2500 40  0000 C CNN
F 1 "74LS14" H 6550 2300 40  0000 C CNN
F 2 "" H 6350 2400 60  0000 C CNN
F 3 "" H 6350 2400 60  0000 C CNN
	1    6350 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 554E64EF
P 5700 2700
F 0 "#PWR?" H 5700 2450 50  0001 C CNN
F 1 "GND" H 5700 2550 50  0000 C CNN
F 2 "" H 5700 2700 60  0000 C CNN
F 3 "" H 5700 2700 60  0000 C CNN
	1    5700 2700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 554E6515
P 5900 2550
F 0 "R?" V 5980 2550 50  0000 C CNN
F 1 "R" V 5900 2550 50  0000 C CNN
F 2 "" V 5830 2550 30  0000 C CNN
F 3 "" H 5900 2550 30  0000 C CNN
	1    5900 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2400 5900 2400
Connection ~ 5700 2400
Connection ~ 5900 2400
$Comp
L GND #PWR?
U 1 1 554E658F
P 5900 2700
F 0 "#PWR?" H 5900 2450 50  0001 C CNN
F 1 "GND" H 5900 2550 50  0000 C CNN
F 2 "" H 5900 2700 60  0000 C CNN
F 3 "" H 5900 2700 60  0000 C CNN
	1    5900 2700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 554E6653
P 5550 2400
F 0 "R?" V 5630 2400 50  0000 C CNN
F 1 "R" V 5550 2400 50  0000 C CNN
F 2 "" V 5480 2400 30  0000 C CNN
F 3 "" H 5550 2400 30  0000 C CNN
	1    5550 2400
	0    1    1    0   
$EndComp
$Comp
L SPST SW?
U 1 1 554E6B30
P 4900 3600
F 0 "SW?" H 4900 3700 50  0000 C CNN
F 1 "SPST" H 4900 3500 50  0000 C CNN
F 2 "" H 4900 3600 60  0000 C CNN
F 3 "" H 4900 3600 60  0000 C CNN
	1    4900 3600
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 554E6B36
P 5700 3750
F 0 "C?" H 5725 3850 50  0000 L CNN
F 1 "C" H 5725 3650 50  0000 L CNN
F 2 "" H 5738 3600 30  0000 C CNN
F 3 "" H 5700 3750 60  0000 C CNN
	1    5700 3750
	1    0    0    -1  
$EndComp
$Comp
L 74LS14 U?
U 1 1 554E6B3C
P 6350 3600
F 0 "U?" H 6500 3700 40  0000 C CNN
F 1 "74LS14" H 6550 3500 40  0000 C CNN
F 2 "" H 6350 3600 60  0000 C CNN
F 3 "" H 6350 3600 60  0000 C CNN
	1    6350 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 554E6B42
P 5700 3900
F 0 "#PWR?" H 5700 3650 50  0001 C CNN
F 1 "GND" H 5700 3750 50  0000 C CNN
F 2 "" H 5700 3900 60  0000 C CNN
F 3 "" H 5700 3900 60  0000 C CNN
	1    5700 3900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 554E6B48
P 5900 3750
F 0 "R?" V 5980 3750 50  0000 C CNN
F 1 "R" V 5900 3750 50  0000 C CNN
F 2 "" V 5830 3750 30  0000 C CNN
F 3 "" H 5900 3750 30  0000 C CNN
	1    5900 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3600 5900 3600
Connection ~ 5700 3600
Connection ~ 5900 3600
$Comp
L GND #PWR?
U 1 1 554E6B51
P 5900 3900
F 0 "#PWR?" H 5900 3650 50  0001 C CNN
F 1 "GND" H 5900 3750 50  0000 C CNN
F 2 "" H 5900 3900 60  0000 C CNN
F 3 "" H 5900 3900 60  0000 C CNN
	1    5900 3900
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 554E6B57
P 5550 3600
F 0 "R?" V 5630 3600 50  0000 C CNN
F 1 "R" V 5550 3600 50  0000 C CNN
F 2 "" V 5480 3600 30  0000 C CNN
F 3 "" H 5550 3600 30  0000 C CNN
	1    5550 3600
	0    1    1    0   
$EndComp
Text HLabel 6800 2400 2    60   Input ~ 0
KEYSWITCH_OUT
Text HLabel 6800 3600 2    60   Input ~ 0
CHARGESWITCH_OUT
$Comp
L +5V #PWR?
U 1 1 554E9B92
P 4400 2400
F 0 "#PWR?" H 4400 2250 50  0001 C CNN
F 1 "+5V" H 4400 2540 50  0000 C CNN
F 2 "" H 4400 2400 60  0000 C CNN
F 3 "" H 4400 2400 60  0000 C CNN
	1    4400 2400
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 554E9BBA
P 4400 3600
F 0 "#PWR?" H 4400 3450 50  0001 C CNN
F 1 "+5V" H 4400 3740 50  0000 C CNN
F 2 "" H 4400 3600 60  0000 C CNN
F 3 "" H 4400 3600 60  0000 C CNN
	1    4400 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
