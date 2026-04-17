#include <cmath>

#ifdef __linux__
#define cimg_display 3
#endif

#include <CImg.h>

#define WIDTH 800
#define HEIGHT 480
#define RATIO 1.0f
#define TOP 1.5f
#define BOTTOM -1.5f
#define CENTER_X 0.0f
constexpr float SCALE_Y = (float)HEIGHT / (TOP - BOTTOM);
constexpr float SCALE_X = SCALE_Y * RATIO;
constexpr float LEFT = -(float)WIDTH / SCALE_X / 2 + CENTER_X;
#define MAX_ITERATIONS 24
#define PERIOD 200

using namespace cimg_library;
using namespace std;

int iter_julia(float x0, float y0, const float cx, const float cy) {
  for (int i = 0; i < MAX_ITERATIONS; i++) {
    if (x0 * x0 + y0 * y0 > 4)
      return i;
    float temp_x = x0;
    x0 = x0 * x0 - y0 * y0 + cx;
    y0 = temp_x * y0 * 2 + cy;
  }
  return MAX_ITERATIONS;
}

int main() {
  CImg<uint8_t> img(WIDTH, HEIGHT, 1, 1);

  uint8_t *mtx = img.data();

  CImgDisplay disp(img, "animation");

  for (int frame = 0; !disp.is_closed(); frame++) {
    frame %= PERIOD * 2;
    float angle = frame * numbers::pi_v<float> / PERIOD;
    int idx = 0;

    for (int iy = 0; iy < HEIGHT; iy++) {
      float y = -iy / SCALE_Y + TOP;

      for (int ix = 0; ix < WIDTH; ix++) {
        idx++;
        float x = ix / SCALE_X + LEFT;
        int res = iter_julia(x, y, sinf(angle * 2), cosf(angle * 3) * .75f);
        uint8_t brightness = res * 255 / MAX_ITERATIONS;
        mtx[idx] = brightness;
      }
    }
    disp.display(img);
    disp.wait(20);
  }
  return 0;
}