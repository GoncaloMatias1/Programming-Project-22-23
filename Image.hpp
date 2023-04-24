#ifndef prog_Image_hpp
#define prog_Image_hpp
#include "Color.hpp"

namespace prog
{
  class Image
  {
  private:
    int row;
    int col;
    Color** p_;
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif

//Using vectors

// #ifndef prog_Image_hpp
// #define prog_Image_hpp
// #include "Color.hpp"
// #include <vector>

// namespace prog
// {
//   class Image
//   {
//   private:
//     int row;
//     int col;
//     std::vector<std::vector<Color>> p_;
//   public:
//     Image(int w, int h, const Color &fill = {255, 255, 255});
//     ~Image();
//     int width() const;
//     int height() const;
//     Color &at(int x, int y);
//     const Color &at(int x, int y) const;
//   };
// }
// #endif