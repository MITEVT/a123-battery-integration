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
Text HLabel 7950 3550 2    60   Output ~ 0
CAN1_RX
Text HLabel 7950 3450 2    60   Output ~ 0
CAN1_TX
Text HLabel 5500 3100 2    60   Output ~ 0
CAN2_TX
Text HLabel 5500 3000 2    60   Output ~ 0
CAN2_RX
Text HLabel 1850 4000 0    60   Output ~ 0
HVSENSE_IN
$Comp
L SPST SW?
U 1 1 553C56D1
P 1900 3000
F 0 "SW?" H 1900 3100 70  0000 C CNN
F 1 "SPST" H 1900 2900 70  0000 C CNN
F 2 "" H 1900 3000 60  0000 C CNN
F 3 "" H 1900 3000 60  0000 C CNN
	1    1900 3000
	1    0    0    -1  
$EndComp
$Comp
L SPST SW?
U 1 1 55450B97
P 1900 2550
F 0 "SW?" H 1900 2650 50  0000 C CNN
F 1 "SPST" H 1900 2450 50  0000 C CNN
F 2 "" H 1900 2550 60  0000 C CNN
F 3 "" H 1900 2550 60  0000 C CNN
	1    1900 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55450C9C
P 1050 2700
F 0 "#PWR?" H 1050 2450 60  0001 C CNN
F 1 "GND" H 1050 2550 60  0000 C CNN
F 2 "" H 1050 2700 60  0000 C CNN
F 3 "" H 1050 2700 60  0000 C CNN
	1    1050 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3VP #PWR?
U 1 1 55450EA2
P 4000 1850
F 0 "#PWR?" H 4050 1880 20  0001 C CNN
F 1 "+3.3VP" H 4000 1940 30  0000 C CNN
F 2 "" H 4000 1850 60  0000 C CNN
F 3 "" H 4000 1850 60  0000 C CNN
	1    4000 1850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 55450EC2
P 4400 1850
F 0 "#PWR?" H 4400 1700 60  0001 C CNN
F 1 "+5V" H 4400 1990 60  0000 C CNN
F 2 "" H 4400 1850 60  0000 C CNN
F 3 "" H 4400 1850 60  0000 C CNN
	1    4400 1850
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55451204
P 6900 5650
F 0 "R?" V 6980 5650 50  0000 C CNN
F 1 "R" V 6900 5650 50  0000 C CNN
F 2 "" V 6830 5650 30  0000 C CNN
F 3 "" H 6900 5650 30  0000 C CNN
	1    6900 5650
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55451240
P 6600 5750
F 0 "R?" V 6680 5750 50  0000 C CNN
F 1 "R" V 6600 5750 50  0000 C CNN
F 2 "" V 6530 5750 30  0000 C CNN
F 3 "" H 6600 5750 30  0000 C CNN
	1    6600 5750
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 554512C6
P 6300 5850
F 0 "R?" V 6380 5850 50  0000 C CNN
F 1 "R" V 6300 5850 50  0000 C CNN
F 2 "" V 6230 5850 30  0000 C CNN
F 3 "" H 6300 5850 30  0000 C CNN
	1    6300 5850
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 554512FE
P 6000 5950
F 0 "R?" V 6080 5950 50  0000 C CNN
F 1 "R" V 6000 5950 50  0000 C CNN
F 2 "" V 5930 5950 30  0000 C CNN
F 3 "" H 6000 5950 30  0000 C CNN
	1    6000 5950
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 5545132E
P 5700 6050
F 0 "R?" V 5780 6050 50  0000 C CNN
F 1 "R" V 5700 6050 50  0000 C CNN
F 2 "" V 5630 6050 30  0000 C CNN
F 3 "" H 5700 6050 30  0000 C CNN
	1    5700 6050
	-1   0    0    1   
