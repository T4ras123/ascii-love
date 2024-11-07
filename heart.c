#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float theta_spacing = 0.07;
const float phi_spacing = 0.02;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;
const int screen_width = 100;
const int screen_height = 60;
const float K1 = screen_width * K2 * 3 / (8 * (R1 + R2));

void render_frame(float A, float B) {
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[screen_width][screen_height];
    float zbuffer[screen_width][screen_height];

    for (int i = 0; i < screen_width; i++) {
        for (int j = 0; j < screen_height; j++) {
            output[i][j] = ' ';
            zbuffer[i][j] = 0.0f;
        }
    }

      for (float t = -M_PI; t <= M_PI; t += 0.01) {
      // Heart parametric equations
      float x = 16 * pow(sin(t), 3);
      float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
      float z = 0;

      int xp = (int)(screen_width / 2 + x);
      int yp = (int)(screen_height / 2 - y);
      

      if (xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
          output[xp][yp] = '*';
        }
      }

    // Print the output array
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            putchar(output[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    float A = 0, B = 0;
    render_frame(A, B);
    return 0;
}