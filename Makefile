# rayc

export CXX      := clang++
export CXXFLAGS := -std=c++17
export LDFLAGS  := -lsdl2 -lsdl2_image -lsdl2_ttf

.PHONY: rayc rayc-map

rayc:
	make -C rayc

rayc-dbg:
	make -C rayc debug

rayc-map:
	make -C rayc-map