#include "include/gmt.h"
void watchdog(void)
{
	pid_t pid1 = fork();
	if (pid1 > 0) {
		return;
	}
	pid_t pid2 = fork();
	if (pid2 > 0) {
		wait(NULL);
		system("killall -9 gmt");
		exit(0);
	} else {
		system("logcat -c");
		system("logcat|grep --line-buffered -m1 -i visibilityChanged 2>&1 >/dev/null");
		printf("\033[?25h");
		exit(0);
	}
}
bool shake_detection(double x, double y, double z)
{
	static int shake_count = 0;
	const double SHAKE_THRESHOLD = 7.0; // Threshold for shake detection
	static time_t last_shake_time = 0;
	time_t current_time = time(NULL);
	if (current_time - last_shake_time > 2) {
		shake_count = 0; // Reset count if more than 2 seconds have passed
		last_shake_time = current_time;
	}
	double magnitude = sqrt(x * x + y * y + z * z);
	if (magnitude > SHAKE_THRESHOLD) {
		shake_count++;
		if (shake_count >= 2) {
			shake_count = 0;
			printf("\a");
			fflush(stdout);
			last_shake_time = current_time;
			return true;
		}
	}
	return false;
}