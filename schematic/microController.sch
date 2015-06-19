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
Sheet 7 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7950 2900 2    60   Input ~ 0
CAN1_RX
Text HLabel 7950 2800 2    60   Output ~ 0
CAN1_TX
Text HLabel 1700 4850 0    60   Output ~ 0
CAN2_TX
Text HLabel 1700 4750 0    60   Output ~ 0
CAN2_RX
$Comp
L SPST SW701
U 1 1 55450B97
P 1700 1900
F 0 "SW701" H 1700 2000 50  0000 C CNN
F 1 "SPST" H 1700 1800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 1700 1900 60  0001 C CNN
F 3 "" H 1700 1900 60  0000 C CNN
	1    1700 1900
	1    0    0    -1  
$EndComp
$Comp
L R R705
U 1 1 55451204
P 6900 5050
F 0 "R705" V 6980 5050 50  0000 C CNN
F 1 "330" V 6900 5050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6830 5050 30  0001 C CNN
F 3 "" H 6900 5050 30  0000 C CNN
	1    6900 5050
	1    0    0    -1  
$EndComp
$Comp
L R R704
U 1 1 55451240
P 6600 5100
F 0 "R704" V 6680 5100 50  0000 C CNN
F 1 "330" V 6600 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6530 5100 30  0001 C CNN
F 3 "" H 6600 5100 30  0000 C CNN
	1    6600 5100
	-1   0    0    1   
$EndComp
$Comp
L R R703
U 1 1 554512C6
P 6300 5200
F 0 "R703" V 6380 5200 50  0000 C CNN
F 1 "330" V 6300 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6230 5200 30  0001 C CNN
F 3 "" H 6300 5200 30  0000 C CNN
	1    6300 5200
	-1   0    0    1   
$EndComp
$Comp
L R R702
U 1 1 554512FE
P 6000 5300
F 0 "R702" V 6080 5300 50  0000 C CNN
F 1 "330" V 6000 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5930 5300 30  0001 C CNN
F 3 "" H 6000 5300 30  0000 C CNN
	1    6000 5300
	-1   0    0    1   
$EndComp
$Comp
L R R701
U 1 1 5545132E
P 5700 5400
F 0 "R701" V 5780 5400 50  0000 C CNN
F 1 "330" V 5700 5400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5630 5400 30  0001 C CNN
F 3 "" H 5700 5400 30  0000 C CNN
	1    5700 5400
	-1   0    0    1   
$EndComp
$Comp
L Led_Small D701
U 1 1 55451A1D
P 5700 5750
F 0 "D701" H 5650 5850 50  0000 L CNN
F 1 "RED" H 5650 5650 50  0001 L CNN
F 2 "LEDs:LED-0603" V 5700 5750 60  0001 C CNN
F 3 "" V 5700 5750 60  0000 C CNN
	1    5700 5750
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D702
U 1 1 55451A63
P 6000 5750
F 0 "D702" H 5950 5850 50  0000 L CNN
F 1 "RED" H 5950 5650 50  0001 L CNN
F 2 "LEDs:LED-0603" V 6000 5750 60  0001 C CNN
F 3 "" V 6000 5750 60  0000 C CNN
	1    6000 5750
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D703
U 1 1 55451ABD
P 6300 5750
F 0 "D703" H 6250 5850 50  0000 L CNN
F 1 "RED" H 6250 5650 50  0001 L CNN
F 2 "LEDs:LED-0603" V 6300 5750 60  0001 C CNN
F 3 "" V 6300 5750 60  0000 C CNN
	1    6300 5750
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D704
U 1 1 55451AF7
P 6600 5750
F 0 "D704" H 6550 5850 50  0000 L CNN
F 1 "RED" H 6550 5650 50  0001 L CNN
F 2 "LEDs:LED-0603" V 6600 5750 60  0001 C CNN
F 3 "" V 6600 5750 60  0000 C CNN
	1    6600 5750
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D705
U 1 1 55451B36
P 6900 5750
F 0 "D705" H 6850 5850 50  0000 L CNN
F 1 "RED" H 6850 5650 50  0001 L CNN
F 2 "LEDs:LED-0603" V 6900 5750 60  0001 C CNN
F 3 "" V 6900 5750 60  0000 C CNN
	1    6900 5750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR038
U 1 1 55451D92
P 6450 6300
F 0 "#PWR038" H 6450 6050 60  0001 C CNN
F 1 "GND" H 6450 6150 60  0000 C CNN
F 2 "" H 6450 6300 60  0000 C CNN
F 3 "" H 6450 6300 60  0000 C CNN
	1    6450 6300
	1    0    0    -1  
