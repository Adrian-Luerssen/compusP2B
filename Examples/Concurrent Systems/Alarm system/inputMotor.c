#define STARTBIT 1
#define TBIT 1
#define MAXCHANNELS 8

static char timer[MAXCHANNELS];
static char parity_aux[MAXCHANNELS];
static char value[MAXCHANNELS];
static char count[MAXCHANNELS];
static char state[MAXCHANNELS];

static const char table[MAXCHANNELS] = {1,2,4,8,16,32,64,128}

void initInputs(void){
	TRISB = 0xFF;
	for (char i = 0; i< MAXCHANNELS; i++){
		timer[i] = TiGetTimer();
		value[i] = 0;
		count[i] = 0;
		parity_aux[i] = 0;
		state[i] = 0
	}
	
}


char getInput(char my_channel){
	//return PORTBbits.my_channel; not possible
	//return PORTB[channel]; not possible
	/*switch(my_channel){
		case 0:
			return PORTBbits.RB0;
		case 1:
			return PORTBbits.RB1;
		case 2:
			return PORTBbits.RB2;
		case 3:
			return PORTBbits.RB3;
		case 4:
			return PORTBbits.RB4;
		case 5:
			return PORTBbits.RB5;
		case 6:
			return PORTBbits.RB6;
		case 7:
			return PORTBbits.RB7;
		
	}
	First approach
	Problem: wastes FLASH, not scalable
	*/
	/*
	return PORTB >> my_channel & 0x01;
	Second approach
	Same problem as using a loop as some channels take longer 
	*/	

	return (PORTB & table[my_channel]) != 0;

}
// can be called in main as
/*
inputMotor(0); //channel 0
inputMotor(1); //channel 1
inputMotor(2); //channel 2
.
.
.
inputMotor(7); //channel 3
*/
void inputMotor(char channel) {
	

	switch(state) {
		case 0:
			if (getInput(channel) == STARTBIT) {
				TiResetTics(timer[channel]);
				value[channel] = 0;
				count[channel] = 0;
				parity_aux[channel] = 0;
				state[channel] = 1;
			}
		break;
		case 1:
			if (TiGetTics(timer[channel]) >= TBIT){
				TiResetTics(timer[channel]);
				parity_aux[channel] += getInput(channel);
				if (count[channel] < 8) {
					value[channel] = ((value[channel] << 1) & 0xFE)| getInput(channel);
					count[channel]++;
				}
				else {
					state[channel] = 2;
				}
			}
			
		break;
		case 2:
			if (TiGetTics(timer[channel]) >= TBIT){
				if (parity_aux[channel] % 2 == 1) {
					state[channel] = 0;
				}
				else  {
					state[channel] = 3;
				}
			}
			
		break;
		case 3:
			state[channel] = 0;
			if (value[channel] > 15 || value[channel] < -15) {
				QuPush(value[channel],channel);
			}
		break;
	}
}