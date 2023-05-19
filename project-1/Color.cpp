#include <algorithm>
#include <iomanip>
#include "Color.hpp"

namespace prog {
    Color::Color() { 
        red_ = 0;
        blue_ = 0;
        green_ = 0;
    }
    Color::Color(const Color& other) {
        red_ = other.red_;
        blue_ = other.blue_;
        green_ = other.green_;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) : red_(red), green_(green), blue_(blue) {}

    rgb_value Color::red() const {return red_;}
    rgb_value Color::green() const {return green_;}
    rgb_value Color::blue() const {return blue_;}
    rgb_value& Color::red() {return red_;}
    rgb_value& Color::green() {return green_;}
    rgb_value& Color::blue() {return blue_;}

    void Color::invert_pixel(){
        red_ = 255 - red_;
        green_ = 255 - green_;
        blue_ = 255 - blue_;
    }

    void Color::average_pixel(){
        red_ = green_ = blue_ = (red_ + green_ + blue_) / 3;
    }

    void Color::copy_pixel(Color& pixel){
        *this = pixel;
    }

    void Color::swap_colors(rgb_value r, rgb_value g, rgb_value b){
        red_ = r;
        green_ = g;
        blue_ = b;
    }

    void Color::swap_pixel(Color& pixel){
        std::swap(*this, pixel);
    }

    bool Color::equal_pixel(rgb_value r, rgb_value g, rgb_value b){
        return (red_  == r and green_ == g and blue_ == b) ? true : false;
    }

    const void Color::pixel_to_hex(std::string& value) const{
        std::ostringstream result;
        result << '#';
        result << std::hex << std::setw(2) << std::setfill('0') << (int) red_
               << std::hex << std::setw(2) << std::setfill('0') << (int) green_
               << std::hex << std::setw(2) << std::setfill('0') << (int) blue_;
        value = result.str();
    }

    bool Color::operator<(const Color& a) const{
        if (red_ < a.red_) return true;
        else if (red_ == a.red_ and green_ < a.green_) return true;
        else if (red_ == a.red_ and green_ == a.green_ and blue_ < a.blue_) return true;
        return false;
    }
}