#include <rayc/data/pak.h>
#include <rayc/utils/string.h>
#include <rayc/utils/path.h>
#include <rayc/utils/abi.h>
#include <rayc/exception.h>
#include <rayc/version.h>
#include <rayc/log.h>

#include <cstdio>
#include <errno.h>

namespace fs = std::filesystem;


rayc::DataPak::FileEntry rayc::DataPak::FileEntry::fromBytes(uint8_t* data, uint32_t& offset) {
  rayc::DataPak::FileEntry entry;

  for (char c = data[offset++]; c != '\0'; c = data[offset++]) {
    entry.name.push_back(c);
  }

  abi::N32 n;

  readInt32(n, data, offset);
  entry.size = n.u32;

  readInt32(n, data, offset += 4);
  entry.offset = n.u32;

  offset += 4;

  return entry;
}


rayc::DataPak::Node::Node(const std::string& name, Type t)
  : name(name), type(t) {}


rayc::DataPak::Node* rayc::DataPak::Node::copy() {
  return new Node(name, type);
}


bool rayc::DataPak::Node::isFolder() {
  return type == N_FOLDER;
}


bool rayc::DataPak::Node::isFile() {
  return type == N_FILE;
}


rayc::DataPak::Folder* rayc::DataPak::Node::asFolder() {
  return (Folder*) this;
}


rayc::DataPak::File* rayc::DataPak::Node::asFile() {
  return (File*) this;
}


rayc::DataPak::Folder::Folder(const std::string& name)
  : Node(name, N_FOLDER) {}


rayc::DataPak::Folder::Folder(const Folder& rhs) : Folder(rhs.name) {
  for (auto& [path, node] : rhs.nodes) {
    nodes[path] = node->copy();
  }
}


rayc::DataPak::Folder::~Folder() {
  for (auto& [_, node] : nodes) {
    delete node;
  }
}


rayc::DataPak::Node* rayc::DataPak::Folder::copy() {
  Folder* tmp = new Folder(name);
  for (auto& [path, node] : nodes) {
    tmp->nodes[path] = node->copy();
  }
  return tmp;
}


rayc::DataPak::Node*& rayc::DataPak::Folder::operator[](const std::string& name) {
  return nodes[name];
}


std::map<std::string, rayc::DataPak::Node*>& rayc::DataPak::Folder::operator*() {
  return nodes;
}


bool rayc::DataPak::Folder::contains(const std::string& name) {
  return nodes.find(name) != nodes.end();
}


std::map<std::string, rayc::DataPak::Node*>& rayc::DataPak::Folder::entries() {
  return nodes;
}


rayc::DataPak::Node*& rayc::DataPak::Folder::get(const std::string& name) {
  return nodes[name];
}


rayc::DataPak::File::File(const std::string& name) : Node(name, N_FILE) {}


rayc::DataPak::File::File(const File& rhs) : File(rhs.name) {
  data = rhs.data;
}


rayc::DataPak::File::~File() {}


rayc::DataPak::Node* rayc::DataPak::File::copy() {
  File* tmp = new File(name);
  tmp->data = data;
  return tmp;
}


rayc::DataPak::DataPak() : root("") {}


rayc::DataPak::DataPak(DataPak&& rhs) : root(rhs.root), header(rhs.header) {
  rhs.root.nodes.clear();
}


rayc::DataPak::~DataPak() {}


void rayc::DataPak::operator=(DataPak&& rhs) {
  header = rhs.header;
  root = rhs.root;

  rhs.root.nodes.clear();
}


rayc::DataPak::Node*& rayc::DataPak::operator[](const std::string& name) {
  return root.nodes[name];
}


std::map<std::string, rayc::DataPak::Node*>& rayc::DataPak::operator*() {
  return root.nodes;
}


bool rayc::DataPak::contains(const std::string& name) {
  return root.contains(name);
}


std::map<std::string, rayc::DataPak::Node*>& rayc::DataPak::entries() {
  return root.nodes;
}


