COMPONENT_NAME := test

include $(INCLUDE_DIR)/component.mk

CXXFLAGS += -I$(TOPDIR)/rayc

SRC := src/test.cc
TARGET := test.so

define Component/Build
	$(call Component/Link/SharedLibraryFromSrc,$(SRC),$(TARGET),-L$(BUILD)/lib -lrayc)
endef

define Component/Compile
	$(call Component/Compile/Batch,$(SRC))
endef
