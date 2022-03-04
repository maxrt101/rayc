# rayc

export TOPDIR      := $(shell pwd)
export INCLUDE_DIR := $(TOPDIR)/include
export BUILD       := $(TOPDIR)/build

.PHONY: build

build: prepare
	make -C rayc

rayc:
	make -C rayc rayc

components:
	make -C rayc components

compile-one:
	make -C rayc compile-one FILE=$(FILE)

prepare:
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/obj
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	mkdir -p $(BUILD)/include

$(V).SILENT:
