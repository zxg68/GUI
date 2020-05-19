
//
// This is example code from Chapter 15.2 "Graphing simple functions" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities

//------------------------------------------------------------------------------

double one(double) { return 1; }
double slope(double x) { return x/2; }
double square(double x) { return x*x; }

//------------------------------------------------------------------------------

int main()
{
    const int xmax = 600;    // window size
    const int ymax = 400;

    const int x_orig = xmax/2;    // position of (0,0) is center of window
    const int y_orig = ymax/2; 
    const Point orig = {x_orig,y_orig};

    const double r_min = -10;    // range [-10:11) 
    const double r_max = 11;

    const int n_points = 400;    // number of points used in range

    const double x_scale = 30;    // scaling factors
    const double y_scale = 30;

    Simple_window win0 = {{100,100},xmax,ymax,"Function graphing"};

    Function s1 = {one,   r_min,r_max,orig,n_points,x_scale,y_scale};
    Function s2 = {slope, r_min,r_max,orig,n_points,x_scale,y_scale};
    Function s3 = {square,r_min,r_max,orig,n_points,x_scale,y_scale};

    win0.attach(s1);
    win0.attach(s2);
    win0.attach(s3);
    win0.wait_for_button();

    Text ts  = {{100,y_orig-50},"one"};
    Text ts2 = {{100,y_orig+y_orig/2-30},"x/2"};
    Text ts3 = {{x_orig-100,30},"x*x"};
    win0.attach(ts);
    win0.attach(ts2);
    win0.attach(ts3);
    win0.set_label("Function graphing: label functions");
    win0.wait_for_button();

    const int xlength = xmax-40;   // make the axis a bit smaller than the window
    const int ylength = ymax-40;

    Axis x = {Axis::x, {20,y_orig}, xlength, xlength/(int)x_scale, "one notch == 1"};
    Axis y = {Axis::y, {x_orig, ylength+20}, ylength, ylength/(int)y_scale, "one notch == 1"};
    win0.attach(x);
    win0.attach(y);
    x.set_color(Colors::red);
    y.set_color(Colors::red);
    win0.wait_for_button();
}

//------------------------------------------------------------------------------
