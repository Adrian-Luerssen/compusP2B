//PIC 1
// RFTX - RC1
// H - RA0
// T - RA1
// PWM - RC0
// eusart(n) --> RC6 and RC7

//PIC 2
// RFRX - RA[5..0] + RE[2..0]
// HB - RC5
// SPK1 - RB4
// OSC1 and OSC2 - RA6 and RA7
// 7SegOut1 - RD[6..0]
// 7SegOut2 - RB[6..0]
// C0 C1 - RC[1..0]
// C2 C2 - RC[3..2]
// eusart(n) --> RC6 and RC7
// we use the eusart to send values of humidity and temperature
// and also to avoid doing th ITOA in assembly
// interrupt 1ms



// pwm out 34.5us at 1 and 34.5 us at 0
//Fosc = 8Mhz Tinst=0.5us
//34.5/0.5 = 69 instructions(Not enough for an interrupt)
// since it is not enough for an interrupt we will have to count instructions
// since we have to count instructions and we dont know how to do it in .c we have to do it in assembly

// PIC 1 in asm
// PIC 2 .c

// MOVF TABLAT,0
// ADDWF CHECKSUM_LOW,1,0
// BTFSS STATUS ,C,0
// INCF CHECKSSUM_HIGH,1,0