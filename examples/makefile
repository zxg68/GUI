GCC	= g++
CXXFLAGS = -std=c++11

NANAPATH = ../nana
GUIPATH = ../GUI_v3

BIN	= gui-example
SOURCES = calculator.cpp
#SOURCES = chapter.12.3.cpp
#SOURCES = chapter.12.7.10.cpp
#SOURCES = chapter.13.2.cpp
#SOURCES = chapter.13.5-1.cpp
#SOURCES = chapter.13.6.cpp
#SOURCES = chapter.13.7.cpp
#SOURCES = chapter.13.8-1.cpp
#SOURCES = chapter.13.8-2.cpp
#SOURCES = chapter.13.9-4.cpp
#SOURCES = chapter.13.10-2.cpp
#SOURCES = chapter.13.11.cpp
#SOURCES = chapter.13.12.cpp
#SOURCES = chapter.13.13.cpp
#SOURCES = chapter.13.14.cpp
#SOURCES = chapter.13.15.cpp
#SOURCES = chapter.13.16.cpp
#SOURCES = chapter.13.17.cpp
#SOURCES = chapter.15.2-4.cpp
#SOURCES = chapter.16.5.cpp
#SOURCES = chapter.16.5-mouse.cpp
#SOURCES = chapter.16.5-place.cpp
#SOURCES = chapter.16.7.cpp
#SOURCES = fractal.cpp
#SOURCES = polar_func.cpp
#SOURCES = test_label.cpp

NANAINC	= $(NANAPATH)/include
NANALIB = $(NANAPATH)/build/bin

INCS	= -I$(GUIPATH) -I$(NANAINC)
LIBS	= -L$(GUIPATH) -L$(NANALIB) -lsfddgui -lnana -lstdc++fs -lX11 -lpthread -lrt -lXft -lpng -lasound -lfontconfig

LINKOBJ	= $(SOURCES:.cpp=.o)

$(BIN): $(LINKOBJ) $(GUIPATH)/libsfddgui.a $(NANALIB)/libnana.a
	$(GCC) $(LINKOBJ) $(INCS) $(LIBS) -o $(BIN) $(CXXFLAGS)

.cpp.o:
	$(GCC) -g -c $< -o $@ $(INCS) $(CXXFLAGS)

$(GUIPATH)/libsfddgui.a: 
	make --directory=$(GUIPATH)

$(NANALIB)/libnana.a:
	make --directory=$(NANAPATH)/build/makefile

clean:
	rm -f *.o $(BIN)
