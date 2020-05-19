
// Drawing object shared by Window and Canvas
// by Zhou XueGong

#include "Drawing.h"
#include "Graph.h"

namespace Graph_lib {

	void Drawing::redraw()
	{
		clear();
		nana::drawing::draw([this](Graphics& g)
			{ for (auto s : shapes) s->draw(g); });
		update();
	}

	void Drawing::attach(Shape& s)
	{
		shapes.push_back(&s);
	}

	void Drawing::detach(Shape& s)
	{
		for (unsigned int i = shapes.size(); 0 < i; --i)	// guess last attached will be first released
			if (shapes[i - 1] == &s)
				shapes.erase(shapes.begin() + (i - 1));//&shapes[i-1]);
	}

	void Drawing::put_on_top(Shape& p) {
		for (int i = 0; i < shapes.size(); ++i) {
			if (&p == shapes[i]) {
				for (++i; i < shapes.size(); ++i)
					shapes[i - 1] = shapes[i];
				shapes[shapes.size() - 1] = &p;
				return;
			}
		}
	}


	constexpr int types[] = {-1, Mouse_event::dbl_click, -1, Mouse_event::move, -1, Mouse_event::push, Mouse_event::release, Mouse_event::wheel};

	Mouse_event::Mouse_event(const nana::arg_mouse& ea)
		: type{(Event_type)types[int(ea.evt_code)]}, pos{ea.pos.x, ea.pos.y}, button{(Buttons)ea.button},
		  state(ea.left_button*s_left + ea.mid_button*s_middle + ea.right_button*s_right
		  	    + ea.alt*s_alt + ea.shift*s_shift + ea.ctrl*s_ctrl)
	{
		if (type == wheel) {
			auto wea = static_cast<const nana::arg_wheel&>(ea);
			which = Wheels(wea.which);
			distance = wea.upwards ? wea.distance : -int(wea.distance);
		}
		else if (type == move && (state & s_buttons))
			type = drag;
	}

	void Drawing::set_mouse_callback(Mouse_cb cb)
	{
		auto mouse_handler = [cb](const nana::arg_mouse& ea) { cb({ea}); };
		own.events().dbl_click (mouse_handler);
		own.events().mouse_up  (mouse_handler);
		own.events().mouse_down(mouse_handler);
		own.events().mouse_move(mouse_handler);
		own.events().mouse_wheel(mouse_handler);
	}

	void Drawing::set_size_callback(Size_cb cb)
	{
		auto size_handler = [cb](const nana::arg_resized& ea) { cb(ea); };
		own.events().resized(size_handler);
	}

}