std::vector<uint8_t> rayc::DataPak::toBytes() {
  std::vector<uint8_t> buffer;
  std::vector<uint8_t> fileBlob;
  std::vector<FileEntry> table;

  abi::N32 n;
  
  auto push_u32 = [&n, &buffer] {
    for (int i = 0; i < 4; i++)
      buffer.push_back(n.u8[i]);
  };
  
  packFolder(&root, "", table, fileBlob);

  for (int i = 0; i < 4; i++) {
    buffer.push_back(PAK_MAGIC[i]);
  }

  n.u32 = PAK_VERSION;
  push_u32();

  n.u32 = getVersionCode();
  push_u32();

  n.u32 = table.size();
  push_u32();

  n.u32 = fileBlob.size();
  push_u32();

  for (auto& entry : table) {
    for (int i = 0; i < entry.name.size(); i++) {
      buffer.push_back(entry.name[i]);
    }
    buffer.push_back(0);
  
    n.u32 = entry.size;
    push_u32();

    n.u32 = entry.offset;
    push_u32();
  }

  buffer.insert(buffer.end(), fileBlob.begin(), fileBlob.end());

  return buffer;
}


void rayc::DataPak::toFile(const std::string& filename) {
  FILE* file = fopen(filename.c_str(), "wb");

  if (!file) {
    error("Failed to open/create '%s'", filename.c_str());
    throw FileOpenException(filename);
  }

  auto buffer = toBytes();
  fwrite(buffer.data(), 1, buffer.size(), file);

  fclose(file);
}


void rayc::DataPak::dump(const std::string& folder) {
  fs::create_directory(folder);
  dumpNode(&root, folder); 
}


rayc::DataPak rayc::DataPak::fromBytes(uint8_t* data) {
  DataPak pak;

  std::vector<FileEntry> table;
  std::vector<uint8_t> blob;

  abi::N32 n;

  readInt32(n, data, 0);
  pak.header.magic = n.u32;

  readInt32(n, data, 4);
  pak.header.fileVersion = n.u32;

  readInt32(n, data, 8);
  pak.header.gameVersion = n.u32;

  readInt32(n, data, 12);
  pak.header.tableEntryCount = n.u32;

  readInt32(n, data, 16);
  pak.header.dataSize = n.u32;

  uint32_t offset = 20;

  for (int i = 0; i < pak.header.tableEntryCount; i++) {
    FileEntry entry = FileEntry::fromBytes(data, offset);
    table.push_back(entry);
  }

  blob = std::vector(data + offset, data + offset + pak.header.dataSize);

  pak.parseFileTable(table, blob);

  return pak;
}


