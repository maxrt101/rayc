COMPONENT_NAME := console

include $(INCLUDE_DIR)/component.mk

CXXFLAGS += -I$(TOPDIR)/rayc

SRC := src/console.cc
TARGET := console.so

define Component/Build
	$(call Component/Link/SharedLibraryFromSrc,$(SRC),$(TARGET),-L$(BUILD)/lib -lrayc)
endef

define Component/Compile
	$(call Component/Compile/Batch,$(SRC))
endef
