CXXFLAGS := -U__GTHREADS -MD -Wall -Werror -std=c++11
LDFLAGS := -lasound -lX11

DZEN2 ?= dzen2

PROJECT_NAME := $(notdir $(abspath ..))
SOURCE_FILES := $(wildcard *.cpp)
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)

.PHONY: release debug run clear

release: CXXFLAGS += -O3
release: export STRIP := yes
release: $(PROJECT_NAME)

debug: CXXFLAGS += -O0 -g
debug: CXXFLAGS += -D_GLIBCXX_DEBUG
debug: LDFLAGS += -O0 -g
debug: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJECT_FILES)
	g++ $(LDFLAGS) $(OBJECT_FILES) -o $@
	if [ -n "$$STRIP" ]; then strip $@; fi

run: $(PROJECT_NAME)
	./$^ | $(DZEN2) -ta l -h 17 -fn -*-inconsolata-bold-*-*--13-*-*-*-*-*-*-*

.cpp.o:
	g++ -c $(CXXFLAGS) -o $@ $<

clear:
	-$(RM) *.o *.d
	-$(RM) $(PROJECT_NAME)

include $(wildcard *.d)
