#include <iostream>
#include "GUI.h"
#include "window.h"

using namespace Graph_lib;
using namespace std;

struct Lines_window : Window {
	Lines_window(Point xy, int w, int h, const string& title);
private:
	Open_polyline lines;
	Button quit_button;
	Out_box xy_out;
	Canvas canvas;

	void mouse_cb(const Mouse_event& evt);
	void out_pos(Point pt);
};

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	: Window{xy, w, h, title},
	  quit_button{"Quit", Exit},
	  xy_out{"current (x,y):"}
{
	div("vert<weight=20<><out min=135><><quit weight=70>><draw>");
	place("quit", quit_button);
	place("out", xy_out);
	place("draw", canvas);
	collocate();
	canvas.attach(lines);
	canvas.set_mouse_callback([this](const Mouse_event& evt) { mouse_cb(evt); });
}

void Lines_window::mouse_cb(const Mouse_event& evt)
{
	if (evt.type == Mouse_event::push)
		lines.add(evt.pos);
	else if (evt.type == Mouse_event::drag) {
		auto last = lines.number_of_points() - 1;
		if (last < 0) return;
		if (last == 0) lines.add(evt.pos);
		else lines.set_point(last, evt.pos);
	}
	else return;

	out_pos(evt.pos);
	redraw();
}

void Lines_window::out_pos(Point pos)
{
	xy_out.put("(" + to_string(pos.x) + "," + to_string(pos.y) + ")");
}

int main()
{
	Lines_window win = {{100,100}, 600, 400, "lines"};
	gui_main(win);
}
