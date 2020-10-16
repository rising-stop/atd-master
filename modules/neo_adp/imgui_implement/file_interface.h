#pragma once
#include <dirent.h>

#include <functional>
#include <stack>

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_repository.h"

struct FolderInfo {
  std::string current_name_;
  std::string full_path_;
  int deepth_ = 0;

  FolderInfo() = default;
  FolderInfo(const std::string& name, const std::string& path, int deepth)
      : current_name_(name), full_path_(path), deepth_(deepth) {}
};

struct FileInfo {
  enum FILE_TYPE : uint8_t {
    FILE_DT_UNKNOWN = 0, /* unknow */
    FILE_DT_FIFO = 1,    /* named pipe */
    FILE_DT_CHR = 2,     /* character device */
    FILE_DT_DIR = 4,     /* directory */
    FILE_DT_BLK = 6,     /* block device */
    FILE_DT_REG = 8,     /* regular file */
    FILE_DT_LNK = 10,    /* symbolic link */
    FILE_DT_SOCK = 12,   /* UNIX domain socket */
    FILE_DT_WHT = 14
  };

  std::string name_;
  FILE_TYPE type_ = FILE_DT_UNKNOWN;
  size_t size_ = 0;
};

class FileAddress {
 public:
  const std::string& get_CurrentFolder() const;
  const std::string& get_CurrentWorkDir() const;
  std::string get_CurrentDirFileFullPath(const std::string&) const;

  bool good() const;
  bool enter_Forward(const std::string&);
  bool escape_Backward();

 private:
  void init(const std::string& name = std::string());

  bool is_DirAvailable(const std::string&) const;
  bool enter_NextFolder(const std::string&);
  bool escape_CurrentFolder();

  std::string get_CurrentFullDir() const;
  void split_FullPath(const std::string&);

  std::stack<FolderInfo> addr_;

  bool is_initialized_ = false;

 public:
  FileAddress(const std::string&);
  FileAddress();
};

class FileInterface : public ImGui_Components {
 public:
  virtual void render() override;
  void set_ActiveFlag(bool*);
  void set_ConfirmCallBack(std::function<bool(const std::string&)>);

 private:
  size_t get_FileSize(const std::string&) const;

  void scan_FolderContent();
  void init();
  void draw();

  bool* flag_is_activated_ = nullptr;
  std::shared_ptr<FileAddress> ptr_addr_;
  std::vector<std::pair<FileInfo, bool>> current_file_;

  ImGuiTextFilter Filter_;

  std::function<bool(const std::string&)> call_back_;

 public:
  FileInterface(bool*, std::function<bool(const std::string&)>);
  FileInterface();
  ~FileInterface() = default;
};

class ResourceInterface_Manager : public atd::utility::Singleton {
  friend atd::utility::Singleton;

 public:
  void set_Button(const std::string&, std::function<bool(const std::string&)>);

 private:
  std::unordered_map<std::string, std::pair<FileInterface*, bool*>>
      source_repository_;

 private:
  ResourceInterface_Manager() = default;
  ~ResourceInterface_Manager();
};

#define OPEN_BUTTON(name, func)                                               \
  atd::utility::Singleton::instance<ResourceInterface_Manager>()->set_Button( \
      name, func)
