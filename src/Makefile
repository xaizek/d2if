CXXFLAGS := -Wall -Werror -std=c++11

PROJECT_NAME := $(notdir $(abspath ..))
SOURCE_FILES := $(wildcard *.cpp)
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)

.PHONY: run

$(PROJECT_NAME): $(OBJECT_FILES)
	g++ $(OBJECT_FILES) -o $@

run: $(PROJECT_NAME)
	./$^ | dzen2 -ta l -h 17 -fn -inconsolata-terminal-bold-*-*--13-*-*-*-*-*-*-*

.cpp.o:
	g++ -c $(CXXFLAGS) -o $@ $<

include $(wildcard *.d)
