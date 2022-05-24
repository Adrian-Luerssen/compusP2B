//PINS

//INPUTS
// RB0 - sensor
// RB[3..1] - Columns
//OUTPUTS
// RD[6..0] - V (7 Segments)
// RB[5..4] - Rows
// RC0 - DirForward
// RC1 - DirBackward
// RA[3..0] - CN[3..0]

//Interrupts
// sensor wil generate rising edge
// interrupt to count distance
// timer0 will interupt to count time

// in store:
char which_channel;
// OPTION 1:
typedef struct {
	char forward;
	char active;
	char time_distance;
	unsigned int run_distance;
	unsigned int elapsed_time;
} data_struct;
//uses: 16 channels * 7bytes;
// OPTION 2:
typedef struct {
	char Forward;
	char active;
	char time_distance;
	char run_distance[4];
	char elapsed_time[4];
} data_struct;
// uses: 16channels * 11 bytes;