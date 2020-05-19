#include "window.h"
#include "GUI.h"

using namespace Graph_lib;

int main()
{
	Window win = {400, 200, "Test Label"};
	Label lbl = {{20, 20}, 360, 30, "Normal <bold size=20>Bold 20</> <red font=Consolas size=14>Red Consolas 14</>"};
	win.attach(lbl);
	gui_main(win);
}
