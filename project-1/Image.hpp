#ifndef prog_Image_hpp
#define prog_Image_hpp
#include "Color.hpp"
#include <map>

namespace prog
{
  class Image
  {
  private:
    int l;
    int c;
    Color** p;

  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;

    void invert_image();
    void to_gray_scale();
    void replace(int r1, int g1, int b1, int r2, int g2, int b2);
    void fill(int x, int y, int w, int h, int r, int g, int b);
    void h_mirror();
    void v_mirror();
    void add(Image* im, int r, int g, int b, int w, int h);
    void crop(Image* im, int x, int y, int w, int h);
    void rotate(Image* im, int direction); // 0 -> left, 1-> right
    void median_filter(Image* im, int window);
    Color find_median(int pos_l, int pos_c, int window);
    void pixel_to_char(std::map<Color, char>& simbols) const;
  };
}
#endif