rayc::DataPak rayc::DataPak::fromFile(const std::string& filename) {
  FILE* file = fopen(filename.c_str(), "rb");
  if (!file) {
    if (errno == 2) {
      error("No such file or directory: '%s'", filename.c_str());
    } else {
      error("Failed to open '%s'", filename.c_str());
    }
    throw FileOpenException(filename);
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  uint8_t* data = new uint8_t[size];
  if (fread(data, 1, size, file) != size) {
    error("Error: couldn't read file (errno=%d)", errno);
    delete [] data;
    fclose(file);
    throw FileReadException(filename);
  }

  DataPak pak = DataPak::fromBytes(data);

  delete [] data;
  fclose(file);

  return pak;
}


rayc::DataPak rayc::DataPak::fromFolder(const std::string& folder) {
  rayc::DataPak pak;

  for (const auto& entry : fs::directory_iterator(folder)) {
    pak.addDirectoryEntry(entry, "");
  }

  return pak;
}


void rayc::DataPak::parseFileTable(std::vector<FileEntry>& table, std::vector<uint8_t>& blob) {
  std::vector<std::string> paths;

  for (auto& entry : table) {
    paths.push_back(entry.name);
  }

  for (auto& entry : table) {
    std::vector<std::string> filePath = strsplit(entry.name, "/");

    Folder* parentFolder = &root;
    for (int i = 0; i < filePath.size()-1; i++) {
      if (parentFolder->nodes.find(filePath[i]) == parentFolder->nodes.end()) {
        parentFolder->nodes[filePath[i]] = new Folder(filePath[i]);
      }
      parentFolder = parentFolder->nodes[filePath[i]]->asFolder();
    }

    File* file = new File(filePath.back());
    file->data = std::vector(blob.begin() + entry.offset, blob.begin() + entry.offset + entry.size);
    parentFolder->nodes[filePath.back()] = file;
  }
}


void rayc::DataPak::packFolder(Folder* folder, std::string path, std::vector<FileEntry>& table, std::vector<uint8_t>& blob) {
  path = path.size() ? path + "/" + folder->name : folder->name;
  for (auto& [name, node] : folder->nodes) {
    if (node->isFolder()) {
      packFolder(node->asFolder(), path, table, blob);
    } else {
      packFile(node->asFile(), path, table, blob);
    }
  }
}


void rayc::DataPak::packFile(File* file, std::string path, std::vector<FileEntry>& table, std::vector<uint8_t>& blob) {
  FileEntry entry;
  path = path.size() ? path + "/" : ""; 
  entry.name = path + file->name;
  entry.size = file->data.size();
  entry.offset = blob.size();
  blob.insert(blob.end(), file->data.begin(), file->data.end());
  table.push_back(entry);
}


void rayc::DataPak::addDirectoryEntry(const fs::directory_entry& dir, const std::string& pathFromPakRoot) {
  if (dir.is_directory()) {
    std::string path = (pathFromPakRoot.size() ? pathFromPakRoot + "/" : "") + getFileNameFromPath(dir.path());
    for (const auto& entry : fs::directory_iterator(dir.path())) {
      addDirectoryEntry(entry, path);
    }
  } else {
    FILE* file = fopen(dir.path().c_str(), "rb");

    if (!file) {
      if (errno == 2) {
        error("No such file or directory: '%s'", dir.path().c_str());
      } else {
        error("Failed to open '%s'", dir.path().c_str());
      }
      throw FileOpenException(dir.path());
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    uint8_t* data = new uint8_t[size];

    if (fread(data, 1, size, file) != size) {
      error("Error: couldn't read file (errno=%d)", errno);
      delete [] data;
      fclose(file);
      throw FileReadException(dir.path());
    }

    std::string path = pathFromPakRoot + "/" + getFileNameFromPath(dir.path());
    std::vector<std::string> filePath = strsplit(path, "/");

    Folder* parentFolder = &root;
    for (int i = 0; i < filePath.size()-1; i++) {
      if (parentFolder->nodes.find(filePath[i]) == parentFolder->nodes.end()) {
        parentFolder->nodes[filePath[i]] = new Folder(filePath[i]);
      }
      parentFolder = parentFolder->nodes[filePath[i]]->asFolder();
    }

    File* pakFile = new File(filePath.back());
    pakFile->data = std::vector<uint8_t>(data, data + size);
    parentFolder->nodes[filePath.back()] = pakFile;

    delete [] data;
    fclose(file);
  }
}


void rayc::DataPak::dumpNode(Node* node, std::string path) {
  if (node->isFolder()) {
    path = (path.size() ? path + "/" : "") + node->name;
    fs::create_directory(path);
    for (auto& [name, subnode] : node->asFolder()->nodes) {
      dumpNode(subnode, path);
    }
  } else {
    std::string filename = path + "/" + node->name;
    FILE* file = fopen(filename.c_str(), "wb");

    if (!file) {
      error("Failed to open/create '%s'", filename.c_str());
      throw FileOpenException(filename);
    }

    File* pakFile = node->asFile();

    // auto buffer = toBytes();
    fwrite(pakFile->data.data(), 1, pakFile->data.size(), file);

    fclose(file);
  }
}
