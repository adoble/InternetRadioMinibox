EESchema Schematic File Version 2
LIBS:InternetRadioMiniBox-rescue
LIBS:InternetRadio-rescue
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
LIBS:maker
LIBS:InternetRadioMiniBox-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Internet Radio Minibox"
Date "2018-12-28"
Rev "8"
Comp ""
Comment1 "Design: Andrew Doble"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Entry Wire Line
	5800 6200 5900 6300
Text Label 5400 3850 0    60   ~ 0
SPI-SCK
Entry Wire Line
	6100 6200 6200 6300
Text Label 5650 3650 0    60   ~ 0
SPI-MOSI
Entry Wire Line
	5950 6200 6050 6300
Text Notes 11900 10300 0    118  ~ 24
Internet Radio
$Comp
L ATMEGA328P-P IC2
U 1 1 58A9BCF5
P 9300 8350
F 0 "IC2" H 8550 9600 50  0000 L BNN
F 1 "ATMEGA328P-P" H 9100 6950 50  0000 L BNN
F 2 "" H 9300 8350 50  0001 C CIN
F 3 "" H 9300 8350 50  0000 C CNN
	1    9300 8350
	1    0    0    -1  
$EndComp
$Comp
L Crystal Y1
U 1 1 58A9BE54
P 12150 8050
F 0 "Y1" H 12150 8200 50  0000 C CNN
F 1 "12 MHz" H 12150 7900 50  0000 C CNN
F 2 "" H 12150 8050 50  0001 C CNN
F 3 "" H 12150 8050 50  0000 C CNN
	1    12150 8050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 58A9C059
P 11800 8250
F 0 "C1" H 11825 8350 50  0000 L CNN
F 1 "22p" H 11825 8150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 11838 8100 50  0001 C CNN
F 3 "" H 11800 8250 50  0000 C CNN
	1    11800 8250
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 58A9ED24
P 12450 8250
F 0 "C2" H 12475 8350 50  0000 L CNN
F 1 "22p" H 12475 8150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12488 8100 50  0001 C CNN
F 3 "" H 12450 8250 50  0000 C CNN
	1    12450 8250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR33
U 1 1 58A9F06A
P 11800 8400
F 0 "#PWR33" H 11800 8150 50  0001 C CNN
F 1 "GND" H 11800 8250 50  0000 C CNN
F 2 "" H 11800 8400 50  0000 C CNN
F 3 "" H 11800 8400 50  0000 C CNN
	1    11800 8400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR34
U 1 1 58A9F0C1
P 12450 8400
F 0 "#PWR34" H 12450 8150 50  0001 C CNN
F 1 "GND" H 12450 8250 50  0000 C CNN
F 2 "" H 12450 8400 50  0000 C CNN
F 3 "" H 12450 8400 50  0000 C CNN
	1    12450 8400
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 58A9F415
P 7700 7400
F 0 "C3" H 7725 7500 50  0000 L CNN
F 1 "100n" H 7725 7300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7738 7250 50  0001 C CNN
F 3 "" H 7700 7400 50  0000 C CNN
	1    7700 7400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR18
U 1 1 58A9F6B3
P 7700 9750
F 0 "#PWR18" H 7700 9500 50  0001 C CNN
F 1 "GND" H 7700 9600 50  0000 C CNN
F 2 "" H 7700 9750 50  0000 C CNN
F 3 "" H 7700 9750 50  0000 C CNN
	1    7700 9750
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 58A9F84D
P 8150 8350
F 0 "C4" H 8175 8450 50  0000 L CNN
F 1 "100n" H 8175 8250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 8200 50  0001 C CNN
F 3 "" H 8150 8350 50  0000 C CNN
	1    8150 8350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 58A9F9CC
P 8150 9750
F 0 "#PWR20" H 8150 9500 50  0001 C CNN
F 1 "GND" H 8150 9600 50  0000 C CNN
F 2 "" H 8150 9750 50  0000 C CNN
F 3 "" H 8150 9750 50  0000 C CNN
	1    8150 9750
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 58A9FCD9
P 10500 7000
F 0 "R4" V 10580 7000 50  0000 C CNN
F 1 "10K" V 10500 7000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 10430 7000 50  0001 C CNN
F 3 "" H 10500 7000 50  0000 C CNN
	1    10500 7000
	1    0    0    -1  
$EndComp
Text Label 10950 7750 0    60   ~ 0
SPI-SCK
Text Label 10950 7650 0    60   ~ 0
SPI-MISO
Text Label 10950 7550 0    60   ~ 0
SPI-MOSI
$Comp
L C C5
U 1 1 58AAC77D
P 15150 9150
F 0 "C5" H 15175 9250 50  0000 L CNN
F 1 "22p" H 15250 9150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 15188 9000 50  0001 C CNN
F 3 "" H 15150 9150 50  0000 C CNN
	1    15150 9150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR40
U 1 1 58AAC8B5
P 15150 9450
F 0 "#PWR40" H 15150 9200 50  0001 C CNN
F 1 "GND" H 15150 9300 50  0000 C CNN
F 2 "" H 15150 9450 50  0000 C CNN
F 3 "" H 15150 9450 50  0000 C CNN
	1    15150 9450
	1    0    0    -1  
$EndComp
Text Notes 15450 8600 0    39   ~ 0
To 0-135pF Rotary \nVariable Capacitor.
$Comp
L GND #PWR39
U 1 1 58AB28EA
P 14200 8100
F 0 "#PWR39" H 14200 7850 50  0001 C CNN
F 1 "GND" H 14200 7950 50  0000 C CNN
F 2 "" H 14200 8100 50  0000 C CNN
F 3 "" H 14200 8100 50  0000 C CNN
	1    14200 8100
	1    0    0    -1  
