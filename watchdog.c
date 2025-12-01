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
		system("logcat|grep --line-buffered -m1 -i visibilityChanged");
		exit(0);
	}
}