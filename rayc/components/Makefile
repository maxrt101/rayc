
export CFLAGS += -I$(TOPDIR)/rayc

define Component/Build/All
	do_build() { \
		for COMPONENT in $$1; do \
			make -C $$COMPONENT; \
		done; \
	}; do_build "$(1)"
endef

.PHONY: build

build:
	$(call Component/Build/All,$(COMPONENTS))