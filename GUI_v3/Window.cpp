#include "Window.h"
#include "GUI.h"
#include <assert.h>

using namespace nana;

namespace Graph_lib {

Window::Window(int w, int h, const string& title)
	: form{API::make_center(w, h)}, dw{*this}
{	caption(title); }

Window::Window(Point xy, int w, int h, const string& title)
	: form{rectangle(xy.x, xy.y, w, h)}, dw{*this}
{	caption(title); }

//---------------------------------------------------- 

void Window::redraw()
{
	dw.redraw();
	for (auto c : canvases) c->redraw();
	show();
}

void Window::attach(Widget& w)
{
	w.attach(*this);	// let the Widget create its form
}

void Window::detach(Widget& b)
{
	b.hide();
}

void Window::place(const char* fld, Widget & w)
{
	attach(w);
	assert(w.pw);
	(*this)[fld] << *w.pw;
}

void gui_main(Window& win) { win.redraw(); nana::exec(); }

} // Graph
