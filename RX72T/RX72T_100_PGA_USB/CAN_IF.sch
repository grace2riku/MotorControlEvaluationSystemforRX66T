EESchema Schematic File Version 4
LIBS:rx72t_100_pga_usb-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
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
L analog:SN65HVD230 U?
U 1 1 61048E51
P 5650 3450
F 0 "U?" H 5650 3975 50  0000 C CNN
F 1 "SN65HVD230" H 5650 3884 50  0000 C CNN
F 2 "Package_SO:SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.514x3.2mm" H 5650 3850 50  0001 C CNN
F 3 "" H 5650 3850 50  0001 C CNN
	1    5650 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3750 5000 3750
Wire Wire Line
	5000 3750 5000 4000
$Comp
L power:GND #PWR?
U 1 1 61049C09
P 5000 4000
F 0 "#PWR?" H 5000 3750 50  0001 C CNN
F 1 "GND" H 5005 3827 50  0000 C CNN
F 2 "" H 5000 4000 50  0001 C CNN
F 3 "" H 5000 4000 50  0001 C CNN
	1    5000 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3150 5000 3150
Wire Wire Line
	5000 3150 5000 2850
$Comp
L power:+3.3V #PWR?
U 1 1 61049E6A
P 5000 2850
F 0 "#PWR?" H 5000 2700 50  0001 C CNN
F 1 "+3.3V" H 5015 3023 50  0000 C CNN
F 2 "" H 5000 2850 50  0001 C CNN
F 3 "" H 5000 2850 50  0001 C CNN
	1    5000 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3350 4500 3350
Wire Wire Line
	5300 3550 4500 3550
Text HLabel 4500 3350 0    50   Input ~ 0
CTX
Text HLabel 4500 3550 0    50   Output ~ 0
CRX
Wire Wire Line
	6000 3350 6500 3350
Wire Wire Line
	6500 3350 6500 3000
$Comp
L akizuki:R R?
U 1 1 6104AF6C
P 7000 3300
F 0 "R?" V 6954 3370 50  0000 L CNN
F 1 "120" V 7045 3370 50  0000 L CNN
F 2 "" H 7000 3230 50  0000 C CNN
F 3 "" V 7000 3300 50  0000 C CNN
	1    7000 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 3150 6250 3150
Wire Wire Line
	6250 3150 6250 2500
Wire Wire Line
	6250 2500 6500 2500
$Comp
L akizuki:R R?
U 1 1 6104D823
P 6650 2500
F 0 "R?" H 6650 2707 50  0000 C CNN
F 1 "47K" H 6650 2616 50  0000 C CNN
F 2 "" H 6650 2430 50  0000 C CNN
F 3 "" V 6650 2500 50  0000 C CNN
	1    6650 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 2500 7000 2500
Wire Wire Line
	7000 2500 7000 2700
$Comp
L power:GND #PWR?
U 1 1 6104DD41
P 7000 2700
F 0 "#PWR?" H 7000 2450 50  0001 C CNN
F 1 "GND" H 7005 2527 50  0000 C CNN
F 2 "" H 7000 2700 50  0001 C CNN
F 3 "" H 7000 2700 50  0001 C CNN
	1    7000 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 S?
U 1 1 6104DEE7
P 6650 2000
F 0 "S?" H 6730 2042 50  0000 L CNN
F 1 "T-POW" H 6730 1951 50  0000 L CNN
F 2 "" H 6650 2000 50  0001 C CNN
F 3 "~" H 6650 2000 50  0001 C CNN
	1    6650 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2000 6250 2000
Wire Wire Line
	6250 2000 6250 2500
Connection ~ 6250 2500
Wire Wire Line
	6450 2100 6000 2100
Wire Wire Line
	6000 2100 6000 2300
Wire Wire Line
	6450 1900 6000 1900
Wire Wire Line
	6000 1900 6000 1700
$Comp
L power:GND #PWR?
U 1 1 6104FFD1
P 6000 2300
F 0 "#PWR?" H 6000 2050 50  0001 C CNN
F 1 "GND" H 6005 2127 50  0000 C CNN
F 2 "" H 6000 2300 50  0001 C CNN
F 3 "" H 6000 2300 50  0001 C CNN
	1    6000 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61050129
