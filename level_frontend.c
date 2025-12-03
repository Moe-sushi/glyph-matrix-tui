#include "include/gmt.h"
void show_level_matrix(double pitch_deg, double roll_deg)
{
	char *buf = malloc(65536);
	setvbuf(stdout, buf, _IOFBF, 65536);
	printf("\033[?25l");
	// Compute pitch and roll (degrees)
	double pitch = pitch_deg * M_PI / 180.0;
	double roll = roll_deg * M_PI / 180.0;
	roll = -roll; // Invert roll for display
	double radius = SIZE / 2.0 - 1; // circle radius
	// Grid and center
	char **grid = init_matrix_grid();
	init_matrix(grid, 1, 'R');
	int cx = SIZE / 2;
	int cy = SIZE / 2;
	// Draw rotated projection
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			double dx = j - cx; // local X relative to center
			double dy = i - cy; // local Y relative to center
			double dz = 0; // circle lies in Z=0 plane

			if (dx * dx + dy * dy >= radius * radius) {
				continue; // skip outside radius
			}
			// Rotate about X axis
			double dy1 = dy * cos(pitch) - dz * sin(pitch);
			double dz1 = dy * sin(pitch) + dz * cos(pitch);

			// Rotate about Y axis
			double dx2 = dx * cos(roll) + dz1 * sin(roll);
			double dz2 = -dx * sin(roll) + dz1 * cos(roll);

			// Orthographic projection onto XY plane
			int gx = (int)round(cx + dx2);
			int gy = (int)round(cy + dy1);

			// Bounds check and mark projection
			if (gx > 0 && gx < SIZE - 1 && gy > 0 && gy < SIZE - 1) {
				if (grid[gy][gx] == 'R') {
					grid[gy][gx] = 'W';
				}
			}
		}
	}

	// Project elevated center marker
	const double center_height = 9.0;
	double center_dx = 0.0;
	double center_dy = 0.0;
	double center_dz = center_height;

	double center_dy1 = center_dy * cos(pitch) - center_dz * sin(pitch);
	double center_dz1 = center_dy * sin(pitch) + center_dz * cos(pitch);
	double center_dx2 = center_dx * cos(roll) + center_dz1 * sin(roll);

	int cx_proj = (int)round(cx + center_dx2);
	int cy_proj = (int)round(cy + center_dy1);
	static int first_bell = 1;
	if (cx_proj >= 0 && cx_proj < SIZE && cy_proj >= 0 && cy_proj < SIZE) {
		if (grid[cy_proj][cx_proj] == 'G') {
			if (first_bell) {
				printf("\a");
				first_bell = 0;
			}
		} else {
			first_bell = 1;
		}
		grid[cy_proj][cx_proj] = 'G';
		// Connect to center using Bresenham's line algorithm for smoother, continuous lines
		int x0 = cx, y0 = cy;
		int x1 = cx_proj, y1 = cy_proj;
		int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
		int dy = -abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
		int err = dx + dy;
		for (;;) {
			if (grid[y0][x0] == 'W' || grid[y0][x0] == 'G') {
				grid[y0][x0] = 'G';
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
	}
	print_matrix(grid);
	free(buf);
	free_matrix_grid(grid);
}
