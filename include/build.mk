# Build Tools

export CXX         := clang++
export CXXFLAGS    := -std=c++17 -I$(BUILD)/include
export LDFLAGS     := -lsdl2 -lsdl2_image -lsdl2_ttf

include $(INCLUDE_DIR)/config.mk

# 1 - Source File
# 2 - Build folder under BUILD/obj (optional)
# 3 - Additional CFLAGS (optional)
define Build/Compile/File
	do_compile() { \
		$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(3) $$1 -o "$(BUILD)/obj/$(2)/$${1%.*}.o"; \
	}; do_compile "$(1)";
endef

# 1 - List of source files
# 2 - Build folder under BUILD/obj (optional)
# 3 - Additional CFLAGS (optional)
define Build/Compile/Batch
	do_compile() { \
		for FILE in $$1; do \
			$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(3) $$FILE -o "$(BUILD)/obj/$(2)/$$(basename $${FILE%.*}).o"; \
		done; \
	}; do_compile "$(1)";
endef

# 1 - List of object files
# 2 - Binary name
# 3 - Build folder under BUILD/obj (optional)
# 4 - Additional LDFLAGS (optional)
define Build/Link/Binary
	$(CXX) $(LDFLAGS) $(4) $(addprefix $(BUILD)/obj/$(3)/, $(1)) -o $(BUILD)/bin/$(2)
endef

# 1 - List of source files, that were compiled with Build/Compile/Batch
# 2 - Binary name
# 3 - Build folder under BUILD/obj (optional)
# 4 - Additional LDFLAGS (optional)
define Build/Link/BinaryFromSrc
	$(call Build/Link/Binary,$(addsuffix .o, $(notdir $(basename $(1)))),$(2),$(3),$(4))
endef
# $(CXX) $(LDFLAGS) $(addprefix $(BUILD)/obj/, $(addsuffix .o, $(notdir $(basename $(1))))) -o $(BUILD)/bin/$(2)

# 1 - List of object files
# 2 - Library name
# 3 - Build folder under BUILD/obj (optional)
# 4 - Additional LDFLAGS (optional)
define Build/Link/SharedLibrary
	$(CXX) $(LDFLAGS) -shared $(4) $(addprefix $(BUILD)/obj/$(3)/, $(1)) -o $(BUILD)/lib/$(2)
endef

# 1 - List of source files, that were compiled with Build/Compile/Batch
# 2 - Library name
# 3 - Build folder under BUILD/obj (optional)
# 4 - Additional LDFLAGS (optional)
define Build/Link/SharedLibraryFromSrc
	$(call Build/Link/SharedLibrary,$(addsuffix .o, $(notdir $(basename $(1)))),$(2),$(3),$(4))
endef

# 1 - List of object files
# 2 - Library name
# 3 - Build folder under BUILD/obj (optional)
define Build/Link/StaticLibrary
	$(AR) cr $(BUILD)/lib/$(2) $(addprefix $(BUILD)/obj/$(3)/, $(1))
endef

# 1 - List of source files, that were compiled with Build/Compile/Batch
# 2 - Library name
# 3 - Build folder under BUILD/obj (optional)
define Build/Link/StaticLibraryFromSrc
	$(call Build/Link/StaticLibrary,$(addsuffix .o, $(notdir $(basename $(1)))),$(2),$(3))
endef