P 6000 1700
F 0 "#PWR?" H 6000 1550 50  0001 C CNN
F 1 "+3.3V" H 6015 1873 50  0000 C CNN
F 2 "" H 6000 1700 50  0001 C CNN
F 3 "" H 6000 1700 50  0001 C CNN
	1    6000 1700
	1    0    0    -1  
$EndComp
Text Notes 7050 2100 0    50   ~ 0
HVD230:\n+3.3V: Low Power\nOpen: Slope Controle Mode\nGND: High Speed Mode
NoConn ~ 6000 3750
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 61050A48
P 9050 3000
F 0 "J?" H 9130 2992 50  0000 L CNN
F 1 "CAN" H 9130 2901 50  0000 L CNN
F 2 "" H 9050 3000 50  0001 C CNN
F 3 "~" H 9050 3000 50  0001 C CNN
	1    9050 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 3200 8700 3200
Wire Wire Line
	8700 3200 8700 3500
$Comp
L power:GND #PWR?
U 1 1 61053573
P 8700 3500
F 0 "#PWR?" H 8700 3250 50  0001 C CNN
F 1 "GND" H 8705 3327 50  0000 C CNN
F 2 "" H 8700 3500 50  0001 C CNN
F 3 "" H 8700 3500 50  0001 C CNN
	1    8700 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 2900 8700 2900
$Comp
L Connector_Generic:Conn_01x03 S?
U 1 1 6107BF16
P 9050 2100
F 0 "S?" H 9130 2142 50  0000 L CNN
F 1 "CAN-POW" H 9130 2051 50  0000 L CNN
F 2 "" H 9050 2100 50  0001 C CNN
F 3 "~" H 9050 2100 50  0001 C CNN
	1    9050 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 2100 8700 2100
Wire Wire Line
	8700 2100 8700 2900
Wire Wire Line
	8850 2000 8700 2000
Wire Wire Line
	8700 2000 8700 1800
$Comp
L power:+3.3V #PWR?
U 1 1 6107D4BA
P 8700 1800
F 0 "#PWR?" H 8700 1650 50  0001 C CNN
F 1 "+3.3V" H 8715 1973 50  0000 C CNN
F 2 "" H 8700 1800 50  0001 C CNN
F 3 "" H 8700 1800 50  0001 C CNN
	1    8700 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 2200 8500 2200
$Comp
L power:+12V #PWR?
U 1 1 6107F433
P 8500 2200
F 0 "#PWR?" H 8500 2050 50  0001 C CNN
F 1 "+12V" V 8515 2328 50  0000 L CNN
F 2 "" H 8500 2200 50  0001 C CNN
F 3 "" H 8500 2200 50  0001 C CNN
	1    8500 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6500 3000 7000 3000
Wire Wire Line
	6000 3550 7000 3550
Wire Wire Line
	7500 3550 7500 3100
Wire Wire Line
	7500 3100 8850 3100
Wire Wire Line
	7000 3150 7000 3000
Connection ~ 7000 3000
Wire Wire Line
	7000 3000 8850 3000
Wire Wire Line
	7000 3450 7000 3550
Connection ~ 7000 3550
Wire Wire Line
	7000 3550 7500 3550
$Comp
L akizuki:C C?
U 1 1 61DD7B55
P 3150 3400
F 0 "C?" H 3233 3453 60  0000 L CNN
F 1 "0.1uF" H 3233 3347 60  0000 L CNN
F 2 "" V 3150 3400 60  0000 C CNN
F 3 "" V 3150 3400 60  0000 C CNN
	1    3150 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 3250 3150 3050
Wire Wire Line
	3150 3550 3150 3750
$Comp
L power:GND #PWR?
U 1 1 61DD8C6E
P 3150 3750
F 0 "#PWR?" H 3150 3500 50  0001 C CNN
F 1 "GND" H 3155 3577 50  0000 C CNN
F 2 "" H 3150 3750 50  0001 C CNN
F 3 "" H 3150 3750 50  0001 C CNN
	1    3150 3750
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61DD8E3D
P 3150 3050
F 0 "#PWR?" H 3150 2900 50  0001 C CNN
F 1 "+3.3V" H 3165 3223 50  0000 C CNN
F 2 "" H 3150 3050 50  0001 C CNN
F 3 "" H 3150 3050 50  0001 C CNN
	1    3150 3050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