$EndComp
Text Label 5350 5250 0    60   ~ 0
IO_LED03
Text Label 5350 5150 0    60   ~ 0
IO_LED02
Text Label 5350 5050 0    60   ~ 0
IO_LED01
Text Label 5350 4950 0    60   ~ 0
IO_LED00
Text Label 5350 4850 0    60   ~ 0
IO_LED10
$Comp
L MCP2515_TSSOP U702
U 1 1 55452515
P 7250 3350
AR Path="/55452515" Ref="U702"  Part="1" 
AR Path="/553C01F1/55452515" Ref="U702"  Part="1" 
F 0 "U702" H 6800 4050 60  0000 C CNN
F 1 "MCP2515_TSSOP" H 7500 2700 60  0000 C CNN
F 2 "" H 7500 3350 60  0001 C CNN
F 3 "" H 7500 3350 60  0000 C CNN
	1    7250 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR039
U 1 1 55452E13
P 7200 4300
F 0 "#PWR039" H 7200 4050 60  0001 C CNN
F 1 "GND" H 7200 4150 60  0000 C CNN
F 2 "" H 7200 4300 60  0000 C CNN
F 3 "" H 7200 4300 60  0000 C CNN
	1    7200 4300
	1    0    0    -1  
$EndComp
$Comp
L C_Small C701
U 1 1 55452519
P 3200 1450
F 0 "C701" H 3210 1520 50  0000 L CNN
F 1 "100n" H 3210 1370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3200 1450 60  0001 C CNN
F 3 "" H 3200 1450 60  0000 C CNN
	1    3200 1450
	1    0    0    -1  
$EndComp
$Comp
L C_Small C702
U 1 1 55452581
P 3600 1450
F 0 "C702" H 3610 1520 50  0000 L CNN
F 1 "100n" H 3610 1370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3600 1450 60  0001 C CNN
F 3 "" H 3600 1450 60  0000 C CNN
	1    3600 1450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 5545270D
P 3200 1600
F 0 "#PWR040" H 3200 1350 50  0001 C CNN
F 1 "GND" H 3200 1450 50  0000 C CNN
F 2 "" H 3200 1600 60  0000 C CNN
F 3 "" H 3200 1600 60  0000 C CNN
	1    3200 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR041
U 1 1 55452747
P 3600 1600
F 0 "#PWR041" H 3600 1350 50  0001 C CNN
F 1 "GND" H 3600 1450 50  0000 C CNN
F 2 "" H 3600 1600 60  0000 C CNN
F 3 "" H 3600 1600 60  0000 C CNN
	1    3600 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR042
U 1 1 554552F0
P 3900 6100
F 0 "#PWR042" H 3900 5850 60  0001 C CNN
F 1 "GND" H 3900 5950 60  0000 C CNN
F 2 "" H 3900 6100 60  0000 C CNN
F 3 "" H 3900 6100 60  0000 C CNN
	1    3900 6100
	1    0    0    -1  
$EndComp
$Comp
L Crystal_Small Y701
U 1 1 55453068
P 8400 4400
F 0 "Y701" H 8400 4500 50  0000 C CNN
F 1 "16MHz" V 8250 4400 50  0000 C CNN
F 2 "" H 8400 4400 60  0001 C CNN
F 3 "" H 8400 4400 60  0000 C CNN
	1    8400 4400
	1    0    0    -1  
$EndComp
$Comp
L C_Small C703
U 1 1 5545338F
P 8300 4700
F 0 "C703" H 8300 4800 50  0000 L CNN
F 1 "22p" H 8400 4700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8300 4700 60  0001 C CNN
F 3 "" H 8300 4700 60  0000 C CNN
	1    8300 4700
	-1   0    0    1   
$EndComp
$Comp
L C_Small C704
U 1 1 55453499
P 8500 4700
F 0 "C704" H 8300 4800 50  0000 L CNN
F 1 "22p" H 8250 4700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8500 4700 60  0001 C CNN
F 3 "" H 8500 4700 60  0000 C CNN
	1    8500 4700
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR043
U 1 1 554536FA
P 8400 5050
F 0 "#PWR043" H 8400 4800 50  0001 C CNN
F 1 "GND" H 8400 4900 50  0000 C CNN
F 2 "" H 8400 5050 60  0000 C CNN
F 3 "" H 8400 5050 60  0000 C CNN
	1    8400 5050
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3 JP701
U 1 1 55454A9D
P 8300 3850
F 0 "JP701" H 8350 3750 50  0000 L CNN
F 1 "JUMPER3" H 8300 3950 50  0000 C BNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 8300 3850 60  0001 C CNN
F 3 "" H 8300 3850 60  0000 C CNN
	1    8300 3850
	0    1    1    0   
