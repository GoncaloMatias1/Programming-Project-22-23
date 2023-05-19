#include "XPM2.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>


namespace prog {
    Image* loadFromXPM2(const std::string& file){
        std::string line;
        std::ifstream in(file);
        std::getline(in, line); //skip the file header

        int w, h, number_colors;
        std::getline(in, line);
        std::istringstream aux(line);
        aux >> w >> h >> number_colors;

        std::map<char, Color> colors;
        for (int k = 0; k < number_colors; k++){
            char simbol, ch;
            std::string value;

            std::getline(in, line);
            std::istringstream aux(line);
            aux >> simbol >> ch >> value;
            unsigned char r, g, b;
            r = std::stoi(value.substr(1, 2), 0, 16);
            g = std::stoi(value.substr(3, 2), 0, 16);
            b = std::stoi(value.substr(5, 2), 0, 16);
            colors[simbol].copy_colors(r, g, b);
        }

        Image* im = new Image(w,h);
        for (int l = 0; l < h; l++){
            std::getline(in, line);
            for (int c = 0; c < w; c++){
                im->at(c,l).copy_pixel(colors[line[c]]);
            }
        }
        return im;
    }

    void saveToXPM2(const std::string& file, const Image* image){
        std::ofstream XPM2;
        std::map<Color, char> simbols;
        XPM2.open(file);
        image->pixel_to_char(simbols);
        XPM2 << "! XPM2" << "\n";
        XPM2 << image->width() << " " << image->height() << " " << simbols.size() << " 1" << "\n";
        for (auto& pair : simbols){
            std::string value;
            pair.first.pixel_to_hex(value);
            XPM2 << pair.second << " c " << value;
            XPM2 << "\n";
        }
        for (int line = 0; line < image->height(); line++){
            for (int col = 0; col < image->width(); col++){
                XPM2 << simbols[image->at(col, line)];
            }
            XPM2 << "\n";
        }
        XPM2.close();
    }
}

