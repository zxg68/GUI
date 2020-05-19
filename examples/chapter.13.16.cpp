
//
// This is example code from Chapter 13.16 "Mark" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

int main()
{
    using namespace Graph_lib;   // our graphics facilities are in Graph_lib

    Simple_window win = {{100,100},600,400,"Circles with centers"};

    Circle c1 = {{100,200},50};
    Circle c2 = {{150,200},100};
    Circle c3 = {{200,200},150};

    win.attach(c1);
    win.attach(c2);
    win.attach(c3);

    Mark m1 = {{100,200},'x'};
    Mark m2 = {{150,200},'y'};
    Mark m3 = {{200,200},'z'};
    c1.set_color(Colors::blue);
    c2.set_color(Colors::red);
    c3.set_color(Colors::green);
    win.attach(m1);
    win.attach(m2);
    win.attach(m3);

    win.wait_for_button();       // Display!
}

//------------------------------------------------------------------------------
