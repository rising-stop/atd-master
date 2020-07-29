#include "lcm_dispatcher.h"

namespace atd {
namespace utility {

template <typename LCM_TYPE>
std::shared_ptr<lcm::ReceiveBuffer> LCM_Messages<LCM_TYPE>::msg_Encode() {
  auto ptr_encoded_msg = std::make_shared<lcm::ReceiveBuffer>();
  this->encode(ptr_encoded_msg->data, 0, this->getEncodedSize());
  ptr_encoded_msg->data_size = this->getEncodedSize();
  return ptr_encoded_msg;
}

template <typename LCM_TYPE>
void LCM_Messages<LCM_TYPE>::msg_Decode(lcm::ReceiveBuffer* buffer) {
  this->decode(buffer->data, 0, this->getEncodedSize());
  buffer->data_size = this->getEncodedSize();
}

}  // namespace utility
}  // namespace atd