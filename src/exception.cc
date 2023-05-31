#include <rayc/exception.h>

rayc::FileOpenException::FileOpenException(const std::string& filename)
  : std::runtime_error("Failed to open file " + filename) {}

rayc::FileReadException::FileReadException(const std::string& filename)
  : std::runtime_error("Failed to read from file " + filename) {}

rayc::FileWriteException::FileWriteException(const std::string& filename)
  : std::runtime_error("Failed to write to file " + filename) {}

rayc::ResourceCreationException::ResourceCreationException(const std::string& res, const std::string& msg)
  : std::runtime_error("Failed to create resource " + res + ": " + msg) {}

rayc::SubsystemInitFailedException::SubsystemInitFailedException(const std::string& res, const std::string& msg)
  : std::runtime_error("Failed to initialize " + res + " sybsystem: " + msg) {}

rayc::VersionMismatchException::VersionMismatchException(const std::string& msg)
  : std::runtime_error("Version Mismatch: " + msg) {}

rayc::MissingConfigEntryException::MissingConfigEntryException(const std::string& msg)
  : std::runtime_error("Missing Config Entry: " + msg) {}
