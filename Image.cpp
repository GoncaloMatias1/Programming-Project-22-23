#include "Image.hpp"
#include <vector>

namespace prog{
  Image::Image(int w, int h, const Color &fill){
    col = w; row = h;
    Color** pointer = new Color*[row];
    for (int k = 0; k < row; k++){
      pointer[k] = new Color[col];
    }
    for (int k = 0; k < row; k++){
      for (int j = 0; j < col; j++){
        pointer[k][j] = fill;
      }
    }
    p_ = pointer;
  }

  Image::~Image(){
    for (int k = 0; k < row; k++){
      delete [] p_[k];
    }
    delete [] p_;
  }
  int Image::width() const {return col;}
  int Image::height() const {return row;}

  Color& Image::at(int x, int y) {return p_[y][x];}
  const Color& Image::at(int x, int y) const {return p_[y][x];}
}

//Using vectors

// #include "Image.hpp"

// namespace prog{
//   Image::Image(int w, int h, const Color &fill){
//     col = w; row = h;
//     std::vector<Color> line(col, fill);
//     p = std::vector<std::vector<Color>>(row, line);
//   }

//   Image::~Image(){}
//   int Image::width() const {return col;}
//   int Image::height() const {return row;}

//   Color& Image::at(int x, int y) {return p[y][x];}
//   const Color& Image::at(int x, int y) const {return p[y][x];}
// }