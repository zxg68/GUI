
#include "GUI.h"
#include <vector>
#include <string>
#include <ctype.h>

using namespace Graph_lib;
using namespace std;

string dtos(double d) {	// remove tralling zeros
	string s = to_string(d);
	while (!s.empty() && s.back() == '0')
		s.pop_back();
	if (!s.empty() && s.back() == '.')
		s.pop_back();
	if (s.empty()) s = "0";
	return s;
}

struct CalcModel {
	enum class State { percent, num, assign, oper };
	State state;
	char op;
	string display;
	double result;
	double oprand;

	CalcModel() { clear(); }

	void clear();
	void num(char n);
	void dot();
	void neg();
	void percent();
	void assign();
	void oper(char op);
};

void CalcModel::clear() {
	state = State::num;
	op = ' ';
	result = oprand = 0.0;
	display = "0";
}

void CalcModel::num(char n) {
	if (state != State::num) display = "";
	if (display == "0") display = "";
	state = State::num;
	display += n;
}

void CalcModel::dot() {
	if (state != State::num) display = "0";
	if (display.find('.') == string::npos)
		display += '.';
	state = State::num;
}

void CalcModel::neg() {
	if (display == "0") return;

	if (display[0] == '-') display.erase(0, 1);
	else display.insert(0, 1, '-');

	if (state == State::assign)
		result = -result;
}

void CalcModel::percent() {
	double d = stod(display);
	d *= result / 100;
	display = dtos(d);
	state = State::percent;
}

void CalcModel::assign() {
	if (state != State::assign)
		oprand = stod(display);
	switch (op) {
	case '+': result += oprand; break;
	case '-': result -= oprand; break;
	case 'X': result *= oprand; break;
	case '/': if (oprand != 0) result /= oprand; break;
	default: result = oprand;
	}
	state = State::assign;
	display = dtos(result);
}

void CalcModel::oper(char op)
{
	if (state <= State::num)
		assign();
	this->op = op;
	state = State::oper;
}

class CalcWin : public Window {
public:
	CalcWin();
	~CalcWin();
private:
	CalcModel model;
	Label display;
	vector<Button*> buttons;

	Button& create_button(char key);
	void key_pressed(char key);
};

CalcWin::CalcWin()
	: Window{300, 300, "Calculator"},
	display{model.display, Format::enable, Align::right}
{
	div("vert<display weight=15% margin=5><weight=2>"
		"<keys margin=2 grid=[4,5] gap=2 collapse(0,4,2,1)>");
	place("display", display);
	display.set_font({"", 18u});
	for (auto key : "Cn%/789X456-123+0.=")
		place("keys", create_button(key));
	collocate();
}

CalcWin::~CalcWin() {
	for (auto pb : buttons)
		delete pb;
}

Button& CalcWin::create_button(char key) {
	auto lbl = key != 'n' ? string(1, key) : (char*)u8"\261";
	auto& kb = *new Button{lbl, [key, this] { key_pressed(key); }};
	buttons.push_back(&kb);
	attach(kb);
	// set_... can only be called after attached or placed
	kb.set_font({"", 12u});
	return kb;
}

void CalcWin::key_pressed(char key) {
	if (isdigit(key)) model.num(key);
	else {
		switch (key) {
		case 'C': model.clear();	break;
		case 'n': model.neg();		break;
		case '%': model.percent();	break;
		case '.': model.dot();		break;
		case '=': model.assign();	break;
		case '+': case '-': case 'X':
		case '/': model.oper(key);	break;
		}
	}
	display.set(model.display);
}

int main()
{
	CalcWin calc;
	gui_main(calc);
}
