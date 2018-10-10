PROG = main
CXX = g++
OBJ = obj/$(PROG).o obj/better_text.o obj/hud_render_window.o \
      obj/line_shape.o obj/useful.o obj/device_handler.o obj/tetris.o
CXXFLAGS = -std=c++14 -Wall -Wextra -Wshadow -Wfloat-equal -Wno-unused-result -O3 -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean all

all: $(PROG)

$(PROG): $(OBJ)
	$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(OBJ) -o $(PROG)

obj/$(PROG).o: $(PROG).cpp include/all.h include/useful.h \
               include/hud_render_window.h include/device_handler.h \
               include/better_text.h include/line_shape.h include/tetris.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj:
	mkdir obj

obj/better_text.o: src/better_text.cpp include/better_text.h
obj/device_handler.o: src/device_handler.cpp include/device_handler.h
obj/hud_render_window.o: src/hud_render_window.cpp \
  include/hud_render_window.h
obj/line_shape.o: src/line_shape.cpp include/line_shape.h
obj/useful.o: src/useful.cpp include/useful.h
obj/tetris.o: src/tetris.cpp include/tetris.h include/device_handler.h

clean:
	rm -f $(OBJ) $(PROG)