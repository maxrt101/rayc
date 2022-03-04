
include $(INCLUDE_DIR)/build.mk
include .config

CXXFLAGS  += -I$(TOPDIR)/rayc
LIBSRC    := core/app.cc core/store.cc core/component.cc io/input.cc utils/die.cc utils/log.cc utils/config.cc utils/shared_library.cc
BINSRC    := main.cc
LIBNAME   := rayc
LIBTARGET := lib$(LIBNAME).a
BINTARGET := rayc
OBJ_DIR   := rayc

.PHONY: build components

components: rayc
	make -C components COMPONENTS=$(COMPONENTS)

rayc: build

build: compile
	$(call Build/Link/StaticLibraryFromSrc,$(LIBSRC),$(LIBTARGET),$(OBJ_DIR))
	$(call Build/Link/BinaryFromSrc,$(BINSRC),$(BINTARGET),$(OBJ_DIR),-L$(BUILD)/lib -l$(LIBNAME))

compile: prepare
	$(call Build/Compile/Batch,$(LIBSRC) $(BINSRC),$(OBJ_DIR))

prepare:
	mkdir -p $(BUILD)/obj/$(OBJ_DIR)

compile-one:
	$(call Build/Compile/File,$(FILE))
