#include "include/gmt.h"
void show_compass_matrix(double angle_deg)
{
	char *buf = malloc(65536);
	setvbuf(stdout, buf, _IOFBF, 65536);
	printf("\033[?25l");

	// Read rotation angle
	// angle_deg = 360.0 - angle_deg; // Compass heading to mathematical angle
	double angle_rad = angle_deg * M_PI / 180.0;

	// Create empty grid
	char grid[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			grid[i][j] = ' ';
		}
	}
	// Initialize grid
	int cx = SIZE / 2;
	int cy = SIZE / 2;
	double radius = SIZE / 2.0 - 1; // radius
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			// Coordinates relative to center
			double dx = j - cx;
			double dy = i - cy;

			// Inside circle check
			if (dx * dx + dy * dy < radius * radius) {
				grid[i][j] = '#'; // fill inside circle
			} else {
				grid[i][j] = ' '; // outside circle
			}
			if (dx * dx + dy * dy == 0) {
				grid[i][j] = 'X'; // center marker
			}
		}
	}

	// Define four direction vectors
	struct Dir {
		int dx, dy;
		char ch;
	} dirs[4] = { { 0, -2, 'N' }, { 0, 2, '#' }, { -2, 0, '#' }, { 2, 0, '#' } }; // NSWE if needed
	int north_marked = 0;
	// Rotate vectors and map to grid
	for (int k = 0; k < 4; k++) {
		double x = dirs[k].dx;
		double y = dirs[k].dy;

		// Rotation
		double xr = x * cos(angle_rad) - y * sin(angle_rad);
		double yr = x * sin(angle_rad) + y * cos(angle_rad);

		// Scale to grid length (use length 5 to avoid out-of-bounds)
		int gx = cx + (int)round(xr * 5);
		int gy = cy + (int)round(yr * 5);

		// Bounds clamp
		if (gx < 0) {
			gx = 0;
		}
		if (gx >= SIZE) {
			gx = SIZE - 1;
		}
		if (gy < 0) {
			gy = 0;
		}
		if (gy >= SIZE) {
			gy = SIZE - 1;
		}

		grid[gy][gx] = dirs[k].ch;
		if (gx == cx || gy == cy) {
			north_marked = 1;
		}
	}
	if (north_marked) {
		printf("\a");
	}

	// Output grid
	printf("\033[0H");
	fflush(stdout);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int x_offset = (w.ws_col - SIZE * 2) / 2 - 1;
	int y_offset = (w.ws_row - SIZE) / 2;
	for (int i = 0; i < y_offset; i++) {
		printf("\n");
	}
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < x_offset; j++) {
			printf(" ");
		}
		for (int j = 0; j < SIZE; j++) {
			if (grid[i][j] == '#') {
				printf("\033[37m█▊");
			} else if (grid[i][j] == 'N') {
				printf("\033[31m█▊");
			} else if (grid[i][j] == 'X') {
				printf("\033[32m█▊");
			} else {
				printf("  ");
			}
		}
		printf("\n");
	}
	fflush(stdout);
}