$EndComp
$Comp
L Led_Small D?
U 1 1 55451A1D
P 5700 6300
F 0 "D?" H 5650 6400 50  0000 L CNN
F 1 "RED" H 5650 6200 50  0001 L CNN
F 2 "" V 5700 6300 60  0000 C CNN
F 3 "" V 5700 6300 60  0000 C CNN
	1    5700 6300
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D?
U 1 1 55451A63
P 6000 6300
F 0 "D?" H 5950 6400 50  0000 L CNN
F 1 "RED" H 5950 6200 50  0001 L CNN
F 2 "" V 6000 6300 60  0000 C CNN
F 3 "" V 6000 6300 60  0000 C CNN
	1    6000 6300
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D?
U 1 1 55451ABD
P 6300 6300
F 0 "D?" H 6250 6400 50  0000 L CNN
F 1 "RED" H 6250 6200 50  0001 L CNN
F 2 "" V 6300 6300 60  0000 C CNN
F 3 "" V 6300 6300 60  0000 C CNN
	1    6300 6300
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D?
U 1 1 55451AF7
P 6600 6300
F 0 "D?" H 6550 6400 50  0000 L CNN
F 1 "RED" H 6550 6200 50  0001 L CNN
F 2 "" V 6600 6300 60  0000 C CNN
F 3 "" V 6600 6300 60  0000 C CNN
	1    6600 6300
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D?
U 1 1 55451B36
P 6900 6300
F 0 "D?" H 6850 6400 50  0000 L CNN
F 1 "RED" H 6850 6200 50  0001 L CNN
F 2 "" V 6900 6300 60  0000 C CNN
F 3 "" V 6900 6300 60  0000 C CNN
	1    6900 6300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 55451D92
P 5700 6400
F 0 "#PWR?" H 5700 6150 60  0001 C CNN
F 1 "GND" H 5700 6250 60  0000 C CNN
F 2 "" H 5700 6400 60  0000 C CNN
F 3 "" H 5700 6400 60  0000 C CNN
	1    5700 6400
	1    0    0    -1  
$EndComp
Text Label 5350 5900 0    60   ~ 0
IO_LED03
Text Label 5350 5800 0    60   ~ 0
IO_LED02
Text Label 5350 5700 0    60   ~ 0
IO_LED01
Text Label 5350 5600 0    60   ~ 0
IO_LED00
Text Label 5350 5500 0    60   ~ 0
IO_LED10
$Comp
L +3.3VP #PWR?
U 1 1 55452278
P 5350 4800
F 0 "#PWR?" H 5400 4830 20  0001 C CNN
F 1 "+3.3VP" H 5350 4890 30  0000 C CNN
F 2 "" H 5350 4800 60  0000 C CNN
F 3 "" H 5350 4800 60  0000 C CNN
	1    5350 4800
	1    0    0    -1  
$EndComp
$Comp
L MCP2515_TSSOP U?
U 1 1 55452515
P 7150 4000
F 0 "U?" H 6700 4700 60  0000 C CNN
F 1 "MCP2515_TSSOP" H 7400 3350 60  0000 C CNN
F 2 "" H 7400 4000 60  0000 C CNN
F 3 "" H 7400 4000 60  0000 C CNN
	1    7150 4000
	1    0    0    -1  
$EndComp
$Comp
L +3.3VP #PWR?
U 1 1 55452DDD
P 7100 3150
F 0 "#PWR?" H 7150 3180 20  0001 C CNN
F 1 "+3.3VP" H 7100 3240 30  0000 C CNN
F 2 "" H 7100 3150 60  0000 C CNN
F 3 "" H 7100 3150 60  0000 C CNN
	1    7100 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55452E13
P 7100 4800
F 0 "#PWR?" H 7100 4550 60  0001 C CNN
F 1 "GND" H 7100 4650 60  0000 C CNN
F 2 "" H 7100 4800 60  0000 C CNN
F 3 "" H 7100 4800 60  0000 C CNN
	1    7100 4800
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 55452519
P 3200 2050
F 0 "C?" H 3210 2120 50  0000 L CNN
F 1 "100n" H 3210 1970 50  0000 L CNN
F 2 "" H 3200 2050 60  0000 C CNN
F 3 "" H 3200 2050 60  0000 C CNN
	1    3200 2050
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 55452581
P 3600 2050
F 0 "C?" H 3610 2120 50  0000 L CNN
F 1 "100n" H 3610 1970 50  0000 L CNN
F 2 "" H 3600 2050 60  0000 C CNN
F 3 "" H 3600 2050 60  0000 C CNN
	1    3600 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5545270D
