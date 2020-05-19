
//
// This is example code from Chapter 13.6 "Open polyline" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

int main()
{
    using namespace Graph_lib;   // our graphics facilities are in Graph_lib

	Simple_window win = {{100,100}, 600, 400, "Open polyline"};

    Open_polyline opl = {{100,100},{150,200},{250,250},{300,200}};

    win.attach(opl);
    win.wait_for_button();       // Display!
}

//------------------------------------------------------------------------------
