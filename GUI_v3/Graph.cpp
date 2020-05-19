#include "Graph.h"
#include <utility>
using std::pair;

#define M_PI       3.14159265358979323846

namespace Graph_lib {

void Shape::draw_lines(Graphics& g) const
{
	if (color().visibility() && 1<points.size())	// draw sole pixel?
		for (unsigned int i=1; i<points.size(); ++i)
			g.line(points[i-1], points[i]);
}

void Shape::draw(Graphics& g) const
{
	auto oldc = g.palette(false);
	// same color for line and text
	g.palette(false, lcolor);
	g.palette(true, lcolor);
	draw_lines(g);
	g.palette(false, oldc);	// reset color (to pevious)
	g.palette(true, oldc);
}


// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel) 
{
    double x1 = p1.x;
    double x2 = p2.x;
	double x3 = p3.x;
	double x4 = p4.x;
	double y1 = p1.y;
	double y2 = p2.y;
	double y3 = p3.y;
	double y4 = p4.y;

	double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
	if (denom == 0){
		parallel= true;
		return pair<double,double>(0,0);
	}
	parallel = false;
	return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
								((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}


//intersection between two line segments
//Returns true if the two segments intersect,
//in which case intersection is set to the point of intersection
bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection){
   bool parallel;
   pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);
   if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
   intersection.x = p1.x + u.first*(p2.x - p1.x);
   intersection.y = p1.y + u.first*(p2.y - p1.y);
   return true;
} 

void Polygon::add(Point p)
{
	int np = number_of_points();

	if (1<np) {	// check that thenew line isn't parallel to the previous one
		if (p==point(np-1)) error("polygon point equal to previous point");
		bool parallel;
		line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
		if (parallel)
			error("two polygon points lie in a straight line");
	}

	for (int i = 1; i<np-1; ++i) {	// check that new segment doesn't interset and old point
		Point ignore{0, 0};
		if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
			error("intersect in polygon");
	}
	

	Closed_polyline::add(p);
}


void Polygon::draw_lines(Graphics& g) const
{
		if (number_of_points() < 3) error("less than 3 points in a Polygon");
		Closed_polyline::draw_lines(g);
}

void Closed_polyline::draw_lines(Graphics& g) const
{
	Open_polyline::draw_lines(g);
		
	if (2 < number_of_points() && color().visibility())	// draw closing line:
		g.line(point(number_of_points()-1), point(0));
}
void Shape::move(int dx, int dy)
{
	for (unsigned int i = 0; i<points.size(); ++i) {
		points[i].x+=dx;
		points[i].y+=dy;
	}
}

void Lines::draw_lines(Graphics& g) const
{
//	if (number_of_points()%2==1) error("odd number of points in set of lines");
	if (color().visibility())
		for (int i=1; i<number_of_points(); i+=2)
			g.line(point(i-1), point(i));
}

void Text::draw_lines(Graphics& g) const
{
	auto ofnt = g.typeface();
	g.typeface(fnt);
	g.string(point(0), lab);
	g.typeface(ofnt);
}

Function::Function(Fct f, double r1, double r2, Point xy, int count, double xscale, double yscale)
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
	if (r2-r1<=0) error("bad graphing range");
	if (count<=0) error("non-positive graphing count");
	double dist = (r2-r1)/count;
	double r = r1;
	for (int i = 0; i<count; ++i) {
		add(Point{xy.x+int(r*xscale),xy.y-int(f(r)*yscale)});
		r += dist;
	}
}

Polar_function::Polar_function(Fct f, Point orig, double t1, double t2, int count, double xscale, double yscale)
{
	if (t2 - t1 <= 0) error("bad graphing range");
	if (count <= 0) error("non-positive graphing count");
	auto dist = (t2 - t1) * 2 * M_PI / (count * 360);
	auto theta = t1 * 2 * M_PI / 360;
	for (auto i = 0; i<count; ++i) {
		add(Point{orig.x + int(f(theta)*std::cos(theta)*xscale), orig.y - int(f(theta)*std::sin(theta)*yscale)});
		theta += dist;
	}
}

void Rectangle::draw_lines(Graphics& g) const
{
	if (fill_color().visibility()) {	// fill
		g.rectangle(nana::rectangle(point(0).x, point(0).y, w, h), true, fill_color());
		g.palette(false, color());	// reset color
	}

	if (color().visibility()) 	// edge on top of fill
		g.rectangle(nana::rectangle(point(0).x, point(0).y, w, h), false, color());
}


Axis::Axis(Orientation d, Point xy, int length, int n, string lab)
	:label(Point{0,0},lab)
{
	if (length<0) error("bad axis length");
	switch (d){
	case Axis::x:
		{	Shape::add(xy);	// axis line
			Shape::add(Point{xy.x+length,xy.y});	// axis line
			if (1<n) {
				int dist = length/n;
				int x = xy.x+dist;
				for (int i = 0; i<n; ++i) {
					notches.add(Point{x,xy.y},Point{x,xy.y-5});
				x += dist;
			}
		}
		// label under the line
		label.move(length/3,xy.y/*+10*/);
		break;
	}
	case Axis::y:
		{	Shape::add(xy);	// a y-axis goes up
			Shape::add(Point{xy.x,xy.y-length});
			if (1<n) {
			int dist = length/n;
			int y = xy.y-dist;
			for (int i = 0; i<n; ++i) {
				notches.add(Point{xy.x,y},Point{xy.x+5,y});
				y -= dist;
			}
		}
		// label at top
		label.move(xy.x-10,xy.y-length-20);
		break;
	}
	case Axis::z:
		error("z axis not implemented");
	}
}

void Axis::draw_lines(Graphics& g) const
{
	Shape::draw_lines(g);	// the line
	notches.draw(g);	// the notches may have a different color from the line
	label.draw(g);	// the label may have a different color from the line
}


void Axis::set_color(Color c)
{
	Shape::set_color(c);
	notches.set_color(c);
	label.set_color(c);
}

void Axis::move(int dx, int dy)
{
	Shape::move(dx,dy);
	notches.move(dx,dy);
	label.move(dx,dy);
}

void draw_mark(Graphics& g, Point xy, char c)
{
	static const int dx = 4;
	static const int dy = 8;
	string m(1,c);
	g.string(nana::point(xy.x - dx, xy.y - dy), m);
}

void Marked_polyline::draw_lines(Graphics& g) const
{
	Open_polyline::draw_lines(g);
	for (int i=0; i<number_of_points(); ++i) 
		draw_mark(g, point(i), mark[i%mark.size()]);
}


void Image::draw_lines(Graphics& g) const
{
	if (w && h)
		img.paste(nana::rectangle(cx,cy,w,h), g, point(0));
	else
		img.paste(g, point(0));
}

} // Graph
