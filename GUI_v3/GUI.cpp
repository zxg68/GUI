#include "GUI.h"
//#include "std_lib_facilities.h"

using namespace Graph_lib;

nana::widget* Label::create_nana_widget(nana::widget& own)
{
	auto lbl = new nana::label{own, area};
	lbl->format(fmt_ == Format::enable);
	lbl->text_align(align_);
	lbl->caption(label());
	return pw = lbl;
}

void Label::set(const string& s)
{
	reference_to<nana::label>(pw).caption(s);
}

//------------------------------------------------------------------------------

nana::widget* Button::create_nana_widget(nana::widget& own)
{
	auto btn = new nana::button{own, area};
	btn->caption(label());
	btn->events().click([this]() { cb_(*this); });
	return pw = btn;
}

//------------------------------------------------------------------------------

struct IO_box : nana::panel<false> {
	nana::place plc_;
	nana::label lbl_;
	nana::textbox txt_;
	IO_box(nana::widget& own, const nana::rectangle& r, const string & lbl, bool edt = true)
		: panel<false>{own, r},
		plc_{*this}, lbl_{*this, lbl}, txt_{*this}
	{
		txt_.editable(edt);
		txt_.multi_lines(false);
		plc_.div("<fit gap=5 all>");
		plc_["all"] << lbl_ << txt_;
		plc_.collocate();
	}
};

nana::widget* In_box::create_nana_widget(nana::widget& own)
{
	return pw = new IO_box{own, area, label()};
}

int In_box::get_int()
{
	auto& pi = reference_to<IO_box>(pw).txt_;
	std::string s;
	if (!pi.getline(0, s) || !isdigit(s[0])) return -999999;
	return std::stoi(s);
}

string In_box::get_string()
{
	auto& pi = reference_to<IO_box>(pw).txt_;
	std::string s;
	pi.getline(0, s);
	return s;
}


nana::widget* Out_box::create_nana_widget(nana::widget& own)
{
	return pw = new IO_box{own, area, label(), false};
}

void Out_box::put(int i)
{
	reference_to<IO_box>(pw).txt_.reset(std::to_string(i));
}

void Out_box::put(const string& s)
{
	reference_to<IO_box>(pw).txt_.reset(s);
}

//------------------------------------------------------------------------------

nana::widget* Menu::create_nana_widget(nana::widget& own) {  // attach all buttons
	static const string sk[] = { "horizontal", "vertical" };
	string div = "<margin=[0,5] " + sk[k] + " buttons>";
	auto pnl = new nana::panel<false>{own, area};
	pnl->caption(label());
	nana::place plc{*pnl};
	plc.div(div);
	for (int i = 0; i < selection.size(); ++i) {
		auto& btn = *selection[i].create_nana_widget(*pnl);
		plc["buttons"] << btn;
	}
	plc.collocate();
	return pw = pnl;
}

//------------------------------------------------------------------------------

struct canvas_ : nana::panel<true> {	// the bool template parameter must be 'true' to support drawing
	Drawing dw;
	canvas_(nana::widget& own, const nana::rectangle& r)
		: panel<true>{own, r},
		dw{*this}
	{}
};

nana::widget* Canvas::create_nana_widget(nana::widget& own)
{
	auto cnvs = new canvas_{own, area};
	dynamic_cast<Window&>(own).canvases.push_back(this);
	return pw = cnvs;
}

void Canvas::attach(Shape& s)
{
	reference_to<canvas_>(pw).dw.attach(s);
}
void Canvas::detach(Shape& s)
{
	reference_to<canvas_>(pw).dw.detach(s);
}
void Canvas::put_on_top(Shape& s)
{
	reference_to<canvas_>(pw).dw.put_on_top(s);
}
void Canvas::redraw()
{
	reference_to<canvas_>(pw).dw.redraw();
}
void Canvas::set_mouse_callback(Drawing::Mouse_cb cb)
{
	reference_to<canvas_>(pw).dw.set_mouse_callback(cb);
}
void Canvas::set_size_callback(Drawing::Size_cb cb)
{
	reference_to<canvas_>(pw).dw.set_size_callback(cb);
}

//------------------------------------------------------------------------------

struct amimation_ : nana::panel<true> {
	nana::animation ani;
	amimation_(nana::widget& own, const nana::rectangle& r, const Animation& self)
		: panel<true>{own, r}, ani{self.fps()}
	{
		ani.looped(self.is_looped());
		ani.output(*this, {0,0});
	}
};

nana::widget* Animation::create_nana_widget(nana::widget& own)
{
	return pw = new amimation_{own, area, *this};
}

void Animation::play()
{
	reference_to<amimation_>(pw).ani.play();
}
void Animation::pause()
{
	reference_to<amimation_>(pw).ani.pause();
}

void Animation::push_back(const std::vector<string>& img_files)
{
	nana::frameset fset;
	for (const auto& img : img_files)
		fset.push_back(nana::paint::image{img});
	reference_to<amimation_>(pw).ani.push_back(fset);
}

void Animation::push_back(const std::vector<const Shape*>& shapes)
{
	Size dim;
	for (const auto sp : shapes)
		dim = max(dim, sp->dimmension());
	nana::frameset fset;
	auto bgcolor = nana::API::bgcolor(reference_to<amimation_>(pw));
	fset.push_back([shapes, bgcolor, dim](std::size_t pos, Graphics& fg, Size& fg_dim) {
						if (pos >= shapes.size()) return false;
						fg_dim = dim;
						fg.make(dim);
						fg.rectangle(true, bgcolor);
						shapes[pos]->draw(fg);
						return true;
					},
		shapes.size());
	reference_to<amimation_>(pw).ani.push_back(fset);
}
