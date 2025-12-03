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
	char **grid = init_matrix_grid();
	init_matrix(grid, 1, 'W');
	// Center coordinates
	int cx = SIZE / 2;
	int cy = SIZE / 2;
	// Define four direction vectors
	struct Dir {
		int dx, dy;
		char ch;
	} dirs[4] = { { 0, -2, 'R' }, { 0, 2, 'W' }, { -2, 0, 'W' }, { 2, 0, 'W' } }; // NSWE if needed
	int north_marked = 0;
	static int first_bell = 1;
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
		if (gx > SIZE) {
			gx = SIZE - 1;
		}
		if (gy < 0) {
			gy = 0;
		}
		if (gy > SIZE) {
			gy = SIZE - 1;
		}

		grid[gy][gx] = dirs[k].ch;
		if (gx == cx || gy == cy) {
			north_marked = 1;
		} else {
			north_marked = 0;
			first_bell = 1;
		}
	}
	if (north_marked && first_bell) {
		printf("\a");
		first_bell = 0;
	}

	print_matrix(grid);
	free(buf);
	free_matrix_grid(grid);
}
