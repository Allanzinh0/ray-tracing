CPPFLAGS=-std=c++20 -Wall -O3 -g
LDFLAGS=-Wall -O3 -g

SOURCES=$(wildcard src/*.cpp src/**/*.cpp)
HEADERS=$(wildcard src/*.h src/**/*.h)
OBJECTS=obj/imgui_impl_opengl3.o obj/imgui_impl_glfw.o $(patsubst src/%.cpp,obj/%.o,$(SOURCES))
INCLUDE_DIRS=vendor/imgui vendor/imgui/backends vendor/glfw/include vendor/glad
LIBS=imgui glfw3 glad

all: dist/dev.out
	./dist/dev.out

dist/dev.out: $(OBJECTS) $(patsubst %,lib/lib%.a,$(LIBS))
	@mkdir -p $(@D)
	g++ -Llib $(LDFLAGS) $(OBJECTS) $(patsubst %,-l%,$(LIBS)) -o dist/dev.out

obj/%.o: src/%.cpp $(HEADERS) $(INCLUDE_DIRS)
	@mkdir -p $(@D)
	g++ $(CPPFLAGS) $(patsubst %,-I%,$(INCLUDE_DIRS)) -c $< -o $@ $(HEADERS)

lib/libglfw3.a:
	@mkdir -p $(@D)
	@cmake -S vendor/glfw -B vendor/glfw/build
	@make -C vendor/glfw/build
	cp vendor/glfw/build/src/libglfw3.a $@

lib/libglad.a:
	@mkdir -p $(@D)
	@make -C vendor/glad
	cp vendor/glad/libglad.a $@

lib/libimgui.a:
	@mkdir -p $(@D)
	@mkdir -p obj
	@make -C vendor/imgui
	cp vendor/imgui/libimgui.a $@

obj/imgui_impl_opengl3.o: vendor/imgui/backends/imgui_impl_opengl3.cpp $(INCLUDE_DIRS)
	@mkdir -p $(@D)
	g++ $(CPPFLAGS) $(patsubst %,-I%,$(INCLUDE_DIRS)) -c $< -o $@ $(HEADERS)

obj/imgui_impl_glfw.o: vendor/imgui/backends/imgui_impl_glfw.cpp $(INCLUDE_DIRS)
	@mkdir -p $(@D)
	g++ $(CPPFLAGS) $(patsubst %,-I%,$(INCLUDE_DIRS)) -c $< -o $@ $(HEADERS)

clean:
	@rm -rf dist obj lib
	@make -C vendor/glfw/build clean
	@make -C vendor/glad clean
	@make -C vendor/imgui clean