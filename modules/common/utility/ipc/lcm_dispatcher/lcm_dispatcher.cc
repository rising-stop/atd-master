#include "lcm_dispatcher.h"

namespace atd {
namespace utility {

std::shared_ptr<lcm::ReceiveBuffer> ProtoLite_Messages::msg_Encode() {
  auto ptr_buf = std::make_shared<lcm::ReceiveBuffer>();
  ptr_buf->data_size = ref_msg_.ByteSize();
  ptr_buf->data = malloc(ptr_buf->data_size);
  if (!ref_msg_.SerializeToArray(ptr_buf->data, ptr_buf->data_size)) {
    throw LCMException(LCMException::PROTOMSG_SERIALIZE_FAIL,
                       "proto msg encode fail");
  }
  return ptr_buf;
}

void ProtoLite_Messages::msg_Decode(lcm::ReceiveBuffer* lcm_buf) {
  if (!ref_msg_.ParseFromArray(lcm_buf->data, lcm_buf->data_size)) {
    throw LCMException(LCMException::PROTOMSG_PARSE_FAIL,
                       "proto msg decode fail");
  }
}

ProtoLite_Messages::ProtoLite_Messages(google::protobuf::MessageLite& proto_msg)
    : ref_msg_(proto_msg) {}

LCM_Proxy::LCM_MODE LCM_Proxy::lcm_Mode() const { return mode_; }

bool LCM_Proxy::is_Good() const { return ptr_lcm_->good(); }

bool LCM_Proxy::publish(LCM_Messages_Adapter& adapter) {
  if (mode_ != SENDER) {
    throw LCMException(LCMException::MODE_ERROR,
                       "LCM_Proxy mode error, current mode is SENDER");
  }
  auto lcm_rbuf = adapter.msg_Encode();
  return ptr_lcm_->publish(channel_, lcm_rbuf->data, lcm_rbuf->data_size);
}

bool LCM_Proxy::subscribe(LCM_Messages_Adapter& adapter) {
  if (mode_ != READER) {
    throw LCMException(LCMException::MODE_ERROR,
                       "LCM_Proxy mode error, current mode is READER");
  }
  if (!spin_handler_) {
    spin_handler_ = new std::thread(&LCM_Proxy::spin, this);
  }
  lcm::ReceiveBuffer tmp_buffer;
  if (buffers_.try_pop_back(tmp_buffer)) {
    adapter.msg_Decode(&tmp_buffer);
    return true;
  }
  return false;
}

void LCM_Proxy::lcm_Handler(const lcm::ReceiveBuffer* rbuf,
                            const std::string& channel) {
  buffers_.push_back_with_limits(*rbuf, buffer_size_);
}

void LCM_Proxy::spin() {
  ptr_lcm_->subscribe(channel_, &LCM_Proxy::lcm_Handler, this);
  while (!flag_shutdown_spin_) {
    ptr_lcm_->handle();
  }
}

LCM_Proxy::LCM_Proxy(LCM_MODE mode, const std::string& channel, int ttl,
                     int buffer_size)
    : mode_(mode), channel_(channel), buffer_size_(buffer_size) {
  char* lcm_init_param;
  const char* udpm = "239.255.76.67:7667";
  CString::cstring_cat(lcm_init_param, "udpm://", udpm,
                       "?ttl=", std::to_string(ttl).c_str());
  ptr_lcm_ = new lcm::LCM(lcm_init_param);

  switch (mode_) {
    case SENDER:
      break;
    case READER:
      break;
    case PARSER:
      break;
    default:
      throw LCMException(LCMException::UNKNOW_MODE,
                         "this mode is not registered yet");
  }
}

LCM_Proxy::~LCM_Proxy() {
  if (spin_handler_) {
    flag_shutdown_spin_ = true;
    while (spin_handler_->joinable()) {
    }
  }
}

}  // namespace utility
}  // namespace atd