$EndComp
$Comp
L LED-RESCUE-InternetRadioMiniBox D2
U 1 1 58AB9582
P 11200 9450
F 0 "D2" H 11200 9550 50  0000 C CNN
F 1 "RED" H 11200 9350 50  0000 C CNN
F 2 "LEDs:LED_1206" H 11200 9450 50  0001 C CNN
F 3 "" H 11200 9450 50  0000 C CNN
	1    11200 9450
	-1   0    0    -1  
$EndComp
$Comp
L LED-RESCUE-InternetRadioMiniBox D1
U 1 1 58AB95F9
P 11200 9150
F 0 "D1" H 11200 9250 50  0000 C CNN
F 1 "GREEN" H 11200 9050 50  0000 C CNN
F 2 "LEDs:LED_1206" H 11200 9150 50  0001 C CNN
F 3 "" H 11200 9150 50  0000 C CNN
	1    11200 9150
	-1   0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 58ABC405
P 11500 9150
F 0 "R5" V 11580 9150 50  0000 C CNN
F 1 "165" V 11500 9150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 11430 9150 50  0001 C CNN
F 3 "" H 11500 9150 50  0000 C CNN
	1    11500 9150
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 58ABC4EC
P 11500 9450
F 0 "R6" V 11580 9450 50  0000 C CNN
F 1 "165" V 11500 9450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 11430 9450 50  0001 C CNN
F 3 "" H 11500 9450 50  0000 C CNN
	1    11500 9450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR32
U 1 1 58ABC5CC
P 11650 9700
F 0 "#PWR32" H 11650 9450 50  0001 C CNN
F 1 "GND" H 11650 9550 50  0000 C CNN
F 2 "" H 11650 9700 50  0000 C CNN
F 3 "" H 11650 9700 50  0000 C CNN
	1    11650 9700
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 58ABF23B
P 6650 7900
F 0 "C6" V 6500 7850 50  0000 L CNN
F 1 "100n" V 6800 7800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6688 7750 50  0001 C CNN
F 3 "" H 6650 7900 50  0000 C CNN
	1    6650 7900
	0    1    1    0   
$EndComp
Text Notes 9000 7100 0    39   ~ 0
Preloaded with Arduino Bootloader
Text Notes 5050 8650 0    60   ~ 0
FTDI for AVR
Text Label 10950 7450 0    60   ~ 0
SPI-FP-CONTROLLER-CS
Text Label 14450 6300 0    60   ~ 0
SPI
NoConn ~ 10300 7250
NoConn ~ 10300 7350
NoConn ~ 10300 8200
NoConn ~ 10300 8400
NoConn ~ 8400 7850
$Comp
L +3.3V #PWR17
U 1 1 58B32E47
P 7700 6600
F 0 "#PWR17" H 7700 6450 50  0001 C CNN
F 1 "+3.3V" H 7700 6740 50  0000 C CNN
F 2 "" H 7700 6600 50  0000 C CNN
F 3 "" H 7700 6600 50  0000 C CNN
	1    7700 6600
	1    0    0    -1  
$EndComp
Text Notes 8750 6550 0    39   ~ 0
Replace with ATTINY 84A-SSU (SOIC-14) to save space
Text Label 14550 8950 0    60   ~ 0
A5
Text Label 14550 8850 0    60   ~ 0
A4
$Comp
L TSR1-2433 M2
U 1 1 58C4983F
P 3800 10250
F 0 "M2" H 3800 10500 60  0000 C CNN
F 1 "TSR1-2433" H 3700 10250 60  0000 C CNN
F 2 "maker:TSR1-2433" H 3800 10250 60  0001 C CNN
F 3 "http://assets.tracopower.com/20170309112219/TSR1/documents/tsr1-datasheet.pdf" H 4350 9650 60  0001 C CNN
	1    3800 10250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR14
U 1 1 58C4D097
P 3800 10750
F 0 "#PWR14" H 3800 10500 50  0001 C CNN
F 1 "GND" H 3800 10600 50  0000 C CNN
F 2 "" H 3800 10750 50  0000 C CNN
F 3 "" H 3800 10750 50  0000 C CNN
	1    3800 10750
	1    0    0    -1  
$EndComp
$Comp
L TSR1-2433 M3
U 1 1 58C4F9DB
P 5950 10250
F 0 "M3" H 5950 10500 60  0000 C CNN
F 1 "TSR1-2433" H 5850 10250 60  0000 C CNN
F 2 "maker:TSR1-2433" H 5950 10250 60  0001 C CNN
F 3 "http://assets.tracopower.com/20170309112219/TSR1/documents/tsr1-datasheet.pdf" H 6500 9650 60  0001 C CNN
	1    5950 10250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR16
U 1 1 58C50313
P 5950 10800
F 0 "#PWR16" H 5950 10550 50  0001 C CNN
F 1 "GND" H 5950 10650 50  0000 C CNN
F 2 "" H 5950 10800 50  0000 C CNN
F 3 "" H 5950 10800 50  0000 C CNN
	1    5950 10800
	1    0    0    -1  
$EndComp
Text Notes 700  9800 0    60   ~ 0
Power
$Comp
L +3.3V #PWR6
U 1 1 58C4493D
P 3700 1950
F 0 "#PWR6" H 3700 1800 50  0001 C CNN
F 1 "+3.3V" H 3700 2090 50  0000 C CNN
F 2 "" H 3700 1950 50  0000 C CNN
F 3 "" H 3700 1950 50  0000 C CNN
	1    3700 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR4
U 1 1 58C45620
P 2050 10800
F 0 "#PWR4" H 2050 10550 50  0001 C CNN
F 1 "GND" H 2050 10650 50  0000 C CNN
F 2 "" H 2050 10800 50  0000 C CNN
F 3 "" H 2050 10800 50  0000 C CNN
	1    2050 10800
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG2
U 1 1 58C52474
P 2150 10750
F 0 "#FLG2" H 2150 10845 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 10930 50  0000 C CNN
F 2 "" H 2150 10750 50  0000 C CNN
F 3 "" H 2150 10750 50  0000 C CNN
	1    2150 10750
	0    1    1    0   
