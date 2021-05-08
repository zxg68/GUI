# 图形库 GUI_v3

## 编译 GUI 库

1. 编译`nana`库并生成`libnana.a`链接库
2. 修改`CMakeLists.txt`第8行为`nana/include`的路径

最后运行
``` shell
cmake .
```
和
``` shell
make
```
命令，得到链接库文件`lib/libGUI_v3.a`

## 其他说明

编译nana库时若找不到Xcursor链接库，可能是因为相关包的版本过高。执行命令
``` shell
sudo apt install libxcursor-dev
```
并按照提示指定相应版本的依赖库即可。

## 其它版本

An extended version of the GUI interface library in PPP (http://www.stroustrup.com/programming.html).

Replace the underlying GUI library from FLTK to nana (https://github.com/cnjinhao/nana).
