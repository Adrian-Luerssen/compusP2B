#define T1sec 200
#define MAXCHANNELS 
static char update_tics;

void initStore(void){
	
}

void sensorMotor(void) {
	static char state = 0;

	switch(state) {
		case 0:
			if (SeNewTicsSensor()) {
				update_tics = SeGetTicsSensor();
				count = 0;
				state = 1;
			}
		break;
		case 1:
			if (count < MAXCHANNELS) {
				if (info[count].active == 1){
					info[count].run_distance += update_tics;
				}
				count++;
			}else {
				state = 0;
			}
			
		break;
	}
}

void timerMotor(void) {
	static char state = 0;

	switch(state) {
		case 0:
			if (TiGetTics(timer) >= T1sec) {
				TiResetTics(timer);
				state = 1;
			}
		break;
		case 1:
			if (count < MAXCHANNELS) {
				if (info[count].active == 1){
					info[count].elapsed_time += 1;
				}
				count++;
			} else {
				state = 0;
			}
			
		break;
	}
}