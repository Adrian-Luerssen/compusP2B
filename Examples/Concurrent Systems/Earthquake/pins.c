//pins
PIC C:
OSC1 and OSC2: RA6 and RA7

Sens+: RA0
Threshhold: RA1
Work: RB0
SMSTx and SMSRx: RC6 and RC7
LedLevel: RA3


PIC ASM:
OSC1 and OSC2: RA6 and RA7
Store: RB0
DRAM: RD[7..0]
@RAM[12..0]: RA[5..0] + RB[7..0]
CTRLRAM[2..0]: RE[2..0]
ToStore: RC
	- Data RC[3..0]
	- NewSample RC4
	- Ack RC5
	- Shaking RC6

INTERRUPTS:
PIC C: Timer0 (0.5ms)
	1MHz => 4uS --> 0.5ms/4uS = 125 instructions (Not Enough)
	4MHz (1MHz + PLL) still too few instructions
	16MHz (internal oscilator) => 250ns --> 0.5ms/250ns = 2000