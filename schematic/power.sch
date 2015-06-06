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
LIBS:MITEVT_power
LIBS:BCM-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
L SPX2920M3-3.3 U201
U 1 1 553BF7B4
P 5750 4200
F 0 "U201" H 5750 4450 40  0000 C CNN
F 1 "SPX2920M3-3.3" H 5750 4400 40  0000 C CNN
F 2 "SOT223" H 5750 4300 35  0000 C CIN
F 3 "" H 5750 4200 60  0000 C CNN
	1    5750 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 553BF801
P 5850 4700
F 0 "#PWR04" H 5850 4700 30  0001 C CNN
F 1 "GND" H 5850 4630 30  0001 C CNN
F 2 "" H 5850 4700 60  0000 C CNN
F 3 "" H 5850 4700 60  0000 C CNN
	1    5850 4700
	1    0    0    -1  
$EndComp
$Comp
L C C203
U 1 1 553BF822
P 5250 4300
F 0 "C203" H 5250 4400 40  0000 L CNN
F 1 "100n" H 5256 4215 40  0000 L CNN
F 2 "" H 5288 4150 30  0000 C CNN
F 3 "" H 5250 4300 60  0000 C CNN
	1    5250 4300
	1    0    0    -1  
$EndComp
$Comp
L C C201
U 1 1 553BF859
P 4950 4300
F 0 "C201" H 4950 4400 40  0000 L CNN
F 1 "10u" H 4956 4215 40  0000 L CNN
F 2 "" H 4988 4150 30  0000 C CNN
F 3 "" H 4950 4300 60  0000 C CNN
	1    4950 4300
	1    0    0    -1  
$EndComp
$Comp
L C C205
U 1 1 553BF886
P 6250 4300
F 0 "C205" H 6250 4400 40  0000 L CNN
F 1 "100n" H 6256 4215 40  0000 L CNN
F 2 "" H 6288 4150 30  0000 C CNN
F 3 "" H 6250 4300 60  0000 C CNN
	1    6250 4300
	1    0    0    -1  
$EndComp
$Comp
L C C207
U 1 1 553BF8AF
P 6550 4300
F 0 "C207" H 6550 4400 40  0000 L CNN
F 1 "10u" H 6556 4215 40  0000 L CNN
F 2 "" H 6588 4150 30  0000 C CNN
F 3 "" H 6550 4300 60  0000 C CNN
	1    6550 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 4150 5350 4150
Wire Wire Line
	6150 4150 6900 4150
Text HLabel 2000 4300 0    60   Input ~ 0
15V
Text HLabel 6900 4150 2    60   Output ~ 0
3.3V
Wire Wire Line
	4950 4450 6550 4450
Connection ~ 5250 4450
Connection ~ 6250 4450
Connection ~ 6550 4450
Connection ~ 4950 4450
Connection ~ 4950 4150
Connection ~ 5250 4150
Connection ~ 6150 4150
Connection ~ 6250 4150
Connection ~ 6550 4150
$Comp
L LT1934 U?
U 1 1 556AD7E1
P 2500 4450
F 0 "U?" H 2900 4950 60  0000 C CNN
F 1 "LT1934" H 2550 4050 60  0000 C CNN
F 2 "" H 2500 4450 60  0000 C CNN
F 3 "" H 2500 4450 60  0000 C CNN
	1    2500 4450
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky_Small D?
U 1 1 556ADA39
P 3500 4400
F 0 "D?" H 3450 4480 50  0000 L CNN
F 1 "MBR0540" H 3550 4350 50  0000 L CNN
F 2 "" V 3500 4400 60  0000 C CNN
F 3 "" V 3500 4400 60  0000 C CNN
	1    3500 4400
	0    -1   1    0   
$EndComp
Wire Wire Line
	3300 4500 3500 4500
$Comp
L C_Small C?
U 1 1 556ADB40
P 3500 4000
F 0 "C?" H 3510 4070 50  0000 L CNN
F 1 "0.22pF" H 3510 3920 50  0000 L CNN
F 2 "" H 3500 4000 60  0000 C CNN
F 3 "" H 3500 4000 60  0000 C CNN
	1    3500 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 4100 3500 4300
Wire Wire Line
	3500 3900 3500 3700
