//
// This is example code from Chapter 16.5 "An example" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <iostream>
#include <sstream>
#include "Graph.h"        // get access to our graphics library facilities
#include "GUI.h"
#include "Window.h"

using namespace Graph_lib;
using namespace std;

//------------------------------------------------------------------------------

struct Lines_window : Window {
	Lines_window(Point xy, int w, int h, const string& title);
	Open_polyline lines;
private:
	Button next_button;        // add (next_x,next_y) to lines
	Button quit_button;
	In_box next_x;
	In_box next_y;
	Out_box xy_out;

	void next();
};

//------------------------------------------------------------------------------

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	next_button{{x_max() - 150, 0}, 70, 20, "Next point", [this] { next(); }},
	quit_button{{x_max() - 70, 0}, 70, 20, "Quit", Exit},
	next_x{{x_max() - 360, 0}, 80, 20, "next x:"},
	next_y{{x_max() - 250, 0}, 80, 20, "next y:"},
	xy_out{{50, 0}, 160, 20, "current (x,y):"}
{
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);
	attach(lines);
}

//------------------------------------------------------------------------------

void Lines_window::next()
{
	int x = next_x.get_int();
	int y = next_y.get_int();

	lines.add({ x, y });

	// update current position readout:
	stringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	redraw();
}

//------------------------------------------------------------------------------

int main()
{
	Lines_window win = {{100, 100}, 600, 400, "lines"};
	gui_main(win);
}

//------------------------------------------------------------------------------
