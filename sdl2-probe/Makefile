SRCS := \
	imgui/imgui_widgets.cpp \
	imgui/imgui_tables.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_demo.cpp \
	imgui/imgui.cpp \
	imgui/misc/cpp/imgui_stdlib.cpp \
	imgui/backends/imgui_impl_sdl2.cpp \
	imgui/backends/imgui_impl_sdlrenderer2.cpp \
	main.cpp

OBJS := $(addprefix build/obj/, $(addsuffix .o, $(SRCS)))

INCLUDES := -I. -Iimgui -I/usr/include/SDL2
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf
FLAGS := -Wall -Werror 

build/a.out: $(OBJS)
	g++ -g -o build/a.out $(FLAGS) $(OBJS) $(LIBS)

run: $(OBJS) build/a.out
	./build/a.out

build/obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	g++ -g -c $^ $(FLAGS) -o $@ $(INCLUDES)

clean:
	rm -rf build/

.PHONY: clean
