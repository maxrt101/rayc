#ifndef _RAYC_DATA_PAK_H_
#define _RAYC_DATA_PAK_H_ 1

#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <filesystem>

namespace rayc {

constexpr uint8_t PAK_MAGIC[] = {'P', 'A', 'K', 'X'};
constexpr uint32_t PAK_VERSION = 0x00000001;

class DataPak {
 public:
  struct Header {
    uint32_t magic;
    uint32_t fileVersion;
    uint32_t gameVersion;
    uint32_t tableEntryCount;
    uint32_t dataSize;
  };

  struct FileEntry {
    std::string name;
    uint32_t size;
    uint32_t offset;

    static FileEntry fromBytes(uint8_t* data, uint32_t& offset);
  };

  struct Folder;
  struct File;

  struct Node {
    std::string name;
    enum Type {N_FOLDER, N_FILE} type;

   public:
    Node(const std::string& name, Type t);
    virtual ~Node() = default;

    virtual Node* copy();

    bool isFolder();
    bool isFile();

    Folder* asFolder();
    File* asFile();
  };

  struct Folder : public Node {
    std::map<std::string, Node*> nodes;

   public:
    Folder(const std::string& name);
    Folder(const Folder& rhs);
    ~Folder();

    Node* copy() override;

    Node*& operator[](const std::string& name);
    std::map<std::string, Node*>& operator*();
    Node*& get(const std::string& name);
    bool contains(const std::string& name);
    std::map<std::string, Node*>& entries();
  };

  struct File : public Node {
    std::vector<uint8_t> data;

   public:
    File(const std::string& name);
    File(const File& rhs);
    ~File();

    Node* copy() override;
  };

 public:
  DataPak();
  DataPak(const DataPak&) = delete;
  DataPak(DataPak&& rhs);
  ~DataPak();

  void operator=(DataPak&& rhs);

  Node*& operator[](const std::string& name);
  std::map<std::string, Node*>& operator*();  
  bool contains(const std::string& name);
  std::map<std::string, Node*>& entries();

  std::vector<uint8_t> toBytes();

  void toFile(const std::string& filename);
  void dump(const std::string& folder);

  static DataPak fromBytes(uint8_t* data);
  static DataPak fromFile(const std::string& filename);
  static DataPak fromFolder(const std::string& folder);

 private:
  void parseFileTable(std::vector<FileEntry>& table, std::vector<uint8_t>& blob);

  void packFolder(Folder* folder, std::string path, std::vector<FileEntry>& table, std::vector<uint8_t>& blob);
  void packFile(File* file, std::string path, std::vector<FileEntry>& table, std::vector<uint8_t>& blob);

  void addDirectoryEntry(const std::filesystem::directory_entry& dir, const std::string& pathFromPakRoot);

  void dumpNode(Node* node, std::string path);

 private:
  Header header;
  Folder root;
};

} /* namespace rayc */

#endif /* _RAYC_DATA_PAK_H_ */