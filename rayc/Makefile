# rayc

CXXFLAGS  := $(CXXFLAGS) -I./
DBGDLAGS  := -g -DDEBUG
SOURCES   := main.cc core/app.cc core/layer.cc core/event.cc core/input.cc res/font.cc res/texture.cc engine/raycaster.cc engine/object.cc engine/map_file.cc layers/base.cc layers/scene.cc layers/scenedda.cc layers/update.cc layers/console.cc utils/debug.cc utils/log.cc
TARGET    := ../bin/rayc

.PHONY: compile

compile:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o $(TARGET)

debug:
	$(CXX) $(CXXFLAGS) $(DBGDLAGS) $(LDFLAGS) $(SOURCES) -o $(TARGET)