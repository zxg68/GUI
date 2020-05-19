
//
// This is example code from Chapter 13.12 "Circle " of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

int main()
{
    using namespace Graph_lib;   // our graphics facilities are in Graph_lib

    Simple_window win = {{100,100},600,400,"Circles"};

    Circle c1 = {{100,200},50};
    Circle c2 = {{150,200},100};
    Circle c3 = {{200,200},150};

    win.attach(c1);
    win.attach(c2);
    win.attach(c3);

    win.wait_for_button();       // Display!
}

//------------------------------------------------------------------------------
