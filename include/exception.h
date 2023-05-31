#ifndef _RAYC_EXCEPTION_H_
#define _RAYC_EXCEPTION_H_ 1

#include <exception>
#include <string>

namespace rayc {

struct FileOpenException : public std::runtime_error {
  FileOpenException(const std::string& filename);
};

struct FileReadException : public std::runtime_error {
  FileReadException(const std::string& filename);
};

struct FileWriteException : public std::runtime_error {
  FileWriteException(const std::string& filename);
};

struct ResourceCreationException : public std::runtime_error {
  ResourceCreationException(const std::string& res, const std::string& msg);
};

struct SubsystemInitFailedException : public std::runtime_error {
  SubsystemInitFailedException(const std::string& res, const std::string& msg);
};

struct VersionMismatchException : public std::runtime_error {
  VersionMismatchException(const std::string& msg);
};

struct MissingConfigEntryException : public std::runtime_error {
  MissingConfigEntryException(const std::string& msg);
};

} /* namespace rayc */

#endif /* _RAYC_EXCEPTION_H_ */