$EndComp
$Comp
L +9V #PWR2
U 1 1 58D5C3C0
P 1200 10000
F 0 "#PWR2" H 1200 9850 50  0001 C CNN
F 1 "+9V" H 1200 10140 50  0000 C CNN
F 2 "" H 1200 10000 50  0000 C CNN
F 3 "" H 1200 10000 50  0000 C CNN
	1    1200 10000
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG1
U 1 1 58D5C646
P 1400 10150
F 0 "#FLG1" H 1400 10245 50  0001 C CNN
F 1 "PWR_FLAG" H 1400 10330 50  0000 C CNN
F 2 "" H 1400 10150 50  0000 C CNN
F 3 "" H 1400 10150 50  0000 C CNN
	1    1400 10150
	-1   0    0    1   
$EndComp
$Comp
L +9V #PWR7
U 1 1 58D60607
P 3100 10000
F 0 "#PWR7" H 3100 9850 50  0001 C CNN
F 1 "+9V" H 3100 10140 50  0000 C CNN
F 2 "" H 3100 10000 50  0000 C CNN
F 3 "" H 3100 10000 50  0000 C CNN
	1    3100 10000
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR15
U 1 1 58D6090A
P 5250 10000
F 0 "#PWR15" H 5250 9850 50  0001 C CNN
F 1 "+9V" H 5250 10140 50  0000 C CNN
F 2 "" H 5250 10000 50  0000 C CNN
F 3 "" H 5250 10000 50  0000 C CNN
	1    5250 10000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P3
U 1 1 58D7596B
P 11850 3000
F 0 "P3" H 11850 3150 50  0000 C CNN
F 1 "CONN_01X02" V 11950 3000 50  0000 C CNN
F 2 "Connect:PINHEAD1-2" H 11850 3000 50  0001 C CNN
F 3 "" H 11850 3000 50  0000 C CNN
	1    11850 3000
	1    0    0    -1  
$EndComp
Text Notes 12050 2700 2    60   ~ 0
To Speaker
$Comp
L CONN_01X02 P1
U 1 1 58D7AB5C
P 950 10200
F 0 "P1" H 950 10350 50  0000 C CNN
F 1 "CONN_01X02" V 1050 10200 50  0000 C CNN
F 2 "Connect:PINHEAD1-2" H 950 10200 50  0001 C CNN
F 3 "" H 950 10200 50  0000 C CNN
	1    950  10200
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR1
U 1 1 58D7B17D
P 1150 10750
F 0 "#PWR1" H 1150 10500 50  0001 C CNN
F 1 "GND" H 1150 10600 50  0000 C CNN
F 2 "" H 1150 10750 50  0000 C CNN
F 3 "" H 1150 10750 50  0000 C CNN
	1    1150 10750
	1    0    0    -1  
$EndComp
Text Label 7200 10150 2    60   ~ 0
AVDD+3.3V
Text Label 5100 10150 2    60   ~ 0
DVDD+3.3V
$Comp
L CONN_02X03 P5
U 1 1 58DAF0A0
P 5800 7050
F 0 "P5" H 5800 7250 50  0000 C CNN
F 1 "ISP6PIN" H 5800 6850 50  0000 C CNN
F 2 "maker:CONNECT_PINHEAD_2_3" H 5800 5850 50  0001 C CNN
F 3 "" H 5800 5850 50  0000 C CNN
	1    5800 7050
	1    0    0    -1  
$EndComp
Text Label 5050 6950 0    60   ~ 0
SPI-MISO
Text Label 5050 7150 0    60   ~ 0
RST-AVR
Text Label 11050 8700 2    60   ~ 0
RST-AVR
Text Label 6450 7050 2    60   ~ 0
SPI-MOSI
$Comp
L +3.3V #PWR23
U 1 1 58DC898B
P 6450 6950
F 0 "#PWR23" H 6450 6800 50  0001 C CNN
F 1 "+3.3V" H 6450 7090 50  0000 C CNN
F 2 "" H 6450 6950 50  0000 C CNN
F 3 "" H 6450 6950 50  0000 C CNN
	1    6450 6950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR24
U 1 1 58DC8BFF
P 6450 7150
F 0 "#PWR24" H 6450 6900 50  0001 C CNN
F 1 "GND" H 6450 7000 50  0000 C CNN
F 2 "" H 6450 7150 50  0000 C CNN
F 3 "" H 6450 7150 50  0000 C CNN
	1    6450 7150
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X06 P6
U 1 1 58DD0FFF
P 5250 8150
F 0 "P6" H 5250 8500 50  0000 C CNN
F 1 "CONN_FTDI_AVR" V 5350 8150 50  0000 C CNN
F 2 "Connect:PINHEAD1-6" H 5250 8150 50  0001 C CNN
F 3 "" H 5250 8150 50  0000 C CNN
	1    5250 8150
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR30
U 1 1 58DDA044
P 6350 8400
F 0 "#PWR30" H 6350 8150 50  0001 C CNN
F 1 "GND" H 6350 8250 50  0000 C CNN
F 2 "" H 6350 8400 50  0000 C CNN
F 3 "" H 6350 8400 50  0000 C CNN
	1    6350 8400
	1    0    0    -1  
$EndComp
Text Label 5650 8300 0    60   ~ 0
CTS-AVR
$Comp
L +3.3V #PWR29
U 1 1 58DDA301
P 6350 8200
F 0 "#PWR29" H 6350 8050 50  0001 C CNN
F 1 "+3.3V" H 6350 8340 50  0000 C CNN
F 2 "" H 6350 8200 50  0000 C CNN
F 3 "" H 6350 8200 50  0000 C CNN
	1    6350 8200
	1    0    0    -1  
