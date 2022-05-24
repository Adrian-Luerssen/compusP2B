static char[] hour;
static char[] min;
static char seconds;

void hourMotor (void){
	static char state = 0;

	switch (state){
		case 0:
			
			if (TiGetTics(timer) >= ONESEC){
				seconds++;
				TiResetTics(timer)
				if (seconds == 60){
					min[0]++;
					if (min[0] == 10){
						min[1]++;
						min[0] = 0;
						if (min[1] == 6){
							hour[0]++;
							min[1] = 0;
							if (hour[0] == 10){
								hour[0] = 0;
								hour[1]++;
							}
						} 
					} 	
				
				}
				
			}
		break;

	}
}

void HoGetHour(char[] hours, char[] mins){
	hours[0] = hour[0];
	hours[1] = hour[1];
	mins[0] = min[0];
	mins[1] = min[1];
}

void HoGetHour(char[] string){
	string[32] = hour[0];
	string[33] = hour[1];
	string[35] = min[0];
	string[34] = min[1];
}