#include "Image.hpp"
#include <vector>
#include <algorithm>

namespace prog{
  Image::Image(int columns, int lines, const Color &fill){
    c = columns;
    l = lines;

    Color** pointer = new Color*[l];
    for (int index = 0; index < l; index++) {pointer[index] = new Color[c];}

    for (int line = 0; line < l; line++){
      for (int column = 0; column < c; column++){
        pointer[line][column] = fill;
      }
    }
    p = pointer;
  }

  Image::~Image(){
    for (int index = 0; index < l; index++) {delete [] p[index];}
    delete [] p;
  }
  int Image::width() const {return c;}
  int Image::height() const {return l;}

  Color& Image::at(int column, int line) {return p[line][column];}
  const Color& Image::at(int column, int line) const {return p[line][column];}

  void Image::invert_image(){
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        p[line][col].invert_pixel();
      }
    }
  }

  void Image::to_gray_scale(){
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        p[line][col].average_pixel();
      }
    }
  }

  void Image::replace(int r1, int g1, int b1, int r2, int g2, int b2){
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        if (p[line][col].equal_pixel(r1, g1, b1)){
          p[line][col].swap_colors(r2, g2, b2);
        }
      }
    }
  }

  void Image::fill(int x, int y, int w, int h, int r, int g, int b){
    for (int line = y; line < y + h; line++){
      for (int col = x; col < x + w; col++){
        p[line][col].swap_colors(r, g, b);
      }
    }
  }

  void Image::h_mirror(){
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c / 2; col++){
        p[line][col].swap_pixel(p[line][c - 1 - col]);
      }
    }
  }

  void Image::v_mirror(){
    for (int line = 0; line < l / 2; line++){
      for (int col = 0; col < c; col++){
        p[line][col].swap_pixel(p[l - 1 - line][col]);
      }
    }
  }

  void Image::add(Image* im, int r, int g, int b, int w, int h){
    for (int line = 0; line < im->l; line++){
      for (int col = 0; col < im->c; col++){
        if (!(im->p[line][col].equal_pixel(r, g, b))){
          p[line + h][col + w].copy_pixel(im->p[line][col]);
        }
      }
    }
  }

  void Image::crop(Image* im, int x, int y, int w, int h){
    for (int line = 0; line < h; line++){
      for (int col = 0; col < w; col++){
        im->p[line][col].copy_pixel(p[line + y][col + x]);
      }
    }
  }

  void Image::rotate(Image* im, int direction){
    for (int col = 0; col < c; col++){
      for (int line = 0; line < l; line++){
        if (direction == 0){
          im->p[c- 1- col][line].copy_pixel(p[line][col]);
        }
        else{
          im->p[col][l - 1 - line].copy_pixel(p[line][col]);
        }
      }
    }
  }

  Color Image::find_median(int pos_l, int pos_c, int window){
    std::vector<std::vector<int>> colors(3);
    window /= 2;
    for (int line = std::max(0, pos_l - window); line <= std::min(l - 1, pos_l + window); line++){
      for (int col = std::max(0, pos_c - window); col <= std::min(c - 1, pos_c + window); col++){
        colors[0].push_back(p[line][col].red());
        colors[1].push_back(p[line][col].green());
        colors[2].push_back(p[line][col].blue());
      }
    }
    for (std::vector<int>& v : colors) {std::sort(v.begin(), v.end());}

    int mid = colors[0].size() / 2;
    if (colors[0].size() % 2 == 0){
      Color pixel((colors[0].at(mid - 1) + colors[0].at(mid)) / 2,
                  (colors[1].at(mid - 1) + colors[1].at(mid)) / 2,
                  (colors[2].at(mid - 1) + colors[2].at(mid)) / 2);
      return pixel;
    }
    else{
      Color pixel(colors[0].at(mid),
                  colors[1].at(mid),
                  colors[2].at(mid));
      return pixel;
    }
  }

  void Image::median_filter(Image* im, int window){
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        im->p[line][col] = this->find_median(line, col, window);
      }
    }
  }
}