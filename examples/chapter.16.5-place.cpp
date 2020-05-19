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
	next_button{"Next point", [this] { next(); }},
	quit_button{"Quit", Exit},
	next_x{"next x:"},
	next_y{"next y:"},
	xy_out{"current (x,y):"}
{
	div("vert<weight=20<><out min=140><><in weight=200 gap=20><weight=20><btn weight=150 gap=10>>");
	place("btn", next_button);
	place("btn", quit_button);
	place("in", next_x);
	place("in", next_y);
	place("out", xy_out);
	collocate();
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