P 3200 2250
F 0 "#PWR?" H 3200 2000 50  0001 C CNN
F 1 "GND" H 3200 2100 50  0000 C CNN
F 2 "" H 3200 2250 60  0000 C CNN
F 3 "" H 3200 2250 60  0000 C CNN
	1    3200 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55452747
P 3600 2250
F 0 "#PWR?" H 3600 2000 50  0001 C CNN
F 1 "GND" H 3600 2100 50  0000 C CNN
F 2 "" H 3600 2250 60  0000 C CNN
F 3 "" H 3600 2250 60  0000 C CNN
	1    3600 2250
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 55452B87
P 4650 2050
F 0 "C?" H 4660 2120 50  0000 L CNN
F 1 "100n" H 4660 1970 50  0000 L CNN
F 2 "" H 4650 2050 60  0000 C CNN
F 3 "" H 4650 2050 60  0000 C CNN
	1    4650 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55452D83
P 4650 2250
F 0 "#PWR?" H 4650 2000 50  0001 C CNN
F 1 "GND" H 4650 2100 50  0000 C CNN
F 2 "" H 4650 2250 60  0000 C CNN
F 3 "" H 4650 2250 60  0000 C CNN
	1    4650 2250
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 55452E98
P 4900 2050
F 0 "C?" H 4910 2120 50  0000 L CNN
F 1 "100n" H 4910 1970 50  0000 L CNN
F 2 "" H 4900 2050 60  0000 C CNN
F 3 "" H 4900 2050 60  0000 C CNN
	1    4900 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55452FBD
P 4900 2250
F 0 "#PWR?" H 4900 2000 50  0001 C CNN
F 1 "GND" H 4900 2100 50  0000 C CNN
F 2 "" H 4900 2250 60  0000 C CNN
F 3 "" H 4900 2250 60  0000 C CNN
	1    4900 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55454D7C
P 1050 3100
F 0 "#PWR?" H 1050 2850 60  0001 C CNN
F 1 "GND" H 1050 2950 60  0000 C CNN
F 2 "" H 1050 3100 60  0000 C CNN
F 3 "" H 1050 3100 60  0000 C CNN
	1    1050 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 554552F0
P 3900 6750
F 0 "#PWR?" H 3900 6500 60  0001 C CNN
F 1 "GND" H 3900 6600 60  0000 C CNN
F 2 "" H 3900 6750 60  0000 C CNN
F 3 "" H 3900 6750 60  0000 C CNN
	1    3900 6750
	1    0    0    -1  
$EndComp
$Comp
L Crystal_Small Y?
U 1 1 55453068
P 8400 4750
F 0 "Y?" H 8400 4850 50  0000 C CNN
F 1 "16MHz" V 8250 4750 50  0000 C CNN
F 2 "" H 8400 4750 60  0000 C CNN
F 3 "" H 8400 4750 60  0000 C CNN
	1    8400 4750
	1    0    0    -1  
$EndComp
$Comp
L C_Small C?
U 1 1 5545338F
P 8300 5050
F 0 "C?" H 8310 5120 50  0000 L CNN
F 1 "22p" V 8450 5000 50  0000 L CNN
F 2 "" H 8300 5050 60  0000 C CNN
F 3 "" H 8300 5050 60  0000 C CNN
	1    8300 5050
	-1   0    0    1   
$EndComp
$Comp
L C_Small C?
U 1 1 55453499
P 8500 5050
F 0 "C?" H 8510 5120 50  0000 L CNN
F 1 "22p" V 8400 5000 50  0000 L CNN
F 2 "" H 8500 5050 60  0000 C CNN
F 3 "" H 8500 5050 60  0000 C CNN
	1    8500 5050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 554536FA
P 8150 5250
F 0 "#PWR?" H 8150 5000 50  0001 C CNN
F 1 "GND" H 8150 5100 50  0000 C CNN
F 2 "" H 8150 5250 60  0000 C CNN
F 3 "" H 8150 5250 60  0000 C CNN
	1    8150 5250
	0    1    1    0   
