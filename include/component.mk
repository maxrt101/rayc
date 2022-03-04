
ifeq ("$(COMPONENT_NAME)","")
$(error "COMPONENT_NAME needs to be specified in copponent/Makefile")
endif

include $(INCLUDE_DIR)/build.mk

# 1 - Source File
# 2 - Additional CFLAGS (optional)
define Component/Compile/File
	$(call Build/Compile/Batch,$(1),$(COMPONENT_NAME),$(2))
endef

# 1 - List of source files
# 2 - Additional CFLAGS (optional)
define Component/Compile/Batch
	$(call Build/Compile/Batch,$(1),$(COMPONENT_NAME),$(2))
endef

# 1 - List of object files
# 2 - Binary name
# 3 - Additional LDFLAGS (optional)
define Component/Link/Binary
	$(call Build/Link/Binary,$(1),$(2),$(COMPONENT_NAME),$(3))
endef

# 1 - List of source files, that were compiled with Build/Compile/Batch
# 2 - Binary name
# 3 - Additional LDFLAGS (optional)
define Component/Link/BinaryFromSrc
	$(call Build/Link/Binary,$(1),$(2),$(COMPONENT_NAME),$(3))
endef

# 1 - List of object files
# 2 - Library name
# 3 - Additional LDFLAGS (optional)
define Component/Link/SharedLibrary
	$(call Build/Link/SharedLibrary,$(1),$(2),$(COMPONENT_NAME),$(3))
endef

# 1 - List of source files, that were compiled with Build/Compile/Batch
# 2 - Library name
# 3 - Additional LDFLAGS (optional)
define Component/Link/SharedLibraryFromSrc
	$(call Build/Link/SharedLibraryFromSrc,$(1),$(2),$(COMPONENT_NAME),$(3))
endef

.PHONY: build

build: compile
	$(call Component/Build)
	@true

compile: prepare
	$(call Component/Compile)
	@true

prepare:
	mkdir -p $(BUILD)/obj/$(COMPONENT_NAME)
	$(call Component/Prepare)
	@true
