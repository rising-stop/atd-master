#include "log_file_handler.h"

bool LCM_File_Handler::open_LogFile(const std::string& name) {
  return file_init(name, READ);
}

bool LCM_File_Handler::create_LogFile(const std::string& name) {
  return file_init(name, WRITE);
}

LCMFILE_MODE LCM_File_Handler::get_Mode() const { return mode_; }

const std::string& LCM_File_Handler::get_Name() const { return name_; }

bool LCM_File_Handler::file_init(const std::string& name, LCMFILE_MODE mode) {
  switch (mode) {
    case WRITE:
      if (mode_ == READ) {
        CUSTOM_EXCEPTION("LCM_File_Handler: mode error");
      }
      name_ = name;
      mode_ = WRITE;
      ptr_file_.reset(new lcm::LogFile(name_, "w"));
      if (!ptr_file_->good()) {
        name_.clear();
        mode_ = DEFAULT;
        return false;
      }
      break;
    case READ:
      if (mode_ == WRITE) {
        CUSTOM_EXCEPTION("LCM_File_Handler: mode error");
      }
      name_ = name;
      mode_ = READ;
      ptr_file_.reset(new lcm::LogFile(name_, "r"));
      if (!ptr_file_->good()) {
        name_.clear();
        mode_ = DEFAULT;
        return false;
      }
      break;
    case DEFAULT:
      return false;
    default:
      CUSTOM_EXCEPTION("mode error, static_cast<int>(mode): %d",
                       static_cast<int>(mode));
  }
  if (!init()) {
    return false;
  }
  flag_is_initalized_ = true;
  return true;
}

LCM_File_Handler::LCM_File_Handler(const std::string& name, LCMFILE_MODE mode) {
  file_init(name, mode);
}

bool LCM_File_Handler::good() const {
  if (flag_is_initalized_) {
    if (mode_ != DEFAULT) {
      if (ptr_file_->good()) {
        return true;
      }
    }
  }
  return false;
}

uint32_t PlanningLog_Reader::get_TotalSize() {
  if (!this->good()) {
    return 0u;
  }
  return log_content_.size();
}

std::pair<PlanningLog_Reader::const_iterator,
          PlanningLog_Reader::const_iterator>
PlanningLog_Reader::get_PlanningMessage(uint32_t header,
                                        uint32_t tailer) const {
  if (!this->good()) {
    return {};
  }
  std::lock_guard<std::mutex> lock(content_mutex_);
  return {log_content_.cbegin() + header, log_content_.cbegin() + tailer};
}

void PlanningLog_Reader::preprocess_LogFile() {
  std::lock_guard<std::mutex> lock(content_mutex_);
  auto event_slice = ptr_file_->readNextEvent();
  while (event_slice) {
    if (event_slice->channel == "PlanningLog") {
      atd::protocol::MONITOR_MSG tmp_msg;
      tmp_msg.ParseFromArray(event_slice->data, event_slice->datalen);
      log_content_.push_back(std::move(tmp_msg));
    }
    event_slice = ptr_file_->readNextEvent();
  }
  flag_is_file_read_ = true;
}

bool PlanningLog_Reader::is_Done() const { return flag_is_file_read_; }

bool PlanningLog_Reader::init() {
  flag_is_file_read_ = false;
  log_content_.clear();
  if (thread_read_) {
    delete thread_read_;
  }
  thread_read_ = new std::thread(&PlanningLog_Reader::preprocess_LogFile, this);
  return true;
}

PlanningLog_Reader::PlanningLog_Reader(const std::string& name)
    : LCM_File_Handler(name, READ) {}

void PlanningLog_Writer::listening(const atd::protocol::MONITOR_MSG& msg) {
  if (!this->good()) {
    return;
  }
  lcm::LogEvent tmp_event;
  tmp_event.datalen = msg.ByteSize();
  if (!msg.SerializePartialToArray(tmp_event.data, tmp_event.datalen)) {
    return;
  }
  tmp_event.channel = "PlanningLog";
  tmp_event.eventnum = COUNTER(this->name_);
  ptr_file_->writeEvent(&tmp_event);
}

bool PlanningLog_Writer::init() { return true; }

PlanningLog_Writer::PlanningLog_Writer(const std::string& name)
    : LCM_File_Handler(name, WRITE) {}

/**
 * @brief defination of class DataSeg4LCMLogger
 */
bool DataSeg4LCMLogger::update() { return inner_update(); }

void DataSeg4LCMLogger::active(const std::string& name) {
  file_reader_.open_LogFile(name);
}

bool DataSeg4LCMLogger::inner_update() {
  if (!file_reader_.is_Done()) {
    return false;
  }
  return file_reader_.good();
}

std::pair<PlanningLog_Reader::const_iterator,
          PlanningLog_Reader::const_iterator>
DataSeg4LCMLogger::get_PlanningMessage(uint32_t head, uint32_t tail) const {
  return file_reader_.get_PlanningMessage(head, tail);
}
