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
// constexpr float RIGHT = CENTER_X*2-LEFT;
// constexpr float REAL_WIDTH = RIGHT-LEFT;
// constexpr float REAL_HEIGHT = TOP-BOTTOM;
#define MAX_ITERATIONS 24
#define PERIOD 40

using namespace cimg_library;
using namespace std;

int iter_julia(float x0, float y0, const float cx, const float cy) {
  // if(x0>0)return MAX_ITERATIONS;
  // else return 0;
  static float temp_x;
  for (int i = 0; i < MAX_ITERATIONS; i++) {
    if (x0 * x0 + y0 * y0 > 4)
      return i;
    temp_x = x0;
    x0 = x0 * x0 - y0 * y0 + cx;
    y0 = temp_x * y0 * 2 + cy;
  }
  return MAX_ITERATIONS;
}

// uint8_t mtx[WIDTH*HEIGHT*3];

int main() {
  int ix, iy, res, idx, frame = 0;
  float x, y, cx = 0.2, cy = 0, angle;
  uint8_t br;
  // CImg<uint8_t> img(WIDTH,HEIGHT,1,3);
  CImg<uint8_t> img(WIDTH, HEIGHT, 1, 1);
  uint8_t *mtx = img.data();
  // for(int i=0;i<WIDTH*HEIGHT*3;i++)mtx[i]=200;
  CImgDisplay disp(img, "animation");
  for (; !disp.is_closed(); frame++) {
    frame %= PERIOD * 2;
    angle = frame * 3.1415f / PERIOD;
    idx = 0;
    for (iy = 0; iy < HEIGHT; iy++) {
      y = -iy / SCALE_Y + TOP;
      for (ix = 0; ix < WIDTH; ix++, idx++) {
        x = ix / SCALE_X + LEFT;
        res = iter_julia(x, y, sinf(angle * 2), cosf(angle * 3) * .75f);
        // res=iter_julia(x,y,0,0);
        br = res * 255 / MAX_ITERATIONS;
        mtx[idx] = br;
        // mtx[idx+WIDTH*HEIGHT]=br;
        // mtx[idx+WIDTH*HEIGHT*2]=br;
      }
    }
    disp.display(img);
    disp.wait(50);
  }
  return 0;
}