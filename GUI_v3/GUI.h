
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef GUI_GUARD
#define GUI_GUARD

#include "Window.h"
#include "Graph.h"

#include <functional>		// std::function

namespace Graph_lib {

//------------------------------------------------------------------------------

	// treat an address as a reference to a W
	template<class W> W& reference_to(void* pw) { return *static_cast<W*>(pw); }

//------------------------------------------------------------------------------

	class Widget {
	// Widget is a handle to a nana::widget - it is *not* a nana::widget
	// We try to keep our interface classes at arm's length from nana
	public:
		Widget(Point xy, int w, int h, const string& s)
			: area(xy.x, xy.y, w, h), lbl{s}
		{}
		Widget(const string& s) : lbl{s} {}
		virtual ~Widget() { delete pw; }

		Widget& operator=(const Widget&) = delete; // don't copy Widgets
		Widget(const Widget&) = delete;

		const string& label() const { return lbl; }
		Point loc()   const { return pw->pos(); }
		int width()   const { return pw->size().width; }
		int height()  const { return pw->size().height; }

//		void move(int dx, int dy) { pw->move(pw->pos() + Point{dx, dy}); }
		void hide() { pw->hide(); }
		void show() { pw->show(); }
		virtual void set_label(const string& s) { pw->caption(lbl = s); }
		virtual void set_fgcolor(Color cc) { pw->fgcolor(cc); }
		virtual void set_bgcolor(Color cc) { pw->bgcolor(cc); }
		virtual void set_font(Font fnt)  { pw->typeface(fnt); }
		void attach(Window& w) { if (pw) show(); else create_nana_widget(w); }

	protected:
		friend void Window::place(const char* fld, Widget& w);	// access pw
//		Window* own = nullptr;    // every Widget belongs to a Window
		nana::widget* pw = nullptr;  // connection to the Nana Widget
		nana::rectangle area;		// initial area before placed
		virtual nana::widget* create_nana_widget(nana::widget& own) = 0;

	private:
		string lbl;
	};

//------------------------------------------------------------------------------
	enum class Format { disable, enable };
	using Align = nana::align;

	struct Label : Widget {
		Label(Point xy, int w, int h, const string& s, Format fm = Format::enable, Align al = Align::left)
			: Widget{xy, w, h, s}, fmt_{fm}, align_{al} { }
		Label(const string& s, Format fm = Format::enable, Align al = Align::left)
			: Widget{s}, fmt_{fm}, align_{al} { }

		void set(const string&);

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;
		Format fmt_;
		Align align_;
	};

//------------------------------------------------------------------------------

	struct In_box : Widget {
		In_box(Point xy, int w, int h, const string& s)
			: Widget(xy, w, h, s) { }
		In_box(const string& s) : Widget{s} { }

		int get_int();
		string get_string();

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;
	};

//------------------------------------------------------------------------------

	struct Out_box : Widget {
		Out_box(Point xy, int w, int h, const string& s)
			: Widget(xy, w, h, s) { }
		Out_box(const string& s) : Widget{s} { }

		void put(int);
		void put(const string&);

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;
	};

//------------------------------------------------------------------------------
	struct Button;
	struct Menu : Widget {
		enum Kind { horizontal, vertical };
		Menu(Point xy, int w, int h, Kind kk, const string& label)
			: Widget{xy, w, h, label}, k{kk} {}
		Menu(Kind kk, const string& label) : Widget{label}, k{kk} {}

		Vector_ref<Button> selection;
		Kind k;

		int attach(Button& b)      // Menu does not delete &b
		{ selection.push_back(b); return int(selection.size() - 1); }
		int attach(Button* p)      // Menu deletes p
		{ selection.push_back(p); return int(selection.size() - 1); }

	private:
		friend struct Button;
		nana::widget* create_nana_widget(nana::widget& own) override;
	};

//------------------------------------------------------------------------------

	struct Button : Widget {
		using callback = std::function<void(Button&)>;
		Button(Point xy, int w, int h, const string& label, callback cb)
			: Widget{xy, w, h, label}, cb_{cb}
		{}
		Button(Point xy, int w, int h, const string& label, std::function<void()> cb)
			: Button{ xy, w, h, label, [cb](Button&) { cb(); } }
		{}
		Button(const string& label, callback cb) : Widget{label}, cb_{cb} {}
		Button(const string& label, std::function<void()> cb)
			: Button{ label, [cb](Button&) { cb(); } }
		{}

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;
		friend nana::widget* Menu::create_nana_widget(nana::widget& own);

		callback cb_;
	};

	//------------------------------------------------------------------------------

	class Canvas : public Widget {
	public:
		Canvas(Point xy, int w, int h) : Widget{xy, w, h, ""} { }
		Canvas() : Widget{""} { }

		using Widget::attach;

		void attach(Shape& s);
		void detach(Shape& s);		// remove s from shapes 
		void put_on_top(Shape& p);	// put p on top of other shapes
		void redraw();
		void set_mouse_callback(Drawing::Mouse_cb cb);
		void set_size_callback(Drawing::Size_cb cb);

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;
	};

//------------------------------------------------------------------------------

	class Animation : public Widget {
	public:
		Animation(Point xy, int w, int h, bool looped = false, int fps = 23)
			: Widget{ xy, w, h, "" }, looped_(looped), fps_(fps) { }
		Animation(bool looped = false, int fps = 23)
			: Widget{ "" }, looped_(looped), fps_(fps) { }

		bool is_looped() const { return looped_; }
		int  fps() const { return fps_; }

		// put frame pictures at the end
		void push_back(const std::vector<string>& img_files);
		// put frame shapes at the end
		void push_back(const std::vector<const Shape*>& shapes);

		void play();
		void pause();

	private:
		nana::widget* create_nana_widget(nana::widget& own) override;

		bool looped_;
		int  fps_;
	};

//------------------------------------------------------------------------------

	inline void Exit() { nana::API::exit(); }	// close all windows

} // of namespace Graph_lib

#endif // GUI_GUARD
