#include "lcm_dispatcher.h"

namespace atd {
namespace utility {

template <typename LCM_TYPE>
std::shared_ptr<lcm::ReceiveBuffer> LCM_Messages<LCM_TYPE>::msg_Encode() {
  auto ptr_encoded_msg = std::make_shared<lcm::ReceiveBuffer>();
  ptr_encoded_msg->data_size = this->getEncodedSize();
  this->encode(ptr_encoded_msg->data, 0, ptr_encoded_msg->data_size);
  return ptr_encoded_msg;
}

template <typename LCM_TYPE>
bool LCM_Messages<LCM_TYPE>::msg_Decode(lcm::ReceiveBuffer* buffer) {
  this->decode(buffer->data, 0, this->getEncodedSize());
  buffer->data_size = this->getEncodedSize();
}

template <typename PROTO_TYPE>
std::shared_ptr<lcm::ReceiveBuffer>
ProtoLite_Messages<PROTO_TYPE>::msg_Encode() {
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
bool ProtoLite_Messages<PROTO_TYPE>::msg_Decode(lcm::ReceiveBuffer* buffer) {
  if (this->ParseFromArray(buffer->data, buffer->data_size)) {
    return true;
  }
  return false;
}

}  // namespace utility
}  // namespace atd