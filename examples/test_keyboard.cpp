#include "GUI.h"
#include <sstream>
#include <ctype.h>

using namespace Graph_lib;
using namespace std;

int main()
{
	Window win = { 400, 120, "Test Keyboard Event" };
	win.div("vert<a margin=20 gap=20><b margin=20>");
	Out_box type = {"Type"}, key = {"Key"};
	Label alt = {"Alt"}, ctrl = {"Ctrl"}, shift = {"Shift"};
	win.place("a", type);
	win.place("a", key);
	win.place("b", alt);
	win.place("b", ctrl);
	win.place("b", shift);
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
