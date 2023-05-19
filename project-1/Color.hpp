#ifndef __prog_Color_hpp__
#define __prog_Color_hpp__
#include <string>
#include <sstream>

namespace prog
{
  typedef unsigned char rgb_value;
  class Color
  {
    private:
      rgb_value red_;
      rgb_value green_;
      rgb_value blue_;

    public:
      Color();
      Color(const Color& c);
      Color(rgb_value r, rgb_value g, rgb_value b);
      rgb_value red() const;
      rgb_value& red();
      rgb_value green() const;
      rgb_value& green();
      rgb_value blue() const;
      rgb_value& blue();

      void invert_pixel();
      void average_pixel();
      void copy_pixel(Color& pixel);
      void copy_colors(rgb_value r, rgb_value g, rgb_value b);
      void swap_pixel(Color& pixel);
      bool equal_pixel(rgb_value r, rgb_value g, rgb_value b) const;
      const void pixel_to_hex(std::string& value) const;
      bool operator<(const Color& a) const;
  };
}
#endif
