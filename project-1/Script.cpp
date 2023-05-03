#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <algorithm>
#include <vector>

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
    string command;
    while (input >> command) {
        cout << "Executing command '" << command << "' ..." << endl;
        if (command == "open") {
            open();
            continue;
        }
        if (command == "blank") {
            blank();
            continue;
        }
        // Other commands require an image to be previously loaded.
        if (command == "save") {
            save();
            continue;
        }
        if (command == "invert") {
            invert();
            continue;
        }
        if (command == "to_gray_scale") {
            to_gray_scale();
            continue;
        }
        if (command == "replace") {
            int r1, g1, b1, r2, g2, b2;
            input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
            replace(r1, g1, b1, r2, g2, b2);
            continue;
        }
        if (command == "fill") {
            int x, y, w, h, r, g, b;
            input >> x >> y >> w >> h >> r >> g >> b;
            fill(x, y, w, h, r, g, b);
            continue;
        }
        if (command == "h_mirror") {
            h_mirror();
            continue;
        }
        if (command == "v_mirror") {
            v_mirror();
            continue;
        }
        if (command == "add"){
            add();
            continue;
        }
        if (command == "crop"){
            crop();
            continue;
        }
        if (command == "rotate_left"){
            rotate_left();
            continue;
        }
        if (command == "rotate_right"){
            rotate_right();
            continue;
        }
        if (command == "median_filter"){
            median_filter();
            continue;
        }
        // TODO: Add any other commands here.
    }
}

    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            Color &pixel = image->at(x, y);
            pixel.red() = 255 - pixel.red();
            pixel.green() = 255 - pixel.green();
            pixel.blue() = 255 - pixel.blue();
        }
    }
    }
    void Script::to_gray_scale() {
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            Color &pixel = image->at(x, y);
            int v = (pixel.red() + pixel.green() + pixel.blue()) / 3;
            pixel.red() = v;
            pixel.green() = v;
            pixel.blue() = v;
        }
    }
    }
    void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2) {
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            Color &pixel = image->at(x, y);
            if (pixel.red() == r1 && pixel.green() == g1 && pixel.blue() == b1) {
                pixel.red() = r2;
                pixel.green() = g2;
                pixel.blue() = b2;
            }
        }
    }
    }
    void Script::fill(int x, int y, int w, int h, int r, int g, int b) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            Color &pixel = image->at(i, j);
            pixel.red() = r;
            pixel.green() = g;
            pixel.blue() = b;
        }
    }
    }
    void Script::h_mirror() {
    int width = image->width();
    int height = image->height();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            std::swap(image->at(x, y), image->at(width - 1 - x, y));
        }
    }
    }
    void Script::v_mirror() {
    int width = image->width();
    int height = image->height();
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            std::swap(image->at(x, y), image->at(x, height - 1 - y));
        }
    }
    }
    void Script::add(){
        string filename;
        int r,g,b,w,h;
        input >> filename >> r >> g >> b >> w >> h;
        Image* im = loadFromPNG(filename);
        for (int y = 0; y < im->height() ; y++){
            for (int x = 0; x < im->width(); x++){
                if(!(im->at(x,y).red() == r && im->at(x,y).green() == g && im->at(x,y).blue() == b )){
                    image->at(x + w, y + h) = im->at(x,y);
                }
            }
        }
        delete im;
    }
    
    void Script::crop(){
        int x,  y,  w,  h;
        input >> x >> y >> w >> h;
        Image* result = new Image(w, h);
        for (int k = 0; k < h; k++){
            for (int i = 0; i < w; i++){
                result->at(i,k) = image->at(i + x, k + y);
            }
        }
        delete image;
        image = result;
    }

    void Script::rotate_left(){
        Image* result = new Image(image->height(), image->width());
        for (int x = 0; x < image->width(); x++){
            for (int y = 0; y < image->height(); y++){
                result->at(y, image->width() - 1 - x) = image->at(x,y);
            }
        }
        delete image;
        image = result;
    }

    void Script::rotate_right(){
        Image* result = new Image(image->height(), image->width());
        for (int x = 0; x < image->width(); x++){
            for (int y = 0; y < image->height(); y++){
                result->at(image->height() - 1 - y ,x) = image->at(x, y);
            }
        }
        delete image;
        image = result;
    }
    void Script::median_filter(){
        int ws;
        input >> ws;
        Image* result = new Image(image->width(), image->height());
        for (int l = 0; l < image->height(); l++){
            for (int c = 0; c < image->width(); c++){
                std::vector<int> red, blue, green;

                for (int i = std::max(0, l - ws / 2); i <= std::min(image->height() - 1, l + ws / 2); i++){
                    for (int j = std::max(0, c - ws / 2); j <= std::min(image->width() - 1, c + ws / 2); j++){
                            red.push_back(image->at(j,i).red());
                            green.push_back(image->at(j,i).green());
                            blue.push_back(image->at(j,i).blue());
                        
                    }
                }

                std::sort(red.begin(), red.end());
                std::sort(green.begin(), green.end());
                std::sort(blue.begin(), blue.end());
                int mid = red.size() / 2;
                if (red.size() % 2 == 0){
                    int red_ = (red.at(mid - 1) + red.at(mid)) / 2;
                    int green_ = (green.at(mid - 1) + green.at(mid)) / 2;
                    int blue_ = (blue.at(mid - 1) + blue.at(mid)) / 2;
                    Color pixel(red_, green_, blue_);
                    result->at(c,l) = pixel;
                }
                else{
                    int red_ = red.at(mid);
                    int green_ = green.at(mid);
                    int blue_ = blue.at(mid);
                    Color pixel(red_, green_, blue_);
                    result->at(c,l) = pixel;
                }
            }
        }
        delete image;
        image = result;
    }
}

