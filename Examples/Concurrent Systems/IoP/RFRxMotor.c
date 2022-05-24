static char state[MAX_CHANNELS];
static char timer[MAX_CHANNELS];
static char num_bits[MAX_CHANNELS];
static char num_chars[MAX_CHANNELS];
static int aux_checksum[MAX_CHANNELS];
static int checksum_received[MAX_CHANNELS];
void initMotor(void){
	for (char i = 0; i < MAX_CHANNELS; i++){
		state[i] = 0;
		timer[i] = TiGetTimer();
	}
}

void RFRxMotor(char channel) {
	static char state[channel] = 0;

	switch(state) {
		case 0:
			if (getInput(channel) == STARTBIT) {
				TiResetTics(timer[channel]);
				state[channel] = 1;
			}
		break;
		case 1:
			if (TiGetTics(timer[channel])>=TBIT) {
				TiResetTics(timer[channel]);
				num_bits[channel] = 0;
				num_chars[channel] = 0;
				aux_char[channel] = 0;
				aux_checksum[channel] = 0;
				checksum_recieved[channel] =0;
				state[channel] = 2;
			}
		break;
		case 2:
			if (TiGetTics(timer[channel])>=TBIT) {
				TiResetTics(timer[channel]);
				aux_char[channel] = (aux_char[channel] << 1) & 0xFE | getInput(channel);
				num_bits[channel]++;
				state[channel] = 3;
			}
		break;
		case 3:
			if (num_bits[channel] != 8) {
				state[channel] = 2;
			}
			else {
				num_chars[channel]++;
				aux_checksum[channel] += aux_char[channel];
				state[channel] = 4;
			}
		break;
		case 4:
			if (num_chars[channel]  < 64 | num_chars[channel] == 68) {
				state[channel] = 2;
			}
			else if (num_chars[channel] >= 64 && num_chars[channel] < 68) {
				CoSendHumidity(aux_char[channel], channel);
				state[channel] = 5;
			}
			else if (num_chars[channel] >= 69 && num_chars[channel] < 73) {
				CoSendTemperature(aux_char[channel], channel);
				state[channel] = 6;
			}
			else if (num_chars[channel] > 73) {
				checksum_recieved[channel] = (checksum_received[channel] << 8) & 0xFF00 | aux_char[channel];
				state[channel] = 7;
			}
			aux_char[channel] = 0;
			num_bits[channel] = 0;
		break;
		case 5:
			state[channel] = 2;
		break;
		case 6:
			state[channel] = 2;
		break;
		case 7:
			if (num_chars[channel] < 75) {
				state[channel] = 2;
			}
			else  {
				CoDataIsCorrect(checksum_received[channel] == aux_checksum[channel],channel);
				state[channel] = 0;
			}
		break;
	}
}