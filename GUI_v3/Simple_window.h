
#include "GUI.h"	// for Simple_window only (doesn't really belong in Window.h)
#include <memory>	// unique_ptr

using namespace Graph_lib;

// Simple_window is basic scaffolding for ultra-simple interaction with graphics
// it provides one window with one "next" button for ultra-simple animation

struct Simple_window : Window {
	Simple_window(Point xy, int w, int h, const string& title)
		: Window{xy, w, h, title},
		  next_button{{x_max() - 70, 0}, 70, 20, "Next", [this] { next(); } }
	{
		attach(next_button);
	}
	
	void wait_for_button()
	// wait for next_button pressed
	// modified event loop, this allows graphics without control inversion
	{
		redraw();
		signal_ = std::make_unique<form>();
		nana::API::wait_for(*signal_);
	}

private:
	Button next_button;
	std::unique_ptr<form> signal_;
	void next() { signal_.reset(); }
};
