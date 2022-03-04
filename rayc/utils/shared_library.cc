#include "utils/shared_library.h"

#include <dlfcn.h>
#include "utils/log.h"

rayc::SharedLibrary::SharedLibrary(SharedLibrary&& rhs) {
  m_handle = rhs.m_handle;
  rhs.m_handle = nullptr;
}

rayc::SharedLibrary::~SharedLibrary() {
  close();
}

rayc::SharedLibrary& rayc::SharedLibrary::operator=(SharedLibrary&& rhs) {
  m_handle = rhs.m_handle;
  rhs.m_handle = nullptr;
  return *this;
}

void* rayc::SharedLibrary::getSymbol(const std::string& symbolName) const {
  return dlsym(m_handle, symbolName.c_str());
}

rayc::SharedLibrary rayc::SharedLibrary::load(const std::string& filename) {
  SharedLibrary lib;
  lib.open(filename);
  return lib;
}

int rayc::SharedLibrary::open(const std::string& filename) {
  m_handle = dlopen(filename.c_str(), RTLD_NOW);
  if (!m_handle) {
    error("Failed to open SharedLibrary (file '%s'): %s", filename.c_str(), dlerror());
    return -1;
  }
  return 0;
}

void rayc::SharedLibrary::close() {
  if (m_handle) {
    dlclose(m_handle);
    m_handle = nullptr;
  }
}
