CXXFLAGS := -Wall -O3 -MD -Werror -std=c++11
LDFLAGS := -lasound -lX11

PROJECT_NAME := $(notdir $(abspath ..))
SOURCE_FILES := $(wildcard *.cpp)
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)

.PHONY: run clear

$(PROJECT_NAME): $(OBJECT_FILES)
	g++ $(LDFLAGS) $(OBJECT_FILES) -o $@

run: $(PROJECT_NAME)
	./$^ | dzen2 -ta l -h 17 -fn -*-inconsolata-bold-*-*--13-*-*-*-*-*-*-*

.cpp.o:
	g++ -c $(CXXFLAGS) -o $@ $<

clear:
	-$(RM) *.o *.d
	-$(RM) $(PROJECT_NAME)

include $(wildcard *.d)
