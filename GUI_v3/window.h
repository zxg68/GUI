#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

#include "nana.h"
#include "Point.h"
#include "Drawing.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

namespace Graph_lib {

class Shape;	// "forward declare" Shape
class Widget;
class Canvas;

class Window : public nana::form { 
public: 
	Window(int w, int h, const string& title);			// let the system pick the location
	Window(Point xy, int w, int h, const string& title);	// top left corner in xy
	virtual ~Window() { }

	int x_max() const { return size().width;  }
	int y_max() const { return size().height; }

	void resize(unsigned w, unsigned h) { size({w, h}); }

	void set_label(const string& s) { caption(s); }

	void attach(Widget& w);
	void detach(Widget& w);		// remove w from window (deactivate callbacks)
	void place(const char* fld, Widget& w);

	void attach(Shape& s)     { dw.attach(s); }
	void detach(Shape& s)     { dw.detach(s); }
	void put_on_top(Shape& s) { dw.put_on_top(s); }
	void set_mouse_callback(Drawing::Mouse_cb cb) { dw.set_mouse_callback(cb); }
	void set_size_callback (Drawing::Size_cb cb)  { dw.set_size_callback(cb);  }

	void redraw();
     
private:
//	int w, h;					// window size
	Drawing dw;
	friend class Canvas;
	vector<Canvas*> canvases;
}; 

void gui_main(Window& win);	// invoke GUI library's main event loop

inline nana::rectangle screen_area() { return nana::screen{}.get_primary().workarea(); }
inline int x_max() { return screen_area().width; }	// width of screen in pixels
inline int y_max() { return screen_area().height; }	// height of screen in pixels

}
#endif
