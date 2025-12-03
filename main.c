#include "include/gmt.h"
char *read_line_from_stdin()
{
	static char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {
		size_t len = strlen(buffer);
		if (len < sizeof(buffer) - 1) {
			buffer[len] = (char)ch;
			buffer[len + 1] = '\0';
		}
	}
	if (ch == EOF && strlen(buffer) == 0) {
		return NULL; // EOF and no data
	}
	return buffer;
}
void on_exit(int unused)
{
	printf("\033[?25h"); // Show cursor
}
int main(int argc, char **argv)
{
	// Register exit handler
	signal(SIGINT, on_exit);
	signal(SIGTERM, on_exit);
	on_exit(0);
	watchdog();
	system("clear");
	// Check if stdin is FIFO
	struct stat st;
	if (fstat(fileno(stdin), &st) == -1) {
		perror("fstat");
		return 1;
	}
	if (!S_ISFIFO(st.st_mode)) {
		fprintf(stderr, "Error: stdin is not a FIFO\n");
		return 1;
	}
	if (argc < 2) {
		while (1) {
			char *line = read_line_from_stdin();
			double x, y, z, w, sx, sy, sz;
			if (line == NULL) {
				return 0;
			}
			if (sscanf(line, "%lf %lf %lf %lf", &x, &y, &z, &w) != 4) {
				continue;
			}
			line = read_line_from_stdin();
			if (line == NULL) {
				return 0;
			}
			if (sscanf(line, "%lf %lf %lf", &sx, &sy, &sz) != 3) {
				continue;
			}
			int mode = 0;
			if (shake_detection(sx, sy, sz)) {
				mode = 1 - mode;
			}
			struct compass_and_level cal = compute_compass_and_level_data(x, y, z, w);
			if (mode == 0) {
				show_compass_matrix(cal.angle);
			} else if (mode == 1) {
				show_level_matrix(cal.roll, cal.pitch);
			}
		}
		return 0;
	}
	while (1) {
		char *line = read_line_from_stdin();
		double x, y, z, w;
		if (line == NULL) {
			return 0;
		}
		if (sscanf(line, "%lf %lf %lf %lf", &x, &y, &z, &w) != 4) {
			continue;
		}
		struct compass_and_level cal = compute_compass_and_level_data(x, y, z, w);
		if (argc >= 2 && strcmp(argv[1], "compass") == 0) {
			show_compass_matrix(cal.angle);
		} else if (argc >= 2 && strcmp(argv[1], "level") == 0) {
			show_level_matrix(cal.roll, cal.pitch);
		} else {
			fprintf(stderr, "Specify 'compass' or 'level' as argument\n");
			return 1;
		}
	}
	return 0;
}