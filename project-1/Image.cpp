#include "Image.hpp"
#include <vector>
#include <algorithm>
#include <fstream>
#include "PNG.hpp"

namespace prog{
  Image::Image(int columns, int lines, const Color &fill){
    c = columns;
    l = lines;

    Color** pointer = create_array(l, c);

    for (int line = 0; line < l; line++){
      for (int column = 0; column < c; column++){
        pointer[line][column] = fill;
      }
    }
    p = pointer;
  }

  Image::~Image(){
    clear_array();
  }

  int Image::width() const {return c;}
  int Image::height() const {return l;}

  Color& Image::at(int column, int line) {return p[line][column];}
  const Color& Image::at(int column, int line) const {return p[line][column];}

  void Image::invert_image() const{
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        p[line][col].invert_pixel();
      }
    }
  }

  Color** Image::create_array(int lines, int collumns){
    Color** new_array = new Color*[lines];
    for (int index = 0; index < lines; index++) {new_array[index] = new Color[collumns];}
    return new_array;
  }

  void Image::clear_array(){
    for (int index = 0; index < l; index++) {delete [] p[index];}
    delete [] p;
  }

  void Image::swap_PrivParameters(int lines, int collumns, Color** array){
    std::swap(lines, l);
    std::swap(collumns, c);
    std::swap(array, p);
  }

  
  void Image::to_gray_scale() const{
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        p[line][col].average_pixel();
      }
    }
  }

  void Image::replace(int r1, int g1, int b1, int r2, int g2, int b2) const{
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        if (p[line][col].equal_pixel(r1, g1, b1)){
          p[line][col].copy_colors(r2, g2, b2);
        }
      }
    }
  }

  void Image::fill(int x, int y, int w, int h, int r, int g, int b) const{
    for (int line = y; line < y + h; line++){
      for (int col = x; col < x + w; col++){
        p[line][col].copy_colors(r, g, b);
      }
    }
  }

  void Image::h_mirror() const{
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c / 2; col++){
        p[line][col].swap_pixel(p[line][c - 1 - col]);
      }
    }
  }

  void Image::v_mirror() const{
    for (int line = 0; line < l / 2; line++){
      for (int col = 0; col < c; col++){
        p[line][col].swap_pixel(p[l - 1 - line][col]);
      }
    }
  }

  void Image::add(const std::string& file, int r, int g, int b, int w, int h) const{
    Image* im = loadFromPNG(file);
    for (int line = 0; line < im->l; line++){
      for (int col = 0; col < im->c; col++){
        if (!(im->p[line][col].equal_pixel(r, g, b))){
          p[line + h][col + w].copy_pixel(im->p[line][col]);
        }
      }
    }
    delete im;
  }

  void Image::crop(int x, int y, int w, int h){
    Color** new_array = create_array(h, w);
    for (int line = 0; line < h; line++){
      for (int col = 0; col < w; col++){
        new_array[line][col].copy_pixel(p[line + y][col + x]);
      }
    }
    clear_array();
    swap_PrivParameters(h, w, new_array);
  }

  void Image::rotate(int direction){
    Color** new_array = create_array(c, l);
    for (int col = 0; col < c; col++){
      for (int line = 0; line < l; line++){
        if (direction == 0){
          new_array[c- 1- col][line].copy_pixel(p[line][col]);
        }
        else if (direction == 1){
          new_array[col][l - 1 - line].copy_pixel(p[line][col]);
        }
      }
    }
    clear_array();
    swap_PrivParameters(c, l, new_array);
  }

  Color Image::find_median(const int& pos_l, const int& pos_c, const int& window) const{
    std::vector<std::vector<int>> colors(3);
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

  void Image::median_filter(const int& window){
    Color** new_array = create_array(l, c);
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        new_array[line][col] = this->find_median(line, col, window);
      }
    }
    clear_array();
    p = new_array;
  }

  void Image::pixel_to_char(std::map<Color, char>& simbols) const{
    char simbol = '!';
    for (int line = 0; line < l; line++){
      for (int col = 0; col < c; col++){
        if (simbols.find(p[line][col]) == simbols.end()){
          simbols[p[line][col]] = simbol;
          simbol++;
        }
      }
    }
  }
}