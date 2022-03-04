# Caching Tools

CACHE_FILE := $(BUILD)/.cache

# 1 - Folder With Sources
# grep - find line | awk - check timestamp | sed - update timestamp
define Build/Cache/Generate
	generate() {\
		for file in $$1/*; do \
			if [ -d $$file ]; then \
				generate $$file; \
			elif [ -f $$file ]; then \
				if [[ ! $$file == *.built ]] && [ -f $(PKG_BUILD_DIR)/.built ]; then \
					if [ "`date -r $$file +%s`" -gt "`cat $(PKG_BUILD_DIR)/.built`" ]; then \
						echo ; \
					fi; \
				fi; \
			fi; \
		done; \
	}; generate "$(1)";
endef

# 1 - Folder With Sources
define Build/Cache/Update
endef

# 1 - Checks if file was updated since last cache update
define Build/Cache/CheckFile
endef
