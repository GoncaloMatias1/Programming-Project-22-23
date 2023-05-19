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

    Color** create_array(int w, int h);
    void clear_array();
    void swap_PrivParameters(int lines, int collumns, Color** array);
    void invert_image() const;
    void to_gray_scale() const;
    void replace(int r1, int g1, int b1, int r2, int g2, int b2) const;
    void fill(int x, int y, int w, int h, int r, int g, int b) const;
    void h_mirror() const;
    void v_mirror() const;
    void add(const std::string& file, int r, int g, int b, int w, int h) const;
    void crop(int x, int y, int w, int h);
    /**
     * direction = 0 -> left
     * direction = 1 -> right
    */
    void rotate(int direction);
    void median_filter(const int& swindow);
    Color find_median(const int& pos_l, const int& pos_c, const int& window) const;
    void pixel_to_char(std::map<Color, char>& simbols) const;
  };
}
#endif