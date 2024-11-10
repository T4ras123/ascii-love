#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 40
#define PI 3.14159265358979323846

void clear_screen() {
    printf("\x1b[H"); // Moves the cursor to the top-left corner
}

void render_frame(float A, float B){

    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[SCREEN_HEIGHT][SCREEN_WIDTH];
    double zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    // Initialize buffers
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            output[i][j] = ' ';
            zbuffer[i][j] = -INFINITY;
        }
    }

    for (double u = 0; u < 2 * PI; u += 0.02) {
        for (double v = 0; v < PI; v += 0.02) {
            // Heart parametric equations
            double x = sin(v) * (15 * sin(u) - 4 * sin(3 * u));
            double y = 8 * cos(v);
            double z = sin(v) * (15 * cos(u) - 5 * cos(2 * u) - 2 * cos(3 * u) - cos(4 * u));

            // Rotate around Y-axis
            double x1 = x * cosB + z * sinB;
            double y1 = y;
            double z1 = -x * sinB + z * cosB;

            // Rotate around X-axis
            double x_rot = x1;
            double y_rot = y1 * cosA - z1 * sinA;
            double z_rot = y1 * sinA + z1 * cosA;

            // Projection
            double z_offset = 70;
            double ooz = 1 / (z_rot + z_offset);
            int xp = (int)(SCREEN_WIDTH / 2 + x_rot * ooz * SCREEN_WIDTH);
            int yp = (int)(SCREEN_HEIGHT / 2 - y_rot * ooz * SCREEN_HEIGHT);

            // Calculate normals
            double nx = sin(v) * (15 * cos(u) - 4 * cos(3 * u));
            double ny = 8 * -sin(v) * sin(v);
            double nz = cos(v) * (15 * sin(u) - 5 * sin(2 * u) - 2 * sin(3 * u) - sin(4 * u));

            // Rotate normals around Y-axis
            double nx1 = nx * cosB + nz * sinB;
            double ny1 = ny;
            double nz1 = -nx * sinB + nz * cosB;

            // Rotate normals around X-axis
            double nx_rot = nx1;
            double ny_rot = ny1 * cosA - nz1 * sinA;
            double nz_rot = ny1 * sinA + nz1 * cosA;

            // Normalize normal vector
            double length = sqrt(nx_rot * nx_rot + ny_rot * ny_rot + nz_rot * nz_rot);
            nx_rot /= length;
            ny_rot /= length;
            nz_rot /= length;

            // Light direction
            double lx = 0;
            double ly = 0;
            double lz = -1;

            // Dot product for luminance
            double L = nx_rot * lx + ny_rot * ly + nz_rot * lz;
            int luminance_index = (int)((L + 1) * 5.5);

            if (xp >= 0 && xp < SCREEN_WIDTH && yp >= 0 && yp < SCREEN_HEIGHT) {
                if (ooz > zbuffer[yp][xp]) {
                    zbuffer[yp][xp] = ooz;
                    const char* luminance = ".,-~:;=!*#$@";
                    luminance_index = luminance_index < 0 ? 0 : (luminance_index > 11 ? 11 : luminance_index);
                    output[yp][xp] = luminance[luminance_index];
                }
            }
        }
    }

    // Print the output array
    printf("\x1b[H");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        fwrite(output[i], 1, SCREEN_WIDTH, stdout);
        putchar('\n');
    }
}

int main() {
    float A = 0;
    float B = 0;
    while (1) {
        clear_screen();
        render_frame(A, B);
        usleep(45000); // Delay for smooth animation
        A += 0.05;     // Rotation speed around X-axis
        B += 0.03;     // Rotation speed around Y-axis
        if (A > 2 * PI) A -= 2 * PI;
        if (B > 2 * PI) B -= 2 * PI;
    }
    return 0;
}
