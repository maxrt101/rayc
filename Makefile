# rayc

export TOPDIR    := $(shell pwd)
export BUILD_DIR := $(TOPDIR)/build
export CXX       := g++
export CXXFLAGS  := -std=c++17 -I$(BUILD_DIR)/include
export LDFLAGS   := -L$(BUILD_DIR)/lib -lsdl2 -lsdl2_image -lsdl2_ttf -lrayc

LIBRAYC_SRC := src/version.cc \
               src/config.cc \
               src/log.cc \
               src/app.cc \
               src/view.cc \
               src/exception.cc \
               src/math/rect.cc \
               src/data/pak.cc \
               src/data/font.cc \
               src/data/resources.cc \
               src/game/map.cc \
               src/game/state.cc \
               src/game/entity.cc \
               src/game/object.cc \
               src/game/player.cc \
               src/game/raycasting.cc \
               src/renderer/font.cc \
               src/renderer/texture.cc \
               src/renderer/utils.cc \
               src/ui/hud.cc \
               src/script/ast.cc \
               src/script/scanner.cc \
               src/script/parser.cc \
               src/script/driver.cc \
               src/script/builtins.cc \
               src/utils/abi.cc \
               src/utils/path.cc \
               src/utils/string.cc

ifeq ($(DEBUG),1)
$(info [!] Debug on)
CXXFLAGS += -g3 -D_DEBUG
endif

.PHONY: rayc

rayc: librayc
	$(info [+] Build rayc)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/rayc.cc -o $(BUILD_DIR)/bin/rayc

librayc: compile
	$(info [+] Build librayc)
	$(AR) cr $(BUILD_DIR)/lib/librayc.a $(BUILD_DIR)/obj/lib/*.o

compile: install-headers
	$(info [+] Compile objects)
	rm -rf $(BUILD_DIR)/obj/lib
	mkdir -p $(BUILD_DIR)/obj/lib
	for file in $(LIBRAYC_SRC); do \
		echo "[:] Compile $$file"; \
		$(CXX) -c $(CXXFLAGS) $$file -o "$(BUILD_DIR)/obj/lib/$$(echo $${file%.*} | sed 's/\//_/g').o"; \
	done

install-headers: prepare
	$(info [+] Install headers)
	rm -rf $(BUILD_DIR)/include/rayc
	cp -r $(TOPDIR)/include $(BUILD_DIR)/include/rayc

prepare:
	$(info [+] Create directories)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/bin
	mkdir -p $(BUILD_DIR)/include
	mkdir -p $(BUILD_DIR)/lib
	mkdir -p $(BUILD_DIR)/obj

clean:
	$(info [+] Clean build)
	rm -rf $(BUILD_DIR)

$(V).SILENT:
