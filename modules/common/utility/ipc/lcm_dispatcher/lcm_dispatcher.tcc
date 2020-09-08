#include "lcm_dispatcher.h"

namespace atd {
namespace utility {

template <typename LCM_TYPE>
std::shared_ptr<lcm::ReceiveBuffer> LCM_Messages<LCM_TYPE>::msg_Encode()
    const noexcept {
  auto ptr_encoded_msg = std::make_shared<lcm::ReceiveBuffer>();
  ptr_encoded_msg->data_size = this->getEncodedSize();
  this->encode(ptr_encoded_msg->data, 0, ptr_encoded_msg->data_size);
  return ptr_encoded_msg;
}

template <typename LCM_TYPE>
bool LCM_Messages<LCM_TYPE>::msg_Decode(
    const lcm::ReceiveBuffer* buffer) noexcept {
  if (0 <= this->decode(buffer->data, 0, buffer->data_size)) {
    return true;
  }
  return false;
}

template <typename PROTO_TYPE>
std::shared_ptr<lcm::ReceiveBuffer> Proto_Messages<PROTO_TYPE>::msg_Encode()
    const noexcept {
  auto ptr_encoded_msg = std::make_shared<lcm::ReceiveBuffer>();
  ptr_encoded_msg->data_size = this->ByteSize();
  ptr_encoded_msg->data = malloc(ptr_encoded_msg->data_size);
  if (!this->SerializeToArray(ptr_encoded_msg->data,
                              ptr_encoded_msg->data_size)) {
    return nullptr;
  }
  return ptr_encoded_msg;
}

template <typename PROTO_TYPE>
bool Proto_Messages<PROTO_TYPE>::msg_Decode(
    const lcm::ReceiveBuffer* buffer) noexcept {
  if (this->ParseFromArray(buffer->data, buffer->data_size)) {
    return true;
  }
  return false;
}

template <typename MSG_TYPE>
LCM_MODE LCM_Proxy<MSG_TYPE>::lcm_Mode() const {
  return mode_;
}

template <typename MSG_TYPE>
bool LCM_Proxy<MSG_TYPE>::is_Good() const {
  return ptr_lcm_->good();
}

template <typename MSG_TYPE>
void LCM_Proxy<MSG_TYPE>::publish(const BASE_MSG_TYPE& adapter) {
  if (mode_ != SENDER) {
    // throw LCMException(LCMException::MODE_ERROR,
    //                    "LCM_Proxy mode error, current mode is SENDER");
    CUSTOM_EXCEPTION("LCM_Proxy mode error, current mode is SENDER");
  }
  if (!is_Good()) {
    return;
  }
  if (!spin_handler_) {
    spin_handler_ = new std::thread(&LCM_Proxy::publisher_spin, this);
  }
  buffers_.push_back_with_limits(adapter, buffer_size_);
}

template <typename MSG_TYPE>
void LCM_Proxy<MSG_TYPE>::publisher_spin() {
  while (!flag_shutdown_spin_) {
    auto adapter = buffers_.wait_pop_front();
    auto lcm_rbuf = adapter->msg_Encode();
    if (!lcm_rbuf) {
      continue;
    }
    ptr_lcm_->publish(channel_, lcm_rbuf->data, lcm_rbuf->data_size);
  }
}

template <typename MSG_TYPE>
bool LCM_Proxy<MSG_TYPE>::subscribe(BASE_MSG_TYPE& adapter) {
  if (mode_ != READER) {
    // throw LCMException(LCMException::MODE_ERROR,
    //                    "LCM_Proxy mode error, current mode is READER");
    CUSTOM_EXCEPTION("LCM_Proxy mode error, current mode is READER");
  }
  if (!is_Good()) {
    return false;
  }
  if (!spin_handler_) {
    spin_handler_ = new std::thread(&LCM_Proxy::receiver_spin, this);
  }
  if (buffers_.try_pop_front(adapter)) {
    return true;
  }
  return false;
}

template <typename MSG_TYPE>
void LCM_Proxy<MSG_TYPE>::catch_LCM_buffers(const lcm::ReceiveBuffer* rbuf,
                                            const std::string& channel) {
  BASE_MSG_TYPE adapter;
  if (!(adapter.msg_Decode(rbuf))) {
    return;
  }
  buffers_.push_back_with_limits(adapter, buffer_size_);
}

template <typename MSG_TYPE>
void LCM_Proxy<MSG_TYPE>::receiver_spin() {
  ptr_lcm_->subscribe(channel_, &LCM_Proxy::catch_LCM_buffers, this);
  while (!flag_shutdown_spin_) {
    if (ptr_lcm_->good()) {
      ptr_lcm_->handle();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      if (init()) {
        ptr_lcm_->subscribe(channel_, &LCM_Proxy::catch_LCM_buffers, this);
      }
    }
  }
}

template <typename MSG_TYPE>
bool LCM_Proxy<MSG_TYPE>::init() {
  if (ptr_lcm_) {
    delete ptr_lcm_;
  }
  std::string lcm_init_param = "udpm://";
  const char* udpm = "239.255.76.67:7667";
  lcm_init_param.append(udpm);
  lcm_init_param.append("?ttl=");
  lcm_init_param.append(std::to_string(ttl_));
  ptr_lcm_ = new lcm::LCM(lcm_init_param);
  return ptr_lcm_->good();
}

template <typename MSG_TYPE>
LCM_Proxy<MSG_TYPE>::LCM_Proxy(LCM_MODE mode, const std::string& channel,
                               int ttl, int buffer_size)
    : mode_(mode), channel_(channel), ttl_(ttl), buffer_size_(buffer_size) {
  init();
  switch (mode_) {
    case SENDER:
      break;
    case READER:
      break;
    default:
      // throw LCMException(LCMException::UNKNOW_MODE,
      //                    "this mode is not registered yet");
      CUSTOM_EXCEPTION("this mode is not registered yet");
  }
}

template <typename MSG_TYPE>
LCM_Proxy<MSG_TYPE>::~LCM_Proxy() {
  if (spin_handler_) {
    flag_shutdown_spin_ = true;
    while (spin_handler_->joinable()) {
      spin_handler_->join();
    }
  }
}

}  // namespace utility
}  // namespace atd