
#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

#include "Point.h"
#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>			// max
#include <stdexcept>
#include <cmath>

#include "nana.h"

using std::vector;
using std::string;

inline void error(const string& s) { throw std::runtime_error(s); }

namespace Graph_lib {
// defense against ill-behaved Linux macros:
#undef major
#undef minor

using Colors = nana::colors;
struct Color {
	enum Transparency { invisible = 0, visible = 255 };

	Color(int rr, int gg, int bb, int vv= visible) : r(rr), g(gg), b(bb), v(vv) { }
	Color(Colors cc, Transparency vv = visible) : r((int(cc)>>16) & 0xFF), g((int(cc)>>8) & 0xFF), b(int(cc) & 0xFF), v(vv) { }
	Color(Transparency vv) : r{0}, g{0}, b{0}, v(vv) { }

	operator nana::color() const { return { r, g, b, v / 255. }; }

	int red()   const { return r; }
	int green() const { return g; }
	int blue()  const { return b; }
	int as_int() const { return (r << 16) + (g << 8) + b; }
	char visibility() const { return v; }
	void set_visibility(Transparency vv) { v = vv; }

private:
	unsigned char r, g, b, v;
};

struct Line_style {
	enum Line_style_type {
		solid,				// -------
		dash,				// - - - -
		dot,				// ....... 
		dashdot,			// - . - . 
		dashdotdot			// -..-..
	};
	Line_style(Line_style_type ss) :s(ss), w(0) { }
	Line_style(Line_style_type lst, int ww) :s(lst), w(ww) { }
	Line_style(int ss) :s(ss), w(0) { }

	int width() const { return w; }
	int style() const { return s; }

private:
	int s;
	int w;
};

struct Font {
	Font(const string& name, unsigned size) : name_{name}, size_{size}, bold_{false}, italic_{false} { }

	Font(const string& name = "", bool bold = false, bool italic = false, unsigned size = 12)
		: name_{name}, size_{size}, bold_{bold}, italic_{italic}
	{ }

	operator nana::paint::font() const
	{ return {name_, double(size_), {bold_?800u:400u, italic_}}; }

	string   name() const { return name_; }
	unsigned size() const { return size_; }
	bool     bold() const { return bold_; }
	bool   italic() const { return italic_; }

private:
	string name_;
	unsigned size_;
	bool bold_, italic_;
};

template<class T> class Vector_ref {
	vector<T*> v;
	vector<T*> owned;
public:
	Vector_ref() {}

	Vector_ref(T* a, T* b=0, T* c=0, T* d=0)
	{
		if (a) push_back(a);
		if (b) push_back(b);
		if (c) push_back(c);
		if (d) push_back(d);
	}

	~Vector_ref() { for (int i=0; i<owned.size(); ++i) delete owned[i]; }

	void push_back(T& s) { v.push_back(&s); }
	void push_back(T* p) { v.push_back(p); owned.push_back(p); }

	// ???void erase(???)

	T& operator[](int i) { return *v[i]; }
	const T& operator[](int i) const { return *v[i]; }
	T& back() { return *v.back(); }
	const T& back() const { return *v.back(); }
	int size() const { return v.size(); }
};

typedef double Fct(double);

inline Size max(Size a, Size b) { return { std::max(a.width, b.width), std::max(a.height, b.height) }; }

class Shape  {	// deals with color and style, and holds sequence of lines
protected:
	Shape() { }
	Shape(std::initializer_list<Point> lst) { for (Point p : lst) add(p); }  // add() the Points to this Shape

	void add(Point p){ points.push_back(p); }
public:
	void draw(Graphics& g) const;					// deal with color and draw_lines
protected:
	void set_point(int i, Point p) { points[i] = p; }
	virtual void draw_lines(Graphics& g) const;	// simply draw the appropriate lines
public:
	virtual void move(int dx, int dy);	// move the shape +=dx and +=dy

	virtual Size dimmension() const;

	void set_color(Color col) { lcolor = col; }
	Color color() const { return lcolor; }

	void set_style(Line_style sty) { ls = sty; }
	Line_style style() const { return ls; }

	void set_fill_color(Color col) { fcolor = col; }
	Color fill_color() const { return fcolor; }

	Point point(int i) const { return points[i]; }
	int number_of_points() const { return int(points.size()); }

	virtual ~Shape() { }

	Shape(const Shape&) = delete;
	Shape& operator=(const Shape&) = delete;
private:
	vector<Point> points;	// not used by all shapes
	Color lcolor {Colors::black};
	Line_style ls {0};
	Color fcolor {Color::invisible};
};

struct Function : Shape {
	// the function parameters are not stored
	Function(Fct f, double r1, double r2, Point orig, int count = 100, double xscale = 25, double yscale = 25);
};
struct Polar_function : Shape {
	// the function parameters are not stored
	Polar_function(Fct f, Point orig, double t1 = 0, double t2 = 361, int count = 360, double xscale = 25, double yscale = 25);
};

struct Line : Shape {
	Line(Point p1, Point p2) { add(p1); add(p2); }
};

struct Rectangle : Shape {

	Rectangle(Point xy, int ww, int hh) :w{ ww }, h{ hh }
	{
		if (h<=0 || w<=0) error("Bad rectangle: non-positive side");
		add(xy);
	}
	Rectangle(Point x, Point y) :w{ y.x - x.x }, h{ y.y - x.y }
	{
		if (h<=0 || w<=0) error("Bad rectangle: first point is not top left");
		add(x);
	}
	void draw_lines(Graphics& g) const;
	Size dimmension() const { return Shape::dimmension() + Size(w, h); }

	int height() const { return h; }
	int width() const { return w; }
private:
	int h;			// height
	int w;			// width
};

struct Open_polyline : Shape {	// open sequence of lines
	Open_polyline() {}
	Open_polyline(std::initializer_list<Point> lst) : Shape{lst} {}
	using Shape::add;
	using Shape::set_point;
};

struct Closed_polyline : Open_polyline {	// closed sequence of lines
	using Open_polyline::Open_polyline;
	void draw_lines(Graphics& g) const;
};


struct Polygon : Closed_polyline {	// closed sequence of non-intersecting lines
	using Closed_polyline::Closed_polyline;
	void add(Point p);
	void draw_lines(Graphics& g) const;
};

struct Lines : Shape {	// indepentdent lines
	Lines() {}
	Lines(std::initializer_list<Point> lst) : Shape{lst} { if (lst.size() % 2) error("odd number of points for Lines"); }
	void draw_lines(Graphics& g) const;
	void add(Point p1, Point p2) { Shape::add(p1); Shape::add(p2); }
};

struct Text : Shape {
	// the point is the bottom left of the first letter
	Text(Point x, const string& s) : lab{s} { add(x); }

	void draw_lines(Graphics& g) const;
	Size dimmension() const;

	void set_label(const string& s) { lab = s; }
	string label() const { return lab; }

	void set_font(Font f) { fnt = f; }
	Font font() const { return fnt; }

private:
	string lab;	// label
	Font fnt;
};


struct Axis : Shape {
	// representation left public
	enum Orientation { x, y, z };
	Axis(Orientation d, Point xy, int length, int nummber_of_notches=0, string label = "");

	void draw_lines(Graphics& g) const;
	void move(int dx, int dy);
	Size dimmension() const;

	void set_color(Color c);

	Text label;
	Lines notches;
//	Orientation orin;
//	int notches;
};

struct Ellipse : Polar_function {
	Ellipse(Point p, int ww, int hh)	// center, min, and max distance from center
		: Polar_function{[](double){return 1.;}, p, 0, 360, 360, ww, hh}, c{p}, w{ww}, h{hh}
	{}

//	void draw_lines(Graphics& g) const;

	Point center() const { return c; }
	Point focus1() const { return { c.x + int(sqrt(double(w*w - h*h))), c.y }; }
	Point focus2() const { return { c.x - int(sqrt(double(w*w - h*h))), c.y }; }

//	void set_major(int ww) { w = ww; }
	int major() const { return w; }
//	void set_minor(int hh) { h = hh; }
	int minor() const { return h; }

protected:
	Point c;
	int w;
	int h;
};

struct Circle : Ellipse {
	Circle(Point p, int rr)	// center and radius
	: Ellipse{p, rr, rr} { }

//	void draw_lines(Graphics& g) const;

//	void set_radius(int rr) { r=rr; }
	int radius() const { return w; }
};

/*
struct Mark : Text {
	static const int dw = 4;
	static const int dh = 4;
	Mark(Point xy, char c) : Text(Point(xy.x-dw, xy.y+dh),string(1,c)) {}
};
*/

struct Marked_polyline : Open_polyline {
	Marked_polyline(const string& m, std::initializer_list<Point> lst = {})
		: Open_polyline{lst}, mark{m}
	{ if (m.empty()) mark = "*"; }
	void draw_lines(Graphics& g) const;
	Size dimmension() const { return Shape::dimmension() + Size{ 12, 12 }; }
private:
	string mark;
};

struct Marks : Marked_polyline {
	Marks(const string& m, std::initializer_list<Point> lst = {})
		: Marked_polyline(m, lst)
	{ set_color(Color::invisible); }
};

struct Mark : Marks {
	Mark(Point xy, char c) : Marks(string(1,c)) { add(xy); }
};

/*
struct Marks : Shape {
	Marks(char m) : mark(string(1,m)) { }
	void add(Point p) { Shape::add(p); }
	void draw_lines() const;
private:
	string mark;
};
*/

struct Image : Shape {
	Image(Point xy = {0,0}, const string& file = "") :w{0}, h{0}, img{file}  { add(xy); }

	void open(const string& file) { img.open(file); }
	void open(const void *data, std::size_t bytes) { img.open(data, bytes); }

	Size dimmension() const { return Shape::dimmension() + img.size(); }
	void draw_lines(Graphics& g) const;
	void set_mask(Point xy, int ww, int hh) { w=ww; h=hh; cx=xy.x; cy=xy.y; }
//	void move(int dx,int dy) { Shape::move(dx,dy); p->draw(point(0).x,point(0).y); }
private:
	int w,h,cx,cy; // define "masking box" within image relative to position (cx,cy)
	nana::paint::image img;
};

}
#endif