$EndComp
$Comp
L JUMPER3 JP702
U 1 1 5545551A
P 8700 3200
F 0 "JP702" H 8750 3100 50  0000 L CNN
F 1 "JUMPER3" H 8700 3300 50  0000 C BNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 8700 3200 60  0001 C CNN
F 3 "" H 8700 3200 60  0000 C CNN
	1    8700 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	6900 4850 5350 4850
Wire Wire Line
	5350 4950 6600 4950
Wire Wire Line
	6300 5050 5350 5050
Wire Wire Line
	5350 5250 5700 5250
Wire Wire Line
	1050 1900 1050 2050
Wire Wire Line
	1200 1900 1050 1900
Wire Wire Line
	2200 1900 2500 1900
Wire Wire Line
	2200 2350 2700 2350
Wire Wire Line
	1050 2350 1050 2450
Wire Wire Line
	1050 2350 1200 2350
Wire Wire Line
	7950 2900 7850 2900
Wire Wire Line
	7850 2800 7950 2800
Wire Wire Line
	6000 5450 6000 5650
Wire Wire Line
	6300 5350 6300 5650
Wire Wire Line
	6600 5250 6600 5650
Wire Wire Line
	6900 5200 6900 5650
Wire Wire Line
	5350 2800 6100 2800
Wire Wire Line
	6100 2800 6100 3200
Wire Wire Line
	5350 2700 6200 2700
Wire Wire Line
	6200 2700 6200 3100
Wire Wire Line
	6300 3000 6300 2600
Wire Wire Line
	6300 2600 5350 2600
Wire Wire Line
	6000 2900 6000 3300
Wire Wire Line
	6000 3300 5350 3300
Wire Wire Line
	4000 1200 4000 1650
Wire Wire Line
	3800 1250 3800 1650
Connection ~ 4000 1250
Connection ~ 3800 1250
Wire Wire Line
	3600 1350 3600 1250
Connection ~ 3600 1250
Wire Wire Line
	3200 1350 3200 1250
Wire Wire Line
	3200 1550 3200 1600
Wire Wire Line
	3600 1550 3600 1600
Wire Wire Line
	3200 1250 4000 1250
Wire Wire Line
	3900 5600 3900 6100
Wire Wire Line
	3900 5750 4100 5750
Wire Wire Line
	4100 5750 4100 5600
Connection ~ 4100 5750
Connection ~ 3900 5750
Connection ~ 8500 4400
Connection ~ 8300 4400
Wire Wire Line
	8500 4900 8500 4800
Wire Wire Line
	8300 4900 8300 4800
Wire Wire Line
	8300 4100 8300 4600
Wire Wire Line
	6550 2900 6000 2900
Wire Wire Line
	6300 3000 6550 3000
Wire Wire Line
	6200 3100 6550 3100
Wire Wire Line
	6100 3200 6550 3200
Wire Wire Line
	8500 3850 8500 4600
Wire Wire Line
	7850 3300 7950 3300
Wire Wire Line
	7950 3300 7950 3850
Wire Wire Line
	7950 3850 8200 3850
Wire Wire Line
	8500 3850 8700 3850
Wire Wire Line
	8700 3850 8700 3450
Wire Wire Line
	2500 850  2500 2250
Connection ~ 2500 1900
Wire Wire Line
	4400 1200 7050 1200
Text Label 4900 1200 0    60   ~ 0
CLKOUT
Wire Wire Line
	8700 1200 8700 2950
Wire Wire Line
	7950 1200 8700 1200
Wire Wire Line
	8300 3350 8300 3600
Wire Wire Line
	1700 4750 2700 4750
Wire Wire Line
	1700 4850 2700 4850
Wire Wire Line
	8300 4900 8500 4900
Wire Wire Line
	8400 4900 8400 5050
Connection ~ 8400 4900
Wire Wire Line
	7850 3200 8600 3200
$Comp
L GND #PWR044
U 1 1 55457C17
P 8450 3400
F 0 "#PWR044" H 8450 3150 50  0001 C CNN
F 1 "GND" H 8450 3250 50  0000 C CNN
F 2 "" H 8450 3400 60  0000 C CNN
F 3 "" H 8450 3400 60  0000 C CNN
	1    8450 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 3350 8450 3350