$EndComp
Text Label 5650 7900 0    60   ~ 0
DTR-AVR
Text Label 5650 8000 0    60   ~ 0
RXI-AVR
Text Label 5650 8100 0    60   ~ 0
TXO-AVR
Text Label 11050 8950 2    60   ~ 0
RXI-AVR
Text Label 11050 8850 2    60   ~ 0
TXO-AVR
Text Label 7150 7900 2    60   ~ 0
RST-AVR
NoConn ~ 6050 8300
Text Notes 6050 7450 2    60   ~ 0
ISP for AVR (Optional)
Text Notes 8750 9900 0    60   ~ 0
Front Panel Controller
Text Label 5050 7050 0    60   ~ 0
SPI-SCK
$Comp
L ROTARY-ENCODER S1
U 1 1 58EE7837
P 15300 7750
F 0 "S1" V 15650 7750 60  0000 C CNN
F 1 "ROTARY-ENCODER" V 14950 7750 60  0000 C CNN
F 2 "maker:ROTARY_ENCODER_EC12" H 15300 7750 60  0001 C CNN
F 3 "" H 15300 7750 60  0001 C CNN
	1    15300 7750
	0    1    1    0   
$EndComp
Text Label 10650 9050 0    60   ~ 0
PD2
Text Label 10600 9250 0    60   ~ 0
PD4
Text Label 14450 7550 0    60   ~ 0
PD2
Text Label 14450 7950 0    60   ~ 0
PD4
NoConn ~ 10300 9450
NoConn ~ 10300 9550
$Comp
L CONN_01X02 P2
U 1 1 58F755C2
P 15650 8900
F 0 "P2" H 15650 9050 50  0000 C CNN
F 1 "CONN_01X02" V 15750 8900 50  0000 C CNN
F 2 "Connect:PINHEAD1-2" H 15650 8900 50  0001 C CNN
F 3 "" H 15650 8900 50  0000 C CNN
	1    15650 8900
	1    0    0    -1  
$EndComp
NoConn ~ 10300 8100
$Comp
L CP C14
U 1 1 5905CE91
P 3650 1250
F 0 "C14" H 3675 1350 50  0000 L CNN
F 1 "22u" H 3500 1150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.00mm" H 3688 1100 50  0001 C CNN
F 3 "" H 3650 1250 50  0000 C CNN
	1    3650 1250
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 5905CF1A
P 3950 1250
F 0 "C15" H 3975 1350 50  0000 L CNN
F 1 "100n" H 3975 1150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3988 1100 50  0001 C CNN
F 3 "" H 3950 1250 50  0000 C CNN
	1    3950 1250
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR10
U 1 1 5905E727
P 3800 1050
F 0 "#PWR10" H 3800 900 50  0001 C CNN
F 1 "+3.3V" H 3800 1190 50  0000 C CNN
F 2 "" H 3800 1050 50  0000 C CNN
F 3 "" H 3800 1050 50  0000 C CNN
	1    3800 1050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR11
U 1 1 5905E7A5
P 3800 1450
F 0 "#PWR11" H 3800 1200 50  0001 C CNN
F 1 "GND" H 3800 1300 50  0000 C CNN
F 2 "" H 3800 1450 50  0000 C CNN
F 3 "" H 3800 1450 50  0000 C CNN
	1    3800 1450
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X04 P7
U 1 1 5906D80C
P 5750 9100
F 0 "P7" H 5750 9350 50  0000 C CNN
F 1 "CONN_CODEC_EXT" H 5750 8850 50  0000 C CNN
F 2 "maker:CONNECT_PINHEAD_2_4" H 5750 7900 50  0001 C CNN
F 3 "" H 5750 7900 50  0000 C CNN
	1    5750 9100
	1    0    0    -1  
$EndComp
Text Label 5050 8950 0    60   ~ 0
XDCS
Text Label 5050 9050 0    60   ~ 0
XRST
Text Label 5050 9150 0    60   ~ 0
SPI-SCK
Text Label 5050 9250 0    60   ~ 0
SPI-MISO
Text Label 6450 8950 2    60   ~ 0
SPI-XCS
Text Label 6450 9050 2    60   ~ 0
DREQ
Text Label 6450 9150 2    60   ~ 0
SPI-MOSI
$Comp
L GND #PWR42
U 1 1 5907192F
P 6250 9300
F 0 "#PWR42" H 6250 9050 50  0001 C CNN
F 1 "GND" H 6250 9150 50  0000 C CNN
F 2 "" H 6250 9300 50  0000 C CNN
F 3 "" H 6250 9300 50  0000 C CNN
	1    6250 9300
	1    0    0    -1  
$EndComp
Text Notes 5050 9500 0    60   ~ 0
Optional
Entry Wire Line
	6400 6200 6500 6300
Entry Wire Line
	6250 6200 6350 6300
Text Label 5850 3200 0    60   ~ 0
SPI-XCS
Text Label 10750 8300 0    60   ~ 0
FP-CHANGE-INTR
Text Label 5250 2800 0    60   ~ 0
SPI-FP-CONTROLLER-CS
$Comp
L Adafruit-Feather-M0-WiFi SOM1
U 1 1 5ACABA0D
P 2800 3350
F 0 "SOM1" H 2400 2150 60  0000 C CNN
F 1 "Adafruit-Feather-M0-WiFi" V 2600 3500 60  0000 C CNN
F 2 "" H 2450 4050 60  0001 C CNN
F 3 "" H 2450 4050 60  0001 C CNN
	1    2800 3350
	1    0    0    -1  