$EndComp
$Comp
L JUMPER3 JP?
U 1 1 55454A9D
P 8300 4200
F 0 "JP?" H 8350 4100 50  0000 L CNN
F 1 "JUMPER3" H 8300 4300 50  0000 C BNN
F 2 "" H 8300 4200 60  0000 C CNN
F 3 "" H 8300 4200 60  0000 C CNN
	1    8300 4200
	0    1    1    0   
$EndComp
$Comp
L JUMPER3 JP?
U 1 1 5545551A
P 8700 4200
F 0 "JP?" H 8750 4100 50  0000 L CNN
F 1 "JUMPER3" H 8700 4300 50  0000 C BNN
F 2 "" H 8700 4200 60  0000 C CNN
F 3 "" H 8700 4200 60  0000 C CNN
	1    8700 4200
	0    1    1    0   
$EndComp
$Comp
L 74LS365 U?
U 1 1 55455737
P 7750 2250
F 0 "U?" H 7750 2250 60  0000 C CNN
F 1 "74LS365" H 7800 2050 60  0000 C CNN
F 2 "" H 7750 2250 60  0000 C CNN
F 3 "" H 7750 2250 60  0000 C CNN
	1    7750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5500 5350 5500
Wire Wire Line
	5350 5600 6600 5600
Wire Wire Line
	6300 5700 5350 5700
Wire Wire Line
	5350 5800 6000 5800
Wire Wire Line
	5350 5900 5700 5900
Connection ~ 1050 2700
Wire Wire Line
	1050 2550 1050 2700
Wire Wire Line
	1400 2550 1050 2550
Connection ~ 1400 2550
Wire Wire Line
	2400 2550 2700 2550
Wire Wire Line
	2700 2550 2700 2900
Connection ~ 1400 3000
Connection ~ 1050 3100
Wire Wire Line
	2400 3000 2700 3000
Connection ~ 3900 6250
Connection ~ 4100 6250
Wire Wire Line
	1050 3000 1050 3100
Wire Wire Line
	1050 3000 1400 3000
Wire Wire Line
	7950 3550 7750 3550
Wire Wire Line
	7750 3450 7950 3450
Wire Wire Line
	2700 4000 1850 4000
Wire Wire Line
	6000 6100 6000 6200
Wire Wire Line
	6300 6200 6300 6000
Wire Wire Line
	6600 5900 6600 6200
Wire Wire Line
	6900 6200 6900 5800
Wire Wire Line
	5700 6400 6900 6400
Connection ~ 6000 6400
Connection ~ 6300 6400
Connection ~ 6600 6400
Connection ~ 5700 6400
Connection ~ 5700 6200
Connection ~ 6000 6200
Connection ~ 6000 6100
Connection ~ 6300 6000
Connection ~ 6300 6200
Connection ~ 6600 6200
Connection ~ 6600 5900
Connection ~ 6900 5800
Connection ~ 6900 6200
Connection ~ 6900 6400
Connection ~ 5700 5900
Connection ~ 6000 5800
Connection ~ 6300 5700
Connection ~ 6600 5600
Connection ~ 6900 5500
Connection ~ 5350 4800
Wire Wire Line
	5350 3450 6100 3450
Wire Wire Line
	6100 3450 6100 3850
Wire Wire Line
	5350 3350 6200 3350
Wire Wire Line
	6200 3350 6200 3750
Wire Wire Line
	6300 3650 6300 3250
Wire Wire Line
	6300 3250 5350 3250
Wire Wire Line
	6000 3550 6000 3950
Wire Wire Line
	6000 3950 5350 3950
Connection ~ 7100 4800
Connection ~ 7100 3150
Wire Wire Line
	4000 1850 4000 2300
Wire Wire Line
	3800 1900 3800 2300
Connection ~ 4000 1900
Connection ~ 3800 1900
Wire Wire Line
	3600 1950 3600 1900
Connection ~ 3600 1900
Wire Wire Line
	3200 1950 3200 1900
Connection ~ 3200 1900
Wire Wire Line
	3200 2150 3200 2250
Wire Wire Line
	3600 2150 3600 2250
Wire Wire Line
	4400 1900 4400 1850
Wire Wire Line
	4400 1900 4900 1900
