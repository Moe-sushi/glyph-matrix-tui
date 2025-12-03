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
	init_matrix(grid, 1, '3');
	// Center coordinates
	int cx = SIZE / 2;
	int cy = SIZE / 2;
	// Define four direction vectors
	struct Dir {
		int dx, dy;
		char ch;
	} dirs[4] = { { 0, -2, 'R' }, { 0, 2, 'B' }, { -2, 0, '3' }, { 2, 0, '3' } }; // NSWE if needed
	int north_marked = 0;
	static int first_bell = 1;
	// Rotate vectors and map to grid
	for (int k = 0; k < 4; k++) {
		double x = dirs[k].dx;
		double y = dirs[k].dy;

		// Rotation
		double xr = x * cos(angle_rad) - y * sin(angle_rad);
		double yr = x * sin(angle_rad) + y * cos(angle_rad);

		// Scale to grid length (use length 6)
		int gx = cx + (int)round(xr * 6);
		int gy = cy + (int)round(yr * 6);

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
		if (grid[gy][gx] != '3') {
			while (grid[gy][gx] != '3') {
				if (gx > SIZE / 2) {
					gx--;
				} else if (gx < SIZE / 2) {
					gx++;
				}
				if (gy > SIZE / 2) {
					gy--;
				} else if (gy < SIZE / 2) {
					gy++;
				}
			}
		}
		grid[gy][gx] = dirs[k].ch;
		// Connect to center using Bresenham's line algorithm for smoother, continuous lines
		int x0 = cx, y0 = cy;
		int x1 = gx, y1 = gy;
		int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
		int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
		int err = dx + dy;

		for (;;) {
			if (grid[y0][x0] != '0') {
				grid[y0][x0] = dirs[k].ch;
			}
			if (x0 == x1 && y0 == y1) {
				break;
			}
			int e2 = 2 * err;
			if (e2 >= dy) {
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx) {
				err += dx;
				y0 += sy;
			}
		}
		// Check for north marker
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
