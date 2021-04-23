#include "GUI.h"
#include <sstream>
#include <ctype.h>

using namespace Graph_lib;
using namespace std;

int main()
{
	auto win = Window{ 240, 120, "Test Animation" };
	auto ani = Animation{ true, 5 };
	auto play = Button{ "Play", [&](Button& b) {
			if (b.label() == "Play")  { ani.play(); b.set_label("Pause"); }
			else { ani.pause(); b.set_label("Play"); }
			win.redraw();
		}
	};
	win.div("<a margin=20>");
	win.place("a", ani);
	win.place("a", play);
	win.collocate();
	auto tri = Polygon{ {10, 50}, {20, 30}, {30, 50} };
	auto rec = Rectangle{ {10, 30}, {30,50} };
	auto cir = Circle{ {20, 40}, 10 };
	ani.push_back({ &tri, &rec, &cir });
//	ani.push_back({ "a_pic0.bmp", "a_pic1.bmp" , "a_pic2.bmp" });
	gui_main(win);
}