Wire Wire Line
	8450 3350 8450 3400
NoConn ~ 7850 3050
NoConn ~ 7850 3500
NoConn ~ 7850 3600
NoConn ~ 6550 3750
NoConn ~ 6550 3650
NoConn ~ 6550 3550
NoConn ~ 6550 3450
NoConn ~ 6550 3350
NoConn ~ 2700 2450
NoConn ~ 2700 2550
Wire Wire Line
	6000 5150 5350 5150
Wire Wire Line
	6900 4850 6900 4900
Wire Wire Line
	7200 4150 7200 4300
Wire Wire Line
	5350 4150 5550 4150
Wire Wire Line
	5550 4150 5550 4100
Text Notes 5600 4200 0    60   ~ 0
Select Serial\nProgram Mode\n
Wire Wire Line
	4400 1200 4400 850 
Wire Wire Line
	4400 850  2500 850 
Text Notes 8950 3250 0    60   ~ 0
Jumper Side 1: CLKOUT\nJumper Side 3: XTAL\n
Wire Wire Line
	6500 2800 6550 2800
Wire Wire Line
	6500 2450 6500 2800
Wire Wire Line
	7200 2400 7200 2500
Wire Wire Line
	6500 2450 7200 2450
Connection ~ 7200 2450
$Comp
L 24C16 U703
U 1 1 556A96E8
P 9100 5550
F 0 "U703" H 9250 5900 60  0000 C CNN
F 1 "24C16" H 9300 5200 60  0000 C CNN
F 2 "" H 9100 5550 60  0001 C CNN
F 3 "" H 9100 5550 60  0000 C CNN
	1    9100 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7100 5750 8400 5750
Wire Wire Line
	7100 5750 7100 4750
Wire Wire Line
	7100 4750 6250 4750
Wire Wire Line
	6250 4750 6250 3900
Wire Wire Line
	6250 3900 5350 3900
Wire Wire Line
	5350 3800 6350 3800
Wire Wire Line
	6350 3800 6350 4600
Wire Wire Line
	6350 4600 7200 4600
Wire Wire Line
	7200 4600 7200 5650
Wire Wire Line
	7200 5650 8400 5650
NoConn ~ 8400 5450
NoConn ~ 9800 5350
NoConn ~ 9800 5450
NoConn ~ 9800 5550
$Comp
L GND #PWR045
U 1 1 556AA07C
P 9100 6150
F 0 "#PWR045" H 9100 5900 50  0001 C CNN
F 1 "GND" H 9100 6000 50  0000 C CNN
F 2 "" H 9100 6150 60  0000 C CNN
F 3 "" H 9100 6150 60  0000 C CNN
	1    9100 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 6050 9100 6150
Wire Wire Line
	9100 4650 9100 5050
$Comp
L C_Small C705
U 1 1 556AA5A3
P 9500 4850
F 0 "C705" H 9510 4920 50  0000 L CNN
F 1 "0.1uF" H 9510 4770 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 9500 4850 60  0001 C CNN
F 3 "" H 9500 4850 60  0000 C CNN
	1    9500 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 4750 9500 4700
Wire Wire Line
	9500 4700 9100 4700
Connection ~ 9100 4700
$Comp
L GND #PWR046
U 1 1 556AA8E7
P 9500 5000
F 0 "#PWR046" H 9500 4750 50  0001 C CNN
F 1 "GND" H 9500 4850 50  0000 C CNN
F 2 "" H 9500 5000 60  0000 C CNN
F 3 "" H 9500 5000 60  0000 C CNN
	1    9500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 4950 9500 5000
Wire Wire Line
	5350 4650 5850 4650
Wire Wire Line
	5350 4750 5850 4750
Text HLabel 5850 4650 2    60   Output ~ 0
CON+
Text HLabel 5850 4750 2    60   Output ~ 0
CON-
Wire Wire Line
	5350 3500 5900 3500
Text HLabel 5900 3500 2    60   Output ~ 0
CON_PRE
Wire Wire Line
	6550 3850 6450 3850
Wire Wire Line
	6450 3850 6450 3300
Wire Wire Line
	6450 3300 6050 3300
Wire Wire Line
	6050 3300 6050 3000
Wire Wire Line
	6050 3000 5900 3000
Wire Wire Line
	5900 3000 5900 2900
Wire Wire Line
	5900 2900 5350 2900
