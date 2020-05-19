
//
// This is example code from Chapter 13.2 "Point and Line" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

int main()
{
    using namespace Graph_lib;    // our graphics facilities are in Graph_lib

	Simple_window win1 = {{100,100}, 600, 400, "two lines"};

    Line horizontal = {{100,100}, {200,100}}; // make a horizontal line
    Line vertical = {{150,50}, {150,150}};    // make a vertical line

    win1.attach(horizontal);      // attach the lines to the window
    win1.attach(vertical);

    win1.wait_for_button();       // Display!
}

//------------------------------------------------------------------------------
