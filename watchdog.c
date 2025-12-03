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
	const double SHAKE_THRESHOLD = 20.0; //
	double magnitude = sqrt(x * x + y * y + z * z);
	if (magnitude > SHAKE_THRESHOLD) {
		shake_count++;
		if (shake_count >= 2) {
			shake_count = 0;
			return true;
		}
	}
	return false;
}