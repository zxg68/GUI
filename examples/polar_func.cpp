#include <math.h>
#include "Simple_window.h"
#include "Graph.h"

constexpr double a = 5, k = 3;
double heart(double t) { return a * (1+cos(t)); }
double lemniscate(double t) { return a * sqrt(fmax(cos(2*t), 0)); }
double rose(double t) { return a * cos(k*t); }

int main() {
	const int xmax = 600;
	const int ymax = 400;//窗口大小
	const int x_orig = xmax / 2;
	const int y_orig = ymax / 2;
	const Point orig{ x_orig,y_orig };//原点定在窗口中心
	Simple_window win = {{100,100},xmax,ymax,"Polar Functions"};

	Polar_function s1 = {heart, orig};
	Text ts1 = {orig + Point{200, 0}, "heart"};
	s1.set_color(Colors::red);
	ts1.set_color(Colors::red);
	win.attach(s1);
	win.attach(ts1);
	win.wait_for_button();

	Polar_function s2 = {lemniscate, orig};
	Text ts2 = {orig - Point{220, 0}, "lemniscate"};
	s2.set_color(Colors::green);
	ts2.set_color(Colors::green);
	win.attach(s2);
	win.attach(ts2);
	win.wait_for_button();

	Polar_function s3 = {rose, orig};
	Text ts3 = {orig - Point{120, 120}, "rose"};
	s3.set_color(Colors::blue);
	ts3.set_color(Colors::blue);
	win.attach(s3);
	win.attach(ts3);
	win.wait_for_button();
}
