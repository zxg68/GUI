#include <iostream>
#include <fstream>
#include <complex>
#include <cstdio>
#include <omp.h>
#include "GUI.h"
#include "window.h"

using namespace Graph_lib;
using namespace std;

using cmplx = std::complex<double>;
int Mandelbrot(cmplx c, int mi = 1) {
	cmplx z;
	for (int k = 255 * mi - 1; k >= 0; --k) {
		z = z * z + c;
		if (std::norm(z) > 4) return k / mi;
	}
	return 0;
}

struct Bmp256 {
	static constexpr int OffPalette = 54, OffBits = 54 + 256 * 4;
	
#pragma pack(2)
	struct Header {
		uint16_t bfType = 0x4D42;
		uint32_t bfSize;
		uint16_t bfReserved1 = 0;
		uint16_t bfReserved2 = 0;
		uint32_t bfOffBits = OffBits;
		uint32_t biSize = 40;
		int32_t  biWidth;
		int32_t  biHeight;
		uint16_t biPlanes = 1;
		uint16_t biBitCount = 8;
		uint32_t biCompression = 0;
		uint32_t biSizeImage = 0;
		int32_t  biXPelsPerMeter = 0;
		int32_t  biYPelsPerMeter = 0;
		uint32_t biClrUsed = 256;
		uint32_t biClrImportant = 256;
	} ;
#pragma pack()

	struct PColor {
		uint8_t B, G, R, A;
		void set(uint8_t r, uint8_t g, uint8_t b) { R = r; G = g; B = b; A = 0; }
	};

	Bmp256() = default;
	Bmp256(int width, int height) { reset(width, height); }
	~Bmp256() { delete[] buffer; }
	void reset(int width, int height);
	int width()  const { return header->biWidth; }
	int height() const { return header->biHeight; }
	int size()   const { return header->bfSize; }
	void set_palette(int i, uint8_t r, uint8_t g, uint8_t b) { palette[i].set(r, g, b); }
	uint8_t& pixel(int row, int col) { return data[row * row_size + col]; }	// get/set the pixel
	void save(const char* file);

	int     row_size;
	uint8_t *buffer = nullptr;
	Header  *header;
	PColor  *palette;
	uint8_t *data;
};

void Bmp256::reset(int width, int height) {
	row_size = ((width + 3) / 4) * 4;
	int data_size = row_size * height;
	int buff_size = OffBits + data_size;

	delete[] buffer;
	buffer = new uint8_t[buff_size];
	header = new(buffer) Header;
	palette = (PColor*)(buffer + OffPalette);
	data = buffer + OffBits;

	header->biWidth = width;
	header->biHeight = height;
	header->bfSize = buff_size;
}

void Bmp256::save(const char* file_name) {
std::ofstream of{file_name, std::ios::binary};
	of.write((char *)buffer, header->bfSize);
}

struct Fractal : Bmp256 {
	Fractal() = default;
	Fractal(double cx, double cy, double sc, int w, int h)
	{ reset(cx, cy, sc, w, h); }

	void reset(double cx, double cy, double sc, int w, int h);
	
	void calc_image();

	void move(int dx, int dy) { ox += dx * scale; oy += dy * scale; }
	void zoom(double s, Point p);
	void resize(int w, int h);

	double ox, oy, scale;

private:
	void calc_palette();
};

void Fractal::reset(double cx, double cy, double sc, int w, int h)
{
	Bmp256::reset(w, h);
	ox = cx - sc * w / 2;
	oy = cy - sc * h / 2;
	scale = sc;
	calc_palette();
	calc_image();
}

void Fractal::calc_palette() {
	for (int i = 0; i < 64; ++i) {
		set_palette(i, 255, 255 - i * 4, 0);
		set_palette(i + 64, 255 - i * 2, 0, i * 2);
		set_palette(i + 128, 127 - i * 2, 0, 128 + i * 2);
		set_palette(i + 192, 0, 0, 255 - i * 3);
	}
	set_palette(0, 0, 0, 0);
}

void Fractal::calc_image() {
#pragma omp parallel for schedule(static, 1)
	for (int j = 0; j < height(); ++j)
		for (int i = 0; i < width(); ++i)
			pixel(j, i) = Mandelbrot(cmplx(ox + i * scale, oy + j * scale));
}

void Fractal::zoom(double s, Point p) {
	double px = ox + scale * p.x;
	double py = oy + scale * p.y;
	scale *= s;
	ox = px - scale * p.x;
	oy = py - scale * p.y;
}

void Fractal::resize(int w, int h) {
	double cx = ox + scale * width() / 2;
	double cy = oy + scale * height() / 2;
	Bmp256::reset(w, h);
	ox = cx - scale * w / 2;
	oy = cy - scale * h / 2;
	calc_palette();
	calc_image();
}

struct Fractal_window : Window {
	Fractal_window(Point xy, int w, int h, const string& title);
	
private:
	Button quit_button;
	Out_box center_out;
	Out_box scale_out;
	Canvas canvas;
	Image img;
	Fractal img_src;
	Point move_start;

	void mouse_cb(const Mouse_event& evt);
	void size_cb(const Size_event& evt);
	void out_pos();
};

Fractal_window::Fractal_window(Point xy, int w, int h, const string& title)
	: Window{ xy, w, h, title },
	quit_button{ "Quit", Exit },
	center_out{ "center:" },
	scale_out{ "scale:" }
{
	div("vert<weight=20<><out arrange=[180,120] gap=20><><quit weight=70>><draw>");
	place("quit", quit_button);
	place("out", center_out);
	place("out", scale_out);
	place("draw", canvas);
	collocate();
	canvas.attach(img);
	img_src.reset(-0.5, 0.0, 1./300, canvas.width(), canvas.height());
	img.open(img_src.buffer, img_src.size());
	canvas.set_mouse_callback([this](const Mouse_event& evt) { mouse_cb(evt); });
	canvas.set_size_callback([this](const Size_event& evt) { size_cb(evt); });
	out_pos();
}


void Fractal_window::mouse_cb(const Mouse_event& evt)
{
	Point pos = {evt.pos.x, img_src.height() - evt.pos.y - 1};
	if (evt.type == Mouse_event::push)
		move_start = pos;
	else if (evt.type == Mouse_event::drag) {
		img_src.move(move_start.x - pos.x, move_start.y - pos.y);
		move_start = pos;
	}
	else if (evt.type == Mouse_event::wheel) {
		img_src.zoom(evt.distance > 0 ? .5 : 2, pos);
	}
	else return;

	img_src.calc_image();
	img.open(img_src.buffer, img_src.size());
	out_pos();
	redraw();
}

void Fractal_window::size_cb(const Size_event& evt) {
	img_src.resize(evt.width, evt.height);
	img.open(img_src.buffer, img_src.size());
	redraw();
}

void Fractal_window::out_pos()
{
	char str[80];
	double sc = img_src.scale;
	double cx = img_src.ox + sc * img_src.width() / 2;
	double cy = img_src.oy + sc * img_src.height() / 2;
	sprintf(str, "%f, %f", cx, cy);
	center_out.put(str);
	sprintf(str, "%g", sc);
	scale_out.put(str);
}

int main()
{
	Fractal_window win = { { 100,100 }, 900, 620, "Mandelbrot Explorer" };
	gui_main(win);
}
