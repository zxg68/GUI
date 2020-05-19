
//
// This is example code from Chapter 16.5 "An example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <iostream>
#include <sstream>
#include "Graph.h"         // get access to our graphics library facilities
#include "GUI.h"

using namespace Graph_lib;
using namespace std;

//------------------------------------------------------------------------------

struct Lines_window : Window {
    Lines_window(Point xy, int w, int h, const string& title );
private:
    // Data:
    Open_polyline lines;

    // Widgets:
    Button next_button;    // add (next_x,next_y) to lines
    Button quit_button;    // end program
    In_box next_x;
    In_box next_y;
    Out_box xy_out;
    Menu color_menu;
    Button menu_button;

    void change(Color c) { lines.set_color(c); } 

    void hide_menu() { color_menu.hide(); menu_button.show(); } 

    // actions invoked by callbacks:
    void red_pressed() { change(Colors::red); hide_menu(); }
    void blue_pressed() { change(Colors::blue); hide_menu(); } 
    void black_pressed() { change(Colors::black); hide_menu(); } 
    void menu_pressed() { menu_button.hide(); color_menu.show(); }
    void next(); 
};

//------------------------------------------------------------------------------

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
    :Window(xy,w,h,title),
    color_menu ({x_max()-70, 30}, 70, 70, Menu::vertical, "color"),
    menu_button({x_max()-80, 30}, 80, 25, "color menu", [this] { menu_pressed(); }),
    next_button({x_max()-150, 0}, 70, 20, "Next point", [this] { next(); }),
    quit_button({x_max()-70,  0}, 70, 20, "Quit", Exit),
    next_x({x_max()-380, 0}, 100, 20, "next x:"),
    next_y({x_max()-270, 0}, 100, 20, "next y:"),
    xy_out({20, 0}, 180, 20, "current (x,y):")
{
    attach(next_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(xy_out);
    xy_out.put("no point");
    color_menu.attach(new Button({0,0},0,0,"red", [this] { red_pressed(); }));
    color_menu.attach(new Button({0,0},0,0,"blue", [this] { blue_pressed(); }));
    color_menu.attach(new Button({0,0},0,0,"black", [this] { black_pressed(); }));
    attach(color_menu);
    color_menu.hide();
    attach(menu_button);
    attach(lines);
}

//------------------------------------------------------------------------------

void Lines_window::next()
{
    int x = next_x.get_int();
    int y = next_y.get_int();

    lines.add({x,y});

    // update current position readout:
    xy_out.put("(" + to_string(x) + "," + to_string(y) + ")");

    redraw();
}

//------------------------------------------------------------------------------

int main()
{
    Lines_window win({100,100},600,400,"lines");
    gui_main(win);
}

//------------------------------------------------------------------------------
