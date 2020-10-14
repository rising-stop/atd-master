#include "file_interface.h"

#include <unistd.h>

#include <algorithm>
#include <iostream>

const std::string& FileAddress::get_CurrentFolder() const {
  return addr_.top().current_name_;
}

const std::string& FileAddress::get_CurrentWorkDir() const {
  return addr_.top().full_path_;
}

std::string FileAddress::get_CurrentDirFileFullPath(
    const std::string& name) const {
  auto res = addr_.top().full_path_;
  res.push_back('/');
  return res.append(name);
}

bool FileAddress::good() const {
  return is_initialized_ && !access(addr_.top().full_path_.c_str(), 0);
}

bool FileAddress::enter_Forward(const std::string& name) {
  return enter_NextFolder(name);
}

bool FileAddress::escape_Backward() { return escape_CurrentFolder(); }

FileAddress::FileAddress() { init(); }

FileAddress::FileAddress(const std::string& path) { init(path); }

void FileAddress::init(const std::string& full_path) {
  if (full_path.empty()) {
    split_FullPath(get_CurrentFullDir());
  } else {
    split_FullPath(full_path);
  }
  is_initialized_ = true;
  std::cout << "full path = " << full_path << std::endl;
  std::cout << "addr top current name " << addr_.top().current_name_
            << "addr top full path " << addr_.top().full_path_ << std::endl;
}

bool FileAddress::is_DirAvailable(const std::string& name) const {
  if (access(get_CurrentDirFileFullPath(name).c_str(), 0)) {
    return false;
  }
  return true;
}

bool FileAddress::enter_NextFolder(const std::string& name) {
  if (!is_DirAvailable(name)) {
    return false;
  }
  auto tmp_path = addr_.top();
  tmp_path.current_name_ = name;
  tmp_path.full_path_.push_back('/');
  tmp_path.full_path_.append(tmp_path.current_name_);
  ++tmp_path.deepth_;
  addr_.push(tmp_path);
  return true;
}

bool FileAddress::escape_CurrentFolder() {
  if (addr_.size() > 1) {
    addr_.pop();
    if (access(addr_.top().full_path_.c_str(), F_OK)) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

std::string FileAddress::get_CurrentFullDir() const {
  int address_char_num = 255;
  char* ptr_path = new char[address_char_num];
  while (!getcwd(ptr_path, address_char_num)) {
    delete ptr_path;
    address_char_num *= 2;
    ptr_path = new char[address_char_num];
  }
  return {ptr_path};
}

void FileAddress::split_FullPath(const std::string& full_path) {
  if (full_path.empty()) {
    return;
  }
  addr_.push({"", "/", 0});
  int header = 1;
  for (int str_index = header; str_index < full_path.size(); ++str_index) {
    if (str_index == full_path.size() - 1) {
      enter_NextFolder(full_path.substr(header, str_index - header + 1));
      header = str_index + 1;
    } else if (full_path.at(str_index) == '/') {
      enter_NextFolder(full_path.substr(header, str_index - header));
      header = str_index + 1;
    }
  }
}

/**
 * @brief defination of class FileInterface
 */

FileInterface::FileInterface(bool* flag,
                             std::function<bool(const std::string&)> func)
    : flag_is_activated_(flag), call_back_(func) {
  init();
}

FileInterface::FileInterface() { init(); }

void FileInterface::render() { draw(); }

void FileInterface::init() {
  ptr_addr_.reset(new FileAddress());
  if (ptr_addr_->good()) {
    scan_FolderContent();
  }
}

void FileInterface::set_ConfirmCallBack(
    std::function<bool(const std::string&)> func) {
  call_back_ = func;
}

size_t FileInterface::get_FileSize(const std::string& file_name) const {
  std::ifstream in(file_name.c_str());
  in.seekg(0, std::ios::end);
  size_t size = in.tellg();
  in.close();
  return size;
}

void FileInterface::scan_FolderContent() {
  current_file_.clear();

  DIR* pDir;
  struct dirent* ptr;

  pDir = opendir(ptr_addr_->get_CurrentWorkDir().c_str());
  if (!pDir) {
    return;
  }

  while ((ptr = readdir(pDir)) != 0) {
    if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
      continue;
    FileInfo tmp_file;
    tmp_file.name_ = ptr->d_name;
    tmp_file.type_ = static_cast<FileInfo::FILE_TYPE>(ptr->d_type);
    tmp_file.size_ =
        get_FileSize(ptr_addr_->get_CurrentDirFileFullPath(tmp_file.name_));
    current_file_.push_back({tmp_file, false});
  }
  closedir(pDir);
  std::sort(current_file_.begin(), current_file_.end(),
            [](const std::pair<FileInfo, bool>& lhs,
               const std::pair<FileInfo, bool>& rhs) -> bool {
              return lhs.first.name_ < rhs.first.name_;
            });
}

void FileInterface::draw() {
  if (!*flag_is_activated_) {
    return;
  }

  ImGui::Begin("Open File", flag_is_activated_);

  if (ImGui::Button("<< Back", ImVec2(0, 0))) {
    if (ptr_addr_->escape_Backward()) {
      scan_FolderContent();
    }
  }
  ImGui::SameLine();
  bool click_confirm = ImGui::Button("Confirm");
  ImGui::SameLine();
  if (ImGui::Button("Cancel")) {
    *flag_is_activated_ = false;
  }
  ImGui::SameLine();
  Filter_.Draw();

  ImGui::BeginChild("file selector", ImVec2(0, 0), true);
  for (auto& item : current_file_) {
    if (Filter_.IsActive()) {
      if (!Filter_.PassFilter(item.first.name_.c_str())) {
        continue;
      }
    }
    if (ImGui::Selectable(item.first.name_.c_str(), item.second,
                          ImGuiSelectableFlags_AllowDoubleClick)) {
      if (click_confirm) {
        if (call_back_(
                ptr_addr_->get_CurrentDirFileFullPath(item.first.name_))) {
          *flag_is_activated_ = false;
        }
      }
      item.second = true;
      if (ImGui::IsMouseDoubleClicked(0)) {
        if (item.first.type_ == FileInfo::FILE_DT_DIR) {
          if (ptr_addr_->enter_Forward(item.first.name_)) {
            scan_FolderContent();
          } else {
            CUSTOM_EXCEPTION("can not open folder : %s",
                             item.first.name_.c_str());
          }
        } else if (item.first.type_ == FileInfo::FILE_DT_REG) {
          if (call_back_(
                  ptr_addr_->get_CurrentDirFileFullPath(item.first.name_))) {
            *flag_is_activated_ = false;
          }
        }
      }
    } else {
      item.second = false;
    }
    ImGui::SameLine(300);
    if (item.first.type_ != FileInfo::FILE_DT_DIR) {
      if (item.first.size_ < 1024u * 1024u) {
        ImGui::Text(" %.2f Kb", static_cast<float>(item.first.size_) / 1024.0f);
      } else if (item.first.size_ < 1024u * 1024u * 1024u) {
        ImGui::Text(" %.2f Mb",
                    static_cast<float>(item.first.size_) / 1024.0f / 1024.0f);
      } else {
        ImGui::Text(" Big file");
      }
    } else {
      ImGui::Text(" folder");
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

void FileInterface::set_ActiveFlag(bool* ptr_flag) {
  flag_is_activated_ = ptr_flag;
}

void ResourceInterface_Manager::set_Button(
    const std::string& id, std::function<bool(const std::string&)> func) {
  if (ImGui::Button("Open")) {
    auto res_ins = source_repository_.insert(std::make_pair(
        std::string(id), std::make_pair(new FileInterface(), new bool(true))));
    if (res_ins.second) {
      res_ins.first->second.first->set_ActiveFlag(res_ins.first->second.second);
      res_ins.first->second.first->set_ConfirmCallBack(func);
    } else {
      *(res_ins.first->second.second) = true;
    }
  } else {
    auto itr_find = source_repository_.find(std::string(id));
    if (itr_find != source_repository_.end()) {
      source_repository_[id].first->render();
    }
  }
}

ResourceInterface_Manager::~ResourceInterface_Manager() {
  for (auto& item : source_repository_) {
    delete item.second.first;
    delete item.second.second;
  }
}