$EndComp
NoConn ~ 2100 2250
NoConn ~ 2100 2400
NoConn ~ 2100 2550
NoConn ~ 2100 4000
NoConn ~ 3500 2300
NoConn ~ 3500 2950
NoConn ~ 3500 3050
NoConn ~ 3500 3450
NoConn ~ 3500 4000
NoConn ~ 3500 4100
Text Label 5500 3750 0    60   ~ 0
SPI-MISO
Text Label 4500 3300 2    60   ~ 0
FP-CHANGE-INTR
Text Label 6450 2600 0    60   ~ 0
DREQ
Text Label 6450 2500 0    60   ~ 0
XDCS
$Comp
L GND #PWR5
U 1 1 5ACCA5E2
P 2800 4850
F 0 "#PWR5" H 2800 4600 50  0001 C CNN
F 1 "GND" H 2800 4700 50  0000 C CNN
F 2 "" H 2800 4850 50  0001 C CNN
F 3 "" H 2800 4850 50  0001 C CNN
	1    2800 4850
	1    0    0    -1  
$EndComp
NoConn ~ 2100 1800
Text Notes 1900 4800 0    60   ~ 0
Main Processor
NoConn ~ 3500 4250
NoConn ~ 3500 4350
NoConn ~ 3500 4450
Text Notes 1300 5400 0    60   ~ 0
1
Text Notes 2050 5400 0    60   ~ 0
16
Text Notes 2050 5000 0    60   ~ 0
17
Text Notes 1600 5000 0    60   ~ 0
28
Text Notes 1300 5500 0    60   ~ 0
Pinouts Feather M0
$Comp
L +5V #PWR3
U 1 1 5AD6452C
P 1850 1400
F 0 "#PWR3" H 1850 1250 50  0001 C CNN
F 1 "+5V" H 1850 1540 50  0000 C CNN
F 2 "" H 1850 1400 50  0001 C CNN
F 3 "" H 1850 1400 50  0001 C CNN
	1    1850 1400
	1    0    0    -1  
$EndComp
$Comp
L TSR1-2450 M1
U 1 1 5AD6B98E
P 2050 10250
F 0 "M1" H 2050 10500 60  0000 C CNN
F 1 "TSR1-2450" H 1950 10250 60  0000 C CNN
F 2 "" H 2050 10250 60  0001 C CNN
F 3 "http://assets.tracopower.com/20170309112219/TSR1/documents/tsr1-datasheet.pdf" H 2600 9650 60  0001 C CNN
	1    2050 10250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5AD70B07
P 2850 10000
F 0 "#PWR?" H 2850 9850 50  0001 C CNN
F 1 "+5V" H 2850 10140 50  0000 C CNN
F 2 "" H 2850 10000 50  0001 C CNN
F 3 "" H 2850 10000 50  0001 C CNN
	1    2850 10000
	1    0    0    -1  
$EndComp
$Comp
L 23K256 IC?
U 1 1 5C26568F
P 2700 8100
F 0 "IC?" H 2350 8550 50  0000 L CNN
F 1 "23K256" H 2600 8150 50  0000 L CNN
F 2 "DIL" H 2750 8000 50  0000 C CIN
F 3 "" H 2900 7600 50  0000 C CNN
	1    2700 8100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5C266456
P 2700 7250
F 0 "#PWR?" H 2700 7100 50  0001 C CNN
F 1 "+3.3V" H 2700 7390 50  0000 C CNN
F 2 "" H 2700 7250 50  0000 C CNN
F 3 "" H 2700 7250 50  0000 C CNN
	1    2700 7250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5C266B7F
P 1950 7450
F 0 "R?" V 2030 7450 50  0000 C CNN
F 1 "10K" V 1950 7450 50  0000 C CNN
F 2 "" V 1880 7450 50  0000 C CNN
F 3 "" H 1950 7450 50  0000 C CNN
	1    1950 7450
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5C266C04
P 2200 7450
F 0 "R?" V 2280 7450 50  0000 C CNN
F 1 "10K" V 2200 7450 50  0000 C CNN
F 2 "" V 2130 7450 50  0000 C CNN
F 3 "" H 2200 7450 50  0000 C CNN
	1    2200 7450
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5C268A47
P 2100 8950
F 0 "C?" H 2125 9050 50  0000 L CNN
F 1 "100n" H 2125 8850 50  0000 L CNN
F 2 "" H 2138 8800 50  0000 C CNN
F 3 "" H 2100 8950 50  0000 C CNN
	1    2100 8950
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 5C268EE7
P 1850 8950
F 0 "C?" H 1875 9050 50  0000 L CNN
F 1 "22u" H 1700 8850 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.00mm" H 1888 8800 50  0001 C CNN
F 3 "" H 1850 8950 50  0000 C CNN
	1    1850 8950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5C269677
P 2000 8800
F 0 "#PWR?" H 2000 8650 50  0001 C CNN
F 1 "+3.3V" H 2000 8940 50  0000 C CNN
F 2 "" H 2000 8800 50  0000 C CNN
F 3 "" H 2000 8800 50  0000 C CNN
	1    2000 8800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5C269BBE
P 2000 9100
F 0 "#PWR?" H 2000 8850 50  0001 C CNN
F 1 "GND" H 2000 8950 50  0000 C CNN
F 2 "" H 2000 9100 50  0000 C CNN
F 3 "" H 2000 9100 50  0000 C CNN
	1    2000 9100
	1    0    0    -1  
$EndComp
Entry Wire Line
	1200 6300 1300 6400
Entry Wire Line
	1050 6300 1150 6400
Entry Wire Line
	900  6300 1000 6400
Entry Wire Line
	750  6300 850  6400
$Comp
L GND #PWR?
U 1 1 5C26C445
P 2700 8750
F 0 "#PWR?" H 2700 8500 50  0001 C CNN
F 1 "GND" H 2700 8600 50  0000 C CNN
F 2 "" H 2700 8750 50  0000 C CNN
F 3 "" H 2700 8750 50  0000 C CNN
	1    2700 8750
	1    0    0    -1  
