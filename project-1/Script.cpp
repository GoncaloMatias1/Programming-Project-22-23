#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

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
            std::string filename;
            int r,g,b,w,h;
            input >> filename >> r >> g >> b >> w >> h;
            add(filename, r, g, b, w, h);
            continue;
        }
        if (command == "crop"){
            int x,  y,  w,  h;
            input >> x >> y >> w >> h;
            crop(x, y, w, h);
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
            int ws;
            input >> ws;
            ws /= 2;
            median_filter(ws);
            continue;
        }
        if (command == "xpm2_open"){
            std::string filename;
            input >> filename;
            xpm2_open(filename);
        }
        if (command == "xpm2_save"){
            std::string filename;
            input >> filename;
            xpm2_save(filename);
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

    void Script::invert(){
        image->invert_image();
    }

    void Script::to_gray_scale(){
        image->to_gray_scale();
    }

    void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2) {
        image->replace(r1, g1, b1, r2, g2, b2);
    }

    void Script::fill(int x, int y, int w, int h, int r, int g, int b) {
        image->fill(x, y, w, h, r, g, b);
    }
    
    void Script::h_mirror() {
        image->h_mirror();
    }

    void Script::v_mirror() {
        image->v_mirror();
    }

    void Script::add(std::string filename, int r, int g, int b, int w, int h){
        image->add(filename, r, g, b, w, h);
    }
    
    void Script::crop(int x, int y, int w, int h){
        image->crop(x, y, w, h);
    }

    void Script::rotate_left(){
        image->rotate(0);
    }

    void Script::rotate_right(){
        image->rotate(1);
    }

    void Script::median_filter(int ws){
        image->median_filter(ws);
    }

    void Script::xpm2_open(const std::string filename){
        clear_image_if_any();
        image = loadFromXPM2(filename);
    }

    void Script::xpm2_save(const std::string& filename){
        saveToXPM2(filename, image);
    }
}