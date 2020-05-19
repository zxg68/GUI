
// Drawing object shared by Window and Canvas
// by Zhou XueGong

#ifndef DRAWING_GUARD
#define DRAWING_GUARD

#include "nana.h"
#include "Point.h"

#include <vector>
#include <functional>
using std::vector;

namespace Graph_lib {
	struct Mouse_event {
		enum Event_type { dbl_click, push, drag, release, move, wheel };
		enum Buttons {					// values for button
			any =    int(nana::mouse::any_button),
			left = int(nana::mouse::left_button),
			middle = int(nana::mouse::middle_button),
			right = int(nana::mouse::right_button)
		};
		enum Wheels {
			vertical   = int(nana::arg_wheel::wheel::vertical),
			horizontal = int(nana::arg_wheel::wheel::horizontal)
		};
		enum States {					// masks for state
			s_left = 0x01, s_middle = 0x02, s_right = 0x04, s_buttons = 0x07,
			s_alt  = 0x10, s_shift  = 0x20, s_ctrl  = 0x40
		};

		Event_type type;
		Point pos;
		Buttons button;
		int state;
		
		Wheels which = vertical;
		int distance = 0;
		
		Mouse_event(const nana::arg_mouse& ea);
	};

	using Keyboard_event = nana::arg_keyboard;
	using Size_event = nana::arg_resized;
	using nana::event_code;

	class Shape;

	class Drawing : nana::drawing {
	public:
		Drawing(nana::widget& w) : nana::drawing{w}, own{w} {}

		void attach(Shape& s);
		void detach(Shape& s);		// remove s from shapes 
		void put_on_top(Shape& p);	// put p on top of other shapes
		void redraw();

		using Keyboard_cb = std::function<void(const Keyboard_event& e)>;
		using Mouse_cb    = std::function<void(const Mouse_event& e)>;
		using Size_cb     = std::function<void(const Size_event&  e)>;
		void set_keyboard_callback(Keyboard_cb cb);
		void set_mouse_callback(Mouse_cb cb);
		void set_size_callback(Size_cb cb);

	private:
		nana::widget& own;
		vector<Shape*> shapes;		// shapes attached to window
	};

} // of namespace Graph_lib

#endif // DRAWING_GUARD