$EndComp
Text Label 1300 7500 1    60   ~ 0
SPI-MOSI
Text Label 1150 7500 1    60   ~ 0
SPI-MISO
Text Label 1000 7450 1    60   ~ 0
SPI-SCK
Text Label 850  7650 1    60   ~ 0
SPI-RAM-CS
NoConn ~ 3250 7800
Entry Wire Line
	12700 6300 12800 6400
Entry Wire Line
	12500 6300 12600 6400
Entry Wire Line
	12300 6300 12400 6400
Entry Wire Line
	12100 6300 12200 6400
NoConn ~ 10750 3350
NoConn ~ 10750 3550
$Comp
L +3.3V #PWR?
U 1 1 5C28A87A
P 10900 1900
F 0 "#PWR?" H 10900 1750 50  0001 C CNN
F 1 "+3.3V" H 10900 2040 50  0000 C CNN
F 2 "" H 10900 1900 50  0000 C CNN
F 3 "" H 10900 1900 50  0000 C CNN
	1    10900 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5C28A8EE
P 10900 2250
F 0 "#PWR?" H 10900 2000 50  0001 C CNN
F 1 "GND" H 10900 2100 50  0000 C CNN
F 2 "" H 10900 2250 50  0000 C CNN
F 3 "" H 10900 2250 50  0000 C CNN
	1    10900 2250
	1    0    0    -1  
$EndComp
$Comp
L Adafruit-Feather-MusicMaker/Amp SOM?
U 1 1 5C28D955
P 10050 3250
F 0 "SOM?" H 9650 1950 60  0000 C CNN
F 1 "Adafruit-Feather-MusicMaker/Amp" V 9850 3300 60  0000 C CNN
F 2 "" H 9700 3750 60  0001 C CNN
F 3 "" H 9700 3750 60  0001 C CNN
	1    10050 3250
	1    0    0    -1  
$EndComp
Entry Wire Line
	7900 6300 8000 6200
Text Label 8800 2750 0    60   ~ 0
SPI-XCS
Entry Wire Line
	8100 6300 8200 6200
Entry Wire Line
	8300 6300 8400 6200
Entry Wire Line
	8500 6300 8600 6200
Text Label 8800 3000 0    60   ~ 0
SPI-MISO
Text Label 8800 3150 0    60   ~ 0
SPI-MOSI
Text Label 8800 3300 0    60   ~ 0
SPI-SCK
Text Notes 1600 6900 0    60   ~ 0
BUffer
Wire Wire Line
	3500 3850 5800 3850
Wire Wire Line
	3500 3650 6100 3650
Wire Wire Line
	3500 2600 9350 2600
Wire Wire Line
	10300 7850 12450 7850
Wire Wire Line
	12450 7850 12450 8050
Wire Wire Line
	12450 8050 12450 8100
Wire Wire Line
	12450 8050 12300 8050
Wire Wire Line
	11800 8050 12000 8050
Wire Wire Line
	11800 7950 11800 8050
Wire Wire Line
	11800 8050 11800 8100
Connection ~ 11800 8050
Connection ~ 12450 8050
Wire Wire Line
	11800 8400 11800 8400
Wire Wire Line
	12450 8400 12450 8400
Wire Wire Line
	7700 6600 7700 6750
Wire Wire Line
	7700 6750 7700 7250
Wire Wire Line
	7700 6750 8150 6750
Wire Wire Line
	8150 6750 10500 6750
Wire Wire Line
	7700 9550 8400 9550
Wire Wire Line
	7700 7550 7700 9550
Wire Wire Line
	7700 9550 7700 9750
Connection ~ 7700 9550
Wire Wire Line
	8400 9450 8150 9450
Wire Wire Line
	8150 8500 8150 9450
Wire Wire Line
	8150 9450 8150 9750
Wire Wire Line
	8150 6750 8150 7550
Wire Wire Line
	8150 7550 8150 8200
Connection ~ 8150 9450
Wire Wire Line
	10500 7150 10500 8700
Wire Wire Line
	10300 8700 10500 8700
Wire Wire Line
	10500 8700 11050 8700
Wire Wire Line
	10300 7950 11800 7950
Wire Wire Line
	10300 7750 12800 7750
Wire Wire Line
	10300 7650 12600 7650
Wire Wire Line
	10300 7550 12400 7550
Wire Wire Line
	10300 7450 12200 7450
Wire Wire Line
	11350 8600 11350 8950
Wire Wire Line
	11350 8950 15150 8950
Wire Wire Line
	15150 8950 15450 8950
Wire Wire Line
	10300 8500 11450 8500
Wire Wire Line
	11450 8500 11450 8850
Wire Wire Line
	11450 8850 15450 8850
Wire Wire Line
	15150 8950 15150 9000
Connection ~ 15150 8950
Wire Wire Line
	10300 8600 11350 8600
Wire Wire Line
	10300 9350 11050 9350
Wire Wire Line
	11050 9350 11050 9450
Wire Wire Line
	10300 9150 11050 9150
Wire Wire Line
	11650 9150 11650 9450
Wire Wire Line
	11650 9450 11650 9750
Connection ~ 11650 9450
Connection ~ 10500 8700
Wire Wire Line
	10300 8850 11050 8850
Wire Wire Line
	10300 8950 11050 8950
Wire Notes Line
	5000 7750 5000 8700
Wire Notes Line
	5000 8700 6200 8700
Wire Notes Line
	6200 8700 6200 7750
Wire Notes Line
	6200 7750 5000 7750
Wire Bus Line
	700  6300 750  6300
Wire Bus Line
	750  6300 900  6300
Wire Bus Line
	900  6300 1050 6300
Wire Bus Line
	1050 6300 1200 6300
Wire Bus Line
	1200 6300 5900 6300
Wire Bus Line
	5900 6300 6050 6300
Wire Bus Line
	6050 6300 6200 6300
Wire Bus Line
	6200 6300 6350 6300
Wire Bus Line
	6350 6300 6500 6300
Wire Bus Line
	6500 6300 7400 6300
