
//
// This is example code from Chapter 13.9 "Rectangle" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

int main()
{
    using namespace Graph_lib;   // our graphics facilities are in Graph_lib

    Simple_window win({100,100},600,400,"Rectangle");

    Rectangle rect00 = {{150,100},200,100}; 
	Rectangle rect11 = {{50,50}, {250,150}};
	Rectangle rect12 = {{50,150},{250,250}};      // just  below rect11
    Rectangle rect21 = {{250,50}, 200,100};       // just to the right of rect11
    Rectangle rect22 = {{250,150},200,100};       // just below rect21

    rect00.set_fill_color(Colors::yellow);
    rect11.set_fill_color(Colors::blue);
    rect12.set_fill_color(Colors::red);
    rect21.set_fill_color(Colors::green);

    win.attach(rect00);
    win.attach(rect11);
    win.attach(rect12);
    win.attach(rect21);
    win.attach(rect22);

    win.set_label("rectangles");
    win.wait_for_button();       // Display!

    rect11.move(400,0);        // to the right of rect21
    rect11.set_fill_color(Colors::white);
    
    win.set_label("rectangles 2");
    win.wait_for_button();       // Display!

	win.put_on_top(rect00);
    win.set_label("rectangles 3");
    win.wait_for_button();       // Display!

    rect00.set_color(Color::invisible);
    rect11.set_color(Color::invisible);
    rect12.set_color(Color::invisible);
    rect21.set_color(Color::invisible);
    rect22.set_color(Color::invisible);

    win.set_label("rectangles 4");
    win.wait_for_button();       // Display!
}

//------------------------------------------------------------------------------
