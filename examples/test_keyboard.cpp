#include "GUI.h"
#include <sstream>
#include <ctype.h>

using namespace Graph_lib;
using namespace std;

int main()
{
	auto win = Window{ 400, 120, "Test Keyboard Event" };
	auto type = Out_box{"Type"}, key = Out_box{"Key"};
	auto alt = Label{"Alt"}, ctrl = Label{"Ctrl"}, shift = Label{"Shift"};
	auto btn = Button{ "", [](Button& b) { b.set_label(b.label() + "!"); } };
	win.div("vert<a margin=20 gap=20><b margin=20>");
	win.place("a", type);
	win.place("a", key);
	win.place("b", alt);
	win.place("b", ctrl);
	win.place("b", shift);
	win.place("b", btn);
	win.collocate();
	win.set_keyboard_callback([&](const Keyboard_event& evt) {
		switch (evt.evt_code) {
		case event_code::key_press: type.put("press"); break;
		case event_code::key_release: type.put("release"); break;
		default: type.put(int(evt.evt_code)); break;
		};
		ostringstream ks;
		ks << "0x" << hex << unsigned(evt.key);
		if (isprint(evt.key)) ks << ' ' << char(evt.key);
		key.put(ks.str());
		if (evt.alt) alt.show(); else alt.hide();
		if (evt.ctrl) ctrl.show(); else ctrl.hide();
		if (evt.shift) shift.show(); else shift.hide();
	});
	gui_main(win);
}