Wire Bus Line
	5900 6300 6050 6300
Wire Bus Line
	6050 6300 6200 6300
Wire Bus Line
	6200 6300 6350 6300
Wire Bus Line
	6350 6300 6500 6300
Wire Bus Line
	6500 6300 7900 6300
Wire Bus Line
	7900 6300 8100 6300
Wire Bus Line
	8100 6300 8300 6300
Wire Bus Line
	8300 6300 8500 6300
Wire Bus Line
	8500 6300 12100 6300
Wire Bus Line
	12100 6300 12300 6300
Wire Bus Line
	12300 6300 12500 6300
Wire Bus Line
	12500 6300 12700 6300
Wire Bus Line
	12700 6300 14650 6300
Wire Bus Line
	12100 6300 12300 6300
Wire Bus Line
	12300 6300 12500 6300
Wire Bus Line
	12500 6300 12700 6300
Wire Bus Line
	6050 6300 6200 6300
Wire Bus Line
	6200 6300 6350 6300
Wire Wire Line
	5800 3850 5800 6200
Wire Wire Line
	6100 3650 6100 6200
Wire Bus Line
	12300 6300 12500 6300
Wire Wire Line
	12200 7450 12200 6400
Wire Wire Line
	12400 7550 12400 6400
Wire Wire Line
	12600 7650 12600 6400
Wire Wire Line
	12800 7750 12800 6400
Wire Wire Line
	10500 6750 10500 6850
Wire Wire Line
	7700 7250 8400 7250
Connection ~ 7700 7250
Connection ~ 8150 7550
Wire Wire Line
	8150 7550 8400 7550
Connection ~ 7700 6750
Connection ~ 8150 6750
Wire Wire Line
	3800 10700 3800 10750
Wire Wire Line
	5950 10700 5950 10800
Wire Wire Line
	2850 10000 2850 10150
Wire Wire Line
	2850 10150 2650 10150
Wire Wire Line
	1150 10150 1400 10150
Wire Wire Line
	1400 10150 1450 10150
Wire Wire Line
	2050 10700 2050 10750
Wire Wire Line
	2050 10750 2050 10800
Wire Wire Line
	3100 10150 3200 10150
Wire Wire Line
	5250 10150 5350 10150
Wire Notes Line
	600  9650 7250 9650
Wire Notes Line
	7250 9650 7250 11100
Wire Notes Line
	7250 11100 150  11100
Wire Notes Line
	600  11100 600  9650
Wire Wire Line
	2150 10750 2050 10750
Connection ~ 2050 10750
Wire Wire Line
	6550 10150 7200 10150
Wire Wire Line
	4400 10150 5100 10150
Wire Wire Line
	1200 10000 1200 10150
Wire Wire Line
	3100 10000 3100 10150
Wire Wire Line
	5250 10000 5250 10150
Wire Wire Line
	11650 3050 11650 3150
Wire Wire Line
	1200 10150 1400 10150
Connection ~ 1400 10150
Wire Wire Line
	1150 10250 1150 10750
Wire Notes Line
	11500 2550 12150 2550
Wire Notes Line
	12150 2550 12150 3450
Wire Notes Line
	12150 3450 11500 3450
Wire Notes Line
	11500 3450 11500 2550
Wire Wire Line
	5550 6950 5050 6950
Wire Wire Line
	6050 7050 6450 7050
Wire Wire Line
	6050 7150 6450 7150
Wire Wire Line
	5550 7150 5050 7150
Wire Wire Line
	6050 6950 6450 6950
Wire Wire Line
	5450 7900 6500 7900
Wire Wire Line
	5450 8000 6100 8000
Wire Wire Line
	5450 8100 6100 8100
Wire Wire Line
	5450 8200 6350 8200
Wire Wire Line
	5450 8300 6050 8300
Wire Wire Line
	5450 8400 6350 8400
Wire Wire Line
	6800 7900 7200 7900
Wire Notes Line
	6600 6700 5000 6700
Wire Notes Line
	5000 6700 5000 7500
Wire Notes Line
	5000 7500 6600 7500
Wire Notes Line
	6600 7500 6600 6700
Wire Wire Line
	15150 9300 15150 9450
Wire Wire Line
	5550 7050 5050 7050
Wire Wire Line
	10300 9050 11050 9050
Wire Wire Line
	10300 9250 11000 9250
Wire Wire Line
	14850 7550 14450 7550
Wire Wire Line
	14850 7950 14450 7950
Wire Wire Line
	14850 7750 14200 7750
Wire Wire Line
	14200 7750 14200 8100
Wire Wire Line
	3650 1100 3800 1100
Wire Wire Line
	3800 1100 3950 1100
Wire Wire Line
	3650 1400 3800 1400
Wire Wire Line
	3800 1400 3950 1400
Wire Wire Line
	3800 1050 3800 1100
Connection ~ 3800 1100
Wire Wire Line
	3800 1450 3800 1400
Connection ~ 3800 1400
Wire Wire Line
	5500 8950 5050 8950
Wire Wire Line
	5500 9050 5050 9050
Wire Wire Line
	5500 9150 5050 9150
Wire Wire Line
	5500 9250 5050 9250
Wire Wire Line
	6000 9250 6250 9250
Wire Wire Line
	6000 9150 6450 9150
Wire Wire Line
	6000 9050 6450 9050
Wire Wire Line
	6000 8950 6450 8950
Wire Wire Line
	6250 9250 6250 9300
Wire Notes Line
	5000 8750 6500 8750
Wire Notes Line
	6500 8750 6500 9500
Wire Notes Line
	6500 9500 5000 9500
Wire Notes Line
	5000 9500 5000 8750
Wire Wire Line
	6250 3200 6250 6200
Wire Wire Line
	6400 2800 6400 6200
Wire Wire Line
	10300 8300 11450 8300
Wire Wire Line
	3500 3300 4500 3300
