CXX					=	g++
override CXXFLAGS	+=	$(shell pkg-config sdl2 SDL2_image --cflags) -g -std=c++17 -I./include
override LDFLAGS	+=	$(shell pkg-config sdl2 SDL2_image --libs) -g

SRCS				=	$(shell find -name "*.cpp")
OBJS				=	$(patsubst ./src/%.cpp,build/%.o,$(SRCS))

.DEFAULT_GOAL		=	all

all:
	mkdir -p $(dir $(OBJS))
	$(MAKE) $(OBJS)
	$(CXX) -o main $(OBJS) $(LDFLAGS)

clean:
	rm -rf build

build/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: all clean