Connection ~ 4400 1900
Wire Wire Line
	4650 1900 4650 1950
Wire Wire Line
	4650 2150 4650 2250
Wire Wire Line
	4900 1900 4900 1950
Connection ~ 4650 1900
Wire Wire Line
	4900 2150 4900 2250
Wire Wire Line
	3200 1900 4000 1900
Connection ~ 4000 2000
Wire Wire Line
	3900 6250 3900 6750
Wire Wire Line
	3900 6400 4100 6400
Wire Wire Line
	4100 6400 4100 6250
Connection ~ 4100 6400
Connection ~ 3900 6400
Connection ~ 8500 4750
Connection ~ 8300 4750
Wire Wire Line
	8500 5250 8500 5150
Wire Wire Line
	8150 5250 8500 5250
Wire Wire Line
	8300 5250 8300 5150
Connection ~ 8300 5250
Wire Wire Line
	8300 4450 8300 4950
Wire Wire Line
	6450 3550 6000 3550
Wire Wire Line
	6300 3650 6450 3650
Wire Wire Line
	6200 3750 6450 3750
Wire Wire Line
	6100 3850 6450 3850
Wire Wire Line
	8500 4500 8500 4950
Wire Wire Line
	7750 3850 8550 3850
Wire Wire Line
	7750 3950 8150 3950
Wire Wire Line
	8150 3950 8150 4200
Wire Wire Line
	8150 4200 8200 4200
Wire Wire Line
	8550 3850 8550 4200
Wire Wire Line
	8550 4200 8600 4200
Wire Wire Line
	8500 4500 8700 4500
Wire Wire Line
	8700 4500 8700 4450
Wire Wire Line
	2500 2550 2500 2300
Wire Wire Line
	2500 2300 2700 2300
Connection ~ 2500 2550
Text Label 2500 2300 0    60   ~ 0
CLKOUT
Wire Wire Line
	7000 2650 7050 2650
Wire Wire Line
	7000 2550 7000 2750
Wire Wire Line
	6950 2550 7050 2550
$Comp
L GND #PWR?
U 1 1 55457597
P 7000 2750
F 0 "#PWR?" H 7000 2500 50  0001 C CNN
F 1 "GND" H 7000 2600 50  0000 C CNN
F 2 "" H 7000 2750 60  0000 C CNN
F 3 "" H 7000 2750 60  0000 C CNN
	1    7000 2750
	1    0    0    -1  
$EndComp
Connection ~ 7000 2650
Wire Wire Line
	7050 1950 6950 1950
Wire Wire Line
	6950 1950 6950 2550
Wire Wire Line
	6950 2350 7050 2350
Wire Wire Line
	7050 2250 6950 2250
Connection ~ 6950 2250
Wire Wire Line
	7050 2150 6950 2150
Connection ~ 6950 2150
Wire Wire Line
	7050 2050 6950 2050
Connection ~ 6950 2050
Connection ~ 7000 2550
Connection ~ 6950 2350
Wire Wire Line
	6800 1850 7050 1850
Text Label 6800 1850 0    60   ~ 0
CLKOUT
Wire Wire Line
	8700 3950 8700 1850
Wire Wire Line
	8700 1850 8450 1850
$Comp
L GND #PWR?
U 1 1 55457C17
P 8450 3950
F 0 "#PWR?" H 8450 3700 50  0001 C CNN
F 1 "GND" H 8450 3800 50  0000 C CNN
F 2 "" H 8450 3950 60  0000 C CNN
F 3 "" H 8450 3950 60  0000 C CNN
	1    8450 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 3950 8300 3900
Wire Wire Line
	8300 3900 8450 3900
Wire Wire Line
	8450 3900 8450 3950
$Comp
L LPC11C14 U?
U 1 1 55453AF6
P 4100 4300
F 0 "U?" H 2950 6150 60  0000 C CNN
F 1 "LPC11C14" H 4900 2500 60  0000 C CNN
F 2 "" H 4250 3100 60  0000 C CNN
F 3 "" H 4250 3100 60  0000 C CNN
	1    4100 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3000 5350 3000
Wire Wire Line
	5350 3100 5500 3100
$EndSCHEMATC
