#ifndef _RAYC_UTILS_SHARED_LIBRARY_H
#define _RAYC_UTILS_SHARED_LIBRARY_H

#include <string>

namespace rayc {

class SharedLibrary {
 private:
  void* m_handle = nullptr;

 public:
  SharedLibrary() = default;
  SharedLibrary(SharedLibrary&& rhs);
  SharedLibrary(const SharedLibrary& rhs) = delete;
  ~SharedLibrary();

  SharedLibrary& operator=(SharedLibrary&& rhs);

  void* getSymbol(const std::string& symbolName) const;
  
  template <typename T>
  inline T getSymbolAs(const std::string& symbolName) const {
    return (T)getSymbol(symbolName);
  }

  static SharedLibrary load(const std::string& filename);

 private:
  int open(const std::string& filename);
  void close();
};

} /* namespace rayc */

#endif
