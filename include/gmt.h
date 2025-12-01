#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#define SIZE 24 // Grid size
struct compass_and_level {
	double angle;
	double pitch;
	double roll;
};
struct compass_and_level compute_compass_and_level_data(double x, double y, double z, double w);
void show_compass_matrix(double angle_deg);
void show_level_matrix(double pitch_deg, double roll_deg);
void watchdog(void);