GCC	= g++
CXXFLAGS= -std=c++11

NANAPATH = ../nana
TARGET	= libsfddgui.a
SOURCES = $(shell ls *.cpp)

NANAINC	= $(NANAPATH)/include
NANALIB = $(NANAPATH)/build/bin

INCS	= -I$(NANAINC)

LINKOBJ	= $(SOURCES:.cpp=.o)

$(TARGET): $(LINKOBJ)
	ar rcs $@ $?
	ranlib $@

.cpp.o:
	$(GCC) -g -c $< -o $@ $(INCS) $(CXXFLAGS)

clean:
	rm -f $(LINKOBJ) $(TARGET)
