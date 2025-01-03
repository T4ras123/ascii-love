#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define SCREEN_WIDTH 250
#define SCREEN_HEIGHT 60
#define PI 3.14159265358979323846

void clear_screen() {
    printf("\x1b[H");
}

void render_frame(float A, float B) {

    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[SCREEN_HEIGHT][SCREEN_WIDTH];
    double zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    const char* luminance = ".,-~:;=!*#$@";

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            output[i][j] = ' ';
            zbuffer[i][j] = -INFINITY;
        }
    }

    for (double u = 0; u < 2 * PI; u += 0.02) {
        for (double v = 0; v < PI; v += 0.02) {
            double x = sin(v) * (15 * sin(u) - 4 * sin(3 * u));
            double y = 8 * cos(v);
            double z = sin(v) * (15 * cos(u) - 5 * cos(2 * u) - 2 * cos(3 * u) - cos(4 * u));

            double x1 = x * cosB + z * sinB;
            double y1 = y;
            double z1 = -x * sinB + z * cosB;

            double x_rot = x1;
            double y_rot = y1 * cosA - z1 * sinA;
            double z_rot = y1 * sinA + z1 * cosA;

            double z_offset = 70;
            double ooz = 1 / (z_rot + z_offset);
            int xp = (int)(SCREEN_WIDTH / 2 + x_rot * ooz * SCREEN_WIDTH * 0.5);
            int yp = (int)(SCREEN_HEIGHT / 2 - y_rot * ooz * SCREEN_HEIGHT * 0.5);

            double nx = sin(v) * (15 * cos(u) - 4 * cos(3 * u));
            double ny = 8 * -sin(v) * sin(v);
            double nz = cos(v) * (15 * sin(u) - 5 * sin(2 * u) - 2 * sin(3 * u) - sin(4 * u));

            double nx1 = nx * cosB + nz * sinB;
            double ny1 = ny;
            double nz1 = -nx * sinB + nz * cosB;

            double nx_rot = nx1;
            double ny_rot = ny1 * cosA - nz1 * sinA;
            double nz_rot = ny1 * sinA + nz1 * cosA;

            double length = sqrt(nx_rot * nx_rot + ny_rot * ny_rot + nz_rot * nz_rot);
            nx_rot /= length;
            ny_rot /= length;
            nz_rot /= length;

            double lx = -0.57735;
            double ly = -0.57735;
            double lz = -0.57735;

            double L = nx_rot * lx + ny_rot * ly + nz_rot * lz;
            int luminance_index = (int)((L + 1) * 5.5);

            if (xp >= 0 && xp < SCREEN_WIDTH && yp >= 0 && yp < SCREEN_HEIGHT) {
                if (ooz > zbuffer[yp][xp]) {
                    zbuffer[yp][xp] = ooz;
                    output[yp][xp] = luminance[luminance_index < 0 ? 0 : (luminance_index > 11 ? 11 : luminance_index)];
                }
            }
        }
    }

    printf("\x1b[H");
    const char* colors[] = {
        "\x1b[38;5;52m",
        "\x1b[38;5;88m",
        "\x1b[38;5;160m",
        "\x1b[38;5;196m"
    };
    int total_colors = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (output[i][j] != ' ') {
                int index = strchr(luminance, output[i][j]) - luminance;
                if (index < 0) index = 0;
                if (index >= 12) index = 11;
                int color_index = index / 3;
                if (color_index >= total_colors) color_index = total_colors - 1;
                printf("%s%c", colors[color_index], output[i][j]);
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    printf("\x1b[0m");
}

int main() {
    float A = 0;
    float B = 0;
    while (1) {
        clear_screen();
        render_frame(A, B);
        usleep(45000);
        A += 0.05;
        B += 0.03;
        if (A > 2 * PI) A -= 2 * PI;
        if (B > 2 * PI) B -= 2 * PI;
    }
    return 0;
}