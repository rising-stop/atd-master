#include "log_file_handler.h"

bool LCM_File_Handler::open_LogFile(const std::string& name) {
  if (mode_ == WRITE) {
    CUSTOM_EXCEPTION("LCM_File_Handler: open mode error");
  }
  name_ = name;
  mode_ = READ;
  auto ptr_file = std::make_shared<lcm::LogFile>((name, "r"));
  if (!ptr_file->good()) {
    return false;
  }
  init(ptr_file);
}

bool LCM_File_Handler::create_LogFile(const std::string& name) {
  if (mode_ == READ) {
    CUSTOM_EXCEPTION("LCM_File_Handler: open mode error");
  }
  name_ = name;
  mode_ = WRITE;
  auto ptr_file = std::make_shared<lcm::LogFile>((name, "w"));
  if (!ptr_file->good()) {
    return false;
  }
  init(ptr_file);
}

LCM_File_Handler::LCM_File_Handler(const std::string& name, LCMFILE_MODE mode)
    : mode_(mode), name_(name) {
  std::shared_ptr<lcm::LogFile> ptr_file;
  if (mode_ == READ) {
    ptr_file.reset(new lcm::LogFile(name, "r"));
    if (ptr_file->good()) {
      init(ptr_file);
    } else {
      mode_ = DEFAULT;
    }
  } else if (mode_ == WRITE) {
    ptr_file.reset(new lcm::LogFile(name, "w"));
    if (ptr_file->good()) {
      init(ptr_file);
    } else {
      mode_ = DEFAULT;
    }
  } else {
  }
}

uint64_t PlanningLog_Reader::get_TotalSize() {
  if (mode_ == DEFAULT) {
    CUSTOM_EXCEPTION("Not initialized");
  }
  return log_content_.size();
}

const atd::protocol::MONITOR_MSG& PlanningLog_Reader::get_PlanningMessage(
    uint64_t index) const {
  return log_content_.at(index);
}

std::pair<PlanningLog_Reader::const_iterator,
          PlanningLog_Reader::const_iterator>
PlanningLog_Reader::get_PlanningMessage(uint64_t header, uint64_t tailer) {
  return {log_content_.cbegin() + header, log_content_.cbegin() + tailer};
}

void PlanningLog_Reader::init(std::shared_ptr<lcm::LogFile> ptr_file) {
  log_content_.clear();
  auto event_slice = ptr_file->readNextEvent();
  while (event_slice) {
    if (event_slice->channel == "PlanningLog") {
      atd::protocol::MONITOR_MSG tmp_msg;
      tmp_msg.ParseFromArray(event_slice->data, event_slice->datalen);
      log_content_.push_back(std::move(tmp_msg));
    }
    event_slice = ptr_file->readNextEvent();
  }
}

PlanningLog_Reader::PlanningLog_Reader(const std::string& name)
    : LCM_File_Handler(name, READ) {}

void PlanningLog_Writer::listening(const atd::protocol::MONITOR_MSG& msg) {
  lcm::LogEvent tmp_event;
  tmp_event.datalen = msg.ByteSize();
  if (!msg.SerializePartialToArray(tmp_event.data, tmp_event.datalen)) {
    return;
  }
  tmp_event.channel = "PlanningLog";
  tmp_event.eventnum = COUNTER(this->name_);
  ptr_file_->writeEvent(&tmp_event);
}

void PlanningLog_Writer::init(std::shared_ptr<lcm::LogFile> ptr_file) {
  ptr_file_ = ptr_file;
}

PlanningLog_Writer::PlanningLog_Writer(const std::string& name)
    : LCM_File_Handler(name, WRITE) {}
