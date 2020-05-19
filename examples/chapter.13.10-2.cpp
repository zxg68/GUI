
//
// This is example code from Chapter 13.10 "Managing unnamed objects" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"
#include "Graph.h"
int main()  {
	using namespace Graph_lib;
	Simple_window win = {{100,100}, 600, 400, "16*16 color matrix"};
	// 拥有动态空间对象所有权的容器
	Vector_ref<Rectangle> vr;
	for (auto i = 0; i<16; ++i)
		for (auto j = 0; j<16; ++j) {
			// 转移所有权
			vr.push_back(new Rectangle{{j*20, i*20}, 20, 20});
			int di = 2 * i - 15, dj = 2 * j - 15;
			vr.back().set_fill_color({ i * 16, j * 16, di * dj });
			win.attach(vr.back());
		}
	win.wait_for_button();
	// vr的生存期结束，自动释放动态空间
}