Wire Wire Line
	2800 3700 3650 3700
Wire Wire Line
	2800 3700 2800 3800
$Comp
L L_Small L?
U 1 1 556ADC9A
P 4000 4250
F 0 "L?" H 4030 4290 50  0000 L CNN
F 1 "47uF" H 4030 4210 50  0000 L CNN
F 2 "" H 4000 4250 60  0000 C CNN
F 3 "" H 4000 4250 60  0000 C CNN
	1    4000 4250
	0    -1   -1   0   
$EndComp
Connection ~ 3500 4300
Wire Wire Line
	3500 4300 3300 4300
Wire Wire Line
	3500 4250 3900 4250
Connection ~ 3500 4250
$Comp
L D_Schottky_Small D?
U 1 1 556ADE46
P 3750 3700
F 0 "D?" H 3700 3780 50  0000 L CNN
F 1 "D_Schottky" H 3650 3600 50  0001 L CNN
F 2 "" V 3750 3700 60  0000 C CNN
F 3 "" V 3750 3700 60  0000 C CNN
	1    3750 3700
	1    0    0    1   
$EndComp
Wire Wire Line
	3850 3700 4150 3700
Wire Wire Line
	4150 3700 4150 4800
Wire Wire Line
	4150 4250 4100 4250
Connection ~ 3500 3700
$Comp
L C_Small C?
U 1 1 556ADF10
P 4150 4900
F 0 "C?" H 4160 4970 50  0000 L CNN
F 1 "100uF" H 4160 4820 50  0000 L CNN
F 2 "" H 4150 4900 60  0000 C CNN
F 3 "" H 4150 4900 60  0000 C CNN
	1    4150 4900
	1    0    0    -1  
$EndComp
Connection ~ 4150 4250
Wire Wire Line
	2800 5000 2800 5150
Wire Wire Line
	2200 5150 4150 5150
Wire Wire Line
	4150 5150 4150 5000
$Comp
L C_Small C?
U 1 1 556AE000
P 3650 4550
F 0 "C?" H 3660 4620 50  0000 L CNN
F 1 "10pF" H 3660 4470 50  0000 L CNN
F 2 "" H 3650 4550 60  0000 C CNN
F 3 "" H 3650 4550 60  0000 C CNN
	1    3650 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4450 3650 4350
Wire Wire Line
	3650 4350 4150 4350
Connection ~ 4150 4350
$Comp
L R_Small R?
U 1 1 556AE372
P 3900 4550
F 0 "R?" H 3930 4570 50  0000 L CNN
F 1 "1M" H 3930 4510 50  0000 L CNN
F 2 "" H 3900 4550 60  0000 C CNN
F 3 "" H 3900 4550 60  0000 C CNN
	1    3900 4550
	1    0    0    -1  
$EndComp
$Comp
L R_Small R?
U 1 1 556AE3C7
P 3900 4950
F 0 "R?" H 3930 4970 50  0000 L CNN
F 1 "604K" H 3930 4910 50  0000 L CNN
F 2 "" H 3900 4950 60  0000 C CNN
F 3 "" H 3900 4950 60  0000 C CNN
	1    3900 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 4350 3900 4450
Connection ~ 3900 4350
Wire Wire Line
	3900 4650 3900 4850
Wire Wire Line
	3900 5050 3900 5150
Connection ~ 3900 5150
Wire Wire Line
	3400 4800 3900 4800
Wire Wire Line
	3650 4800 3650 4650
Connection ~ 3900 4800
Wire Wire Line
	3400 4500 3400 4800
Connection ~ 3650 4800
Connection ~ 3400 4500
Wire Wire Line
	2300 4500 2200 4500
Wire Wire Line
	2200 4500 2200 5150
Connection ~ 2800 5150
Wire Wire Line
	2000 4300 2300 4300
Connection ~ 4150 4150
Text HLabel 6900 3850 2    60   Output ~ 0
5V
Wire Wire Line
	6900 3850 4700 3850
Wire Wire Line
	4700 3850 4700 4150
Connection ~ 4700 4150
Connection ~ 5750 4450
Wire Wire Line
	5850 4450 5850 4700
Connection ~ 5850 4450
$EndSCHEMATC