NoConn ~ 2700 3450
NoConn ~ 2700 3550
NoConn ~ 2700 3650
NoConn ~ 2700 3750
NoConn ~ 2700 3850
NoConn ~ 2700 3950
NoConn ~ 2700 4050
NoConn ~ 2700 4550
NoConn ~ 2700 4650
NoConn ~ 2700 4950
NoConn ~ 2700 5050
NoConn ~ 5350 4450
NoConn ~ 5350 4550
NoConn ~ 5350 3600
NoConn ~ 5350 3400
NoConn ~ 5350 3100
Wire Wire Line
	2500 2250 2700 2250
Text Notes 2450 4650 2    60   ~ 0
A123 BUS
Text Notes 7950 2700 0    60   ~ 0
BRUSA BUS
Wire Wire Line
	5350 4350 5750 4350
Text HLabel 5750 4350 2    60   Output ~ 0
EN12V0
$Comp
L 74LS14 U401
U 3 1 557AEB63
P 7500 1200
F 0 "U401" H 7650 1300 40  0000 C CNN
F 1 "74LS14" H 7700 1100 40  0000 C CNN
F 2 "" H 7500 1200 60  0001 C CNN
F 3 "" H 7500 1200 60  0000 C CNN
	3    7500 1200
	1    0    0    -1  
$EndComp
NoConn ~ 2700 3350
$Comp
L +3.3V #PWR047
U 1 1 557B0C05
P 4000 1200
F 0 "#PWR047" H 4000 1050 50  0001 C CNN
F 1 "+3.3V" H 4000 1340 50  0000 C CNN
F 2 "" H 4000 1200 60  0000 C CNN
F 3 "" H 4000 1200 60  0000 C CNN
	1    4000 1200
	1    0    0    -1  
$EndComp
$Comp
L R_Small R706
U 1 1 557C5D5C
P 7500 5500
F 0 "R706" H 7530 5520 50  0000 L CNN
F 1 "1k" H 7530 5460 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 7500 5500 60  0001 C CNN
F 3 "" H 7500 5500 60  0000 C CNN
	1    7500 5500
	1    0    0    -1  
$EndComp
$Comp
L R_Small R707
U 1 1 557C5DAD
P 7900 5500
F 0 "R707" H 7930 5520 50  0000 L CNN
F 1 "1k" H 7930 5460 50  0000 L CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 7900 5500 60  0001 C CNN
F 3 "" H 7900 5500 60  0000 C CNN
	1    7900 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 5600 7500 5650
Connection ~ 7500 5650
Wire Wire Line
	7900 5600 7900 5750
Connection ~ 7900 5750
Wire Wire Line
	7500 5400 7500 5300
Wire Wire Line
	7900 5400 7900 5300
$Comp
L +3.3V #PWR048
U 1 1 557C6031
P 7500 5300
F 0 "#PWR048" H 7500 5150 50  0001 C CNN
F 1 "+3.3V" H 7500 5440 50  0000 C CNN
F 2 "" H 7500 5300 60  0000 C CNN
F 3 "" H 7500 5300 60  0000 C CNN
	1    7500 5300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR049
U 1 1 557C605F
P 7900 5300
F 0 "#PWR049" H 7900 5150 50  0001 C CNN
F 1 "+3.3V" H 7900 5440 50  0000 C CNN
F 2 "" H 7900 5300 60  0000 C CNN
F 3 "" H 7900 5300 60  0000 C CNN
	1    7900 5300
	1    0    0    -1  
$EndComp
Text Label 8050 1200 0    60   ~ 0
CLKOUT_BUFF
Text Label 8700 3750 0    60   ~ 0
XTAL_H
Text Label 8300 4250 2    60   ~ 0
XTAL_LOW
Text Label 7950 3300 0    60   ~ 0
CAN_OSC1
Text Label 7950 3200 0    60   ~ 0
CAN_OSC2
Text Label 5600 2600 0    60   ~ 0
CAN1_MISO
Text Label 5600 2700 0    60   ~ 0
CAN1_MOSI
Text Label 5600 2800 0    60   ~ 0
CAN1_SCK
Text Label 5450 3300 0    60   ~ 0
CAN1_SSEL
Text Label 5500 2900 0    60   ~ 0
CAN1_INT
Text Label 5600 3800 0    60   ~ 0
EEPROM_SCL
Text Label 5600 3900 0    60   ~ 0
EEPROM_SDA
Text Label 2050 4750 0    60   ~ 0
CAN2_RX
Text Label 2050 4850 0    60   ~ 0
CAN2_TX
Text Label 5500 4650 0    60   ~ 0
CON+
Text Label 5500 4750 0    60   ~ 0
CON-
Text Label 5450 3500 0    60   ~ 0
CON_PRE
$Comp
L GND #PWR050
U 1 1 557CCCC1
P 1050 2450
F 0 "#PWR050" H 1050 2200 50  0001 C CNN
F 1 "GND" H 1050 2300 50  0000 C CNN
F 2 "" H 1050 2450 60  0000 C CNN
F 3 "" H 1050 2450 60  0000 C CNN
	1    1050 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR051
U 1 1 557CCDC3
P 1050 2050
F 0 "#PWR051" H 1050 1800 50  0001 C CNN
F 1 "GND" H 1050 1900 50  0000 C CNN
F 2 "" H 1050 2050 60  0000 C CNN
F 3 "" H 1050 2050 60  0000 C CNN
	1    1050 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5150 1700 5150
Wire Wire Line
	2700 5250 1700 5250
Text HLabel 1700 5150 0    60   Input ~ 0
KEYSWITCH_IN
Text HLabel 1700 5250 0    60   Input ~ 0
CHARGESWITCH_IN
Wire Wire Line
	5700 5550 5700 5650
Wire Wire Line
	5700 5850 5700 5950
Wire Wire Line
	5700 5950 6900 5950
Wire Wire Line
	6000 5950 6000 5850
Wire Wire Line
	6300 5950 6300 5850
Connection ~ 6000 5950
Wire Wire Line
	6600 5950 6600 5850
Connection ~ 6300 5950
Wire Wire Line
	6900 5950 6900 5850
Connection ~ 6600 5950
Wire Wire Line
	6450 5950 6450 6300
Connection ~ 6450 5950
$Comp
L +3.3V #PWR052
U 1 1 557D07D4
P 5550 4100
F 0 "#PWR052" H 5550 3950 50  0001 C CNN
F 1 "+3.3V" H 5550 4240 50  0000 C CNN
F 2 "" H 5550 4100 60  0000 C CNN
F 3 "" H 5550 4100 60  0000 C CNN
	1    5550 4100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR053
U 1 1 557D0A7D
P 9100 4650
F 0 "#PWR053" H 9100 4500 50  0001 C CNN
F 1 "+3.3V" H 9100 4790 50  0000 C CNN
F 2 "" H 9100 4650 60  0000 C CNN
F 3 "" H 9100 4650 60  0000 C CNN
	1    9100 4650
	1    0    0    -1  
$EndComp
$Comp
L SPST SW702
U 1 1 557CC81E
P 1700 2350
F 0 "SW702" H 1700 2450 50  0000 C CNN
F 1 "SPST" H 1700 2250 50  0000 C CNN
F 2 "" H 1700 2350 60  0001 C CNN
F 3 "" H 1700 2350 60  0000 C CNN
	1    1700 2350
	1    0    0    -1  
$EndComp
Text Label 2250 2350 0    60   ~ 0
MCU_RESET
$Comp
L +3.3V #PWR054
U 1 1 557CCF10
P 7200 2400
F 0 "#PWR054" H 7200 2250 50  0001 C CNN
F 1 "+3.3V" H 7200 2540 50  0000 C CNN
F 2 "" H 7200 2400 60  0000 C CNN
F 3 "" H 7200 2400 60  0000 C CNN
	1    7200 2400
	1    0    0    -1  
$EndComp
$Comp
L LPC11C14 U701
U 1 1 55453AF6
P 4100 3650
AR Path="/55453AF6" Ref="U701"  Part="1" 
AR Path="/553C01F1/55453AF6" Ref="U701"  Part="1" 
F 0 "U701" H 2950 5500 60  0000 C CNN
F 1 "LPC11C14" H 4900 1850 60  0000 C CNN
F 2 "" H 4250 2450 60  0001 C CNN
F 3 "" H 4250 2450 60  0000 C CNN
	1    4100 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2350 5600 2350
Wire Wire Line
	5600 2350 5600 1650
Wire Wire Line
	5600 1650 6050 1650
Wire Wire Line
	5350 2450 5700 2450
Wire Wire Line
	5700 2450 5700 1750
Wire Wire Line
	5700 1750 6050 1750
Text HLabel 6050 1650 2    60   Input ~ 0
MCU_RXD
Text HLabel 6050 1750 2    60   Output ~ 0
MCU_TXD
$EndSCHEMATC