Wire Wire Line
	3500 3200 6250 3200
Wire Wire Line
	5950 3750 5950 6200
Wire Wire Line
	5950 3750 3500 3750
Wire Wire Line
	3500 2800 6400 2800
Wire Wire Line
	2800 4750 2800 4850
Wire Wire Line
	3500 2500 8000 2500
Wire Notes Line
	1300 5000 2150 5000
Wire Notes Line
	2150 5000 2150 5300
Wire Notes Line
	2150 5300 1300 5300
Wire Notes Line
	1300 5300 1300 5000
Wire Notes Line
	1300 5100 1400 5100
Wire Notes Line
	1400 5100 1400 5200
Wire Notes Line
	1400 5200 1300 5200
Wire Wire Line
	3500 2050 3700 2050
Wire Wire Line
	3700 2050 3700 1950
Wire Wire Line
	2100 1800 1850 1800
Wire Wire Line
	1850 1800 1850 1400
Wire Wire Line
	1850 8800 2000 8800
Wire Wire Line
	2000 8800 2100 8800
Connection ~ 2000 8800
Wire Wire Line
	1850 9100 2000 9100
Wire Wire Line
	2000 9100 2100 9100
Connection ~ 2000 9100
Wire Wire Line
	2700 7250 2700 7300
Wire Wire Line
	2700 7300 2700 7550
Wire Wire Line
	1950 7300 2200 7300
Wire Wire Line
	2200 7300 2700 7300
Connection ~ 2700 7300
Wire Wire Line
	2200 7600 2200 7800
Connection ~ 2200 7300
Wire Wire Line
	1950 7600 1950 8300
Wire Wire Line
	850  8300 1950 8300
Wire Wire Line
	1950 8300 2200 8300
Wire Wire Line
	1300 6400 1300 8000
Wire Wire Line
	1300 8000 2200 8000
Wire Wire Line
	2200 8100 1150 8100
Wire Wire Line
	1150 8100 1150 6400
Wire Wire Line
	2200 8200 1000 8200
Wire Wire Line
	1000 8200 1000 6400
Wire Wire Line
	850  8300 850  6400
Connection ~ 1950 8300
Wire Wire Line
	2700 8600 2700 8750
Wire Wire Line
	11650 2950 10750 2950
Wire Wire Line
	11650 3150 10750 3150
Wire Wire Line
	10750 2000 10900 2000
Wire Wire Line
	10900 2000 10900 1900
Wire Wire Line
	10750 2200 10900 2200
Wire Wire Line
	10900 2200 10900 2250
Wire Wire Line
	8000 2500 8000 2450
Wire Wire Line
	8000 2450 9350 2450
Wire Wire Line
	8000 6200 8000 2750
Wire Wire Line
	8000 2750 9350 2750
Wire Wire Line
	8200 6200 8200 3000
Wire Wire Line
	8200 3000 9350 3000
Wire Wire Line
	8400 6200 8400 3150
Wire Wire Line
	8400 3150 9350 3150
Wire Wire Line
	8600 6200 8600 3300
Wire Wire Line
	8600 3300 9350 3300
Wire Notes Line
	1550 6750 1550 9450
Wire Notes Line
	1550 9450 3550 9450
Wire Notes Line
	3550 9450 3550 6750
Wire Notes Line
	3550 6750 1550 6750
Wire Notes Line
	1950 5200 1950 5250
Wire Notes Line
	2050 5200 1950 5200
Wire Notes Line
	2050 5150 2050 5200
Wire Notes Line
	1950 5150 2050 5150
Wire Notes Line
	1950 5100 1950 5150
Wire Notes Line
	2050 5100 1950 5100
Wire Notes Line
	2050 5050 2050 5100
Wire Notes Line
	1950 5050 2050 5050
Text Notes 9600 5350 0    60   ~ 0
1
Text Notes 10350 5350 0    60   ~ 0
16
Text Notes 10350 4950 0    60   ~ 0
17
Text Notes 9900 4950 0    60   ~ 0
28
Text Notes 9600 5450 0    60   ~ 0
Pinouts Feather MusicMaker/Amp
Wire Notes Line
	9600 4950 10450 4950
Wire Notes Line
	10450 4950 10450 5250
Wire Notes Line
	10450 5250 9600 5250
Wire Notes Line
	9600 5250 9600 4950
Wire Notes Line
	10200 5100 10250 5100
Wire Notes Line
	10200 5150 10250 5150
Wire Notes Line
	10200 5200 10400 5200
Wire Notes Line
	10400 5200 10400 5050
Wire Notes Line
	10400 5050 10150 5050
Wire Notes Line
	10150 5050 10150 5150
Wire Wire Line
	9400 2200 8950 2200
Wire Wire Line
	8950 2200 8950 1750
$Comp
L +3.3V #PWR?
U 1 1 5C29FF93
P 8950 1750
F 0 "#PWR?" H 8950 1600 50  0001 C CNN
F 1 "+3.3V" H 8950 1890 50  0000 C CNN
F 2 "" H 8950 1750 50  0000 C CNN
F 3 "" H 8950 1750 50  0000 C CNN
	1    8950 1750
	1    0    0    -1  
$EndComp
Text Label 8950 2200 1    60   ~ 0
AVDD+3.3V
Text Notes 6900 1650 0    60   ~ 0
TO DO:\n - Power supplies
Text Notes 3950 4450 0    60   ~ 0
SPI used internally \nfor WIFI
Wire Notes Line
	3650 4200 3750 4200
Wire Notes Line
	3750 4200 3750 4500
Wire Notes Line
	3750 4500 3650 4500
Wire Notes Line
	3950 4200 4850 4200
Wire Notes Line
	4850 4200 4850 4500
Wire Notes Line
	4850 4500 3950 4500
Wire Notes Line
	3950 4500 3950 4200
Wire Notes Line
	3750 4350 3950 4350
$EndSCHEMATC
