#include "include/gmt.h"
void init_matrix(char **grid, bool mark_center)
{
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
			if (mark_center && dx * dx + dy * dy == 0) {
				grid[i][j] = 'X'; // center marker
			}
		}
	}
}
void print_matrix(char **grid)
{
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
			if (grid[i][j] == 'R') {
				printf("\033[31m█▊");
			} else if (grid[i][j] == 'G') {
				printf("\033[32m█▊");
			} else if (grid[i][j] == 'W') {
				printf("\033[37m█▊");
			} else {
				printf("  ");
			}
		}
		printf("\n");
	}
	fflush(stdout);
}
char **init_matrix_grid(void)
{
	char **grid = malloc(SIZE * sizeof(char *));
	for (int i = 0; i < SIZE; i++) {
		grid[i] = malloc(SIZE * sizeof(char));
	}
	return grid;
}
void free_matrix_grid(char **grid)
{
	for (int i = 0; i < SIZE; i++) {
		free(grid[i]);
	}
	free(grid);
}