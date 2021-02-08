#include "message_package.h"

#include <stdlib.h>

#include <cstring>
// #include <iostream>
#include <string>
// #include <thread>

namespace Themis {
namespace Transmit {

/************************************************
 *********** class TransmitMessage **************
 ************************************************/

TransmitMessage::TransmitMessage(const TransmitMessage& instance) {
  if (instance.ptr_data_ != nullptr) {
    set_DataPointer(instance.ptr_data_, instance.size_);
  }
}

TransmitMessage::TransmitMessage(TransmitMessage&& instance)
    : ptr_data_(instance.ptr_data_), size_(instance.size_) {
  instance.ptr_data_ = nullptr;
}

TransmitMessage::TransmitMessage(void* ptr_data, MSG_SIZE size) {
  set_DataPointer(ptr_data, size);
}

TransmitMessage::~TransmitMessage() { free(ptr_data_); }

void TransmitMessage::set_DataPointer(const void* ptr_data, MSG_SIZE size) {
  if (ptr_data == nullptr) {
    return;
  }
  if (ptr_data_ != nullptr) {
    free(ptr_data_);
  }
  try {
    ptr_data_ = malloc(size);
  } catch (...) {
    //  exception here
  }

  memcpy(ptr_data_, ptr_data, size);
  size_ = size;
}

const void* TransmitMessage::data() const { return ptr_data_; }

MSG_SIZE TransmitMessage::size() const { return size_; }

/************************************************
 ********* class ProtocolCertificator ***********
 ************************************************/

ProtocolCertificator::ProtocolCertificator() {
  protocol_.reset(new package_protocol());
  read_ProtocolSerialNo();
}

MSG_SIZE ProtocolCertificator::get_ProtocolLength() const {
  return sizeof(package_protocol);
}

void ProtocolCertificator::read_ProtocolSerialNo() {
  if (strlen(TRANSMIT_MSG_PROTOCOL_HEAD_CODE) < 8) {
    //  exceptions here
  }
  if (strlen(TRANSMIT_MSG_PROTOCOL_SERIAL_NO) < 8) {
    //  exceptions here
  }
  const char* tmp_char = TRANSMIT_MSG_PROTOCOL_HEAD_CODE;
  const char* tmp_no = TRANSMIT_MSG_PROTOCOL_SERIAL_NO;
  for (auto i = 0; i < 8; ++i) {
    protocol_->head_[i] = tmp_char[i];
    protocol_->serial_no_[i] = tmp_no[i];
  }
}

void ProtocolCertificator::register_Protocol(MSG_SIZE msg_size) {
  protocol_.reset(new package_protocol());
  read_ProtocolSerialNo();
  protocol_->size_ = msg_size;
  protocol_->msg_id_ = ++msg_counter_;
  protocol_->msg_package_no_ = 0;
}

const void* ProtocolCertificator::get_MsgProtocol() {
  if (protocol_ == nullptr) {
    // exception here
  }
  ++protocol_->msg_package_no_;
  return protocol_.get();
}

bool ProtocolCertificator::is_NewMsg() const {
  return protocol_->msg_package_no_ == 1U;
}

bool ProtocolCertificator::is_ValidMsg(const void* ptr_msg, MSG_SIZE msg_size) {
  if (msg_size <= get_ProtocolLength()) {
    return false;
  }
  package_protocol* incoming_protocol = (package_protocol*)ptr_msg;
  const char* tmp_char = TRANSMIT_MSG_PROTOCOL_HEAD_CODE;
  const char* tmp_no = TRANSMIT_MSG_PROTOCOL_SERIAL_NO;
  for (auto i = 0; i < 8; ++i) {
    if (tmp_char[i] != incoming_protocol->head_[i]) {
      return false;
    }
    if (tmp_no[i] != incoming_protocol->serial_no_[i]) {
      return false;
    }
  }

  if (incoming_protocol->msg_package_no_ == 1U) {
    *protocol_ = *incoming_protocol;
  } else {
    if (protocol_->msg_id_ != incoming_protocol->msg_id_ ||
        protocol_->msg_package_no_ != incoming_protocol->msg_package_no_ - 1U) {
      return false;
    }
    ++protocol_->msg_package_no_;
  }

  return true;
}

MSG_SIZE ProtocolCertificator::get_MessageSize() const {
  return protocol_->size_;
}

/************************************************
 ********** class EncapsuedMessage **************
 ************************************************/

EncapsuedMessage::EncapsuedMessage(const EncapsuedMessage& instance)
    : consumed_size_(instance.consumed_size_) {
  base_msg_.reset(new TransmitMessage(*instance.base_msg_));
  ptr_data_seg_ = malloc(instance.consumed_size_);
  memcpy(ptr_data_seg_, instance.ptr_data_seg_, instance.consumed_size_);
}

EncapsuedMessage::EncapsuedMessage(EncapsuedMessage&& instance)
    : ptr_data_seg_(instance.ptr_data_seg_) {
  instance.ptr_data_seg_ = nullptr;
}

EncapsuedMessage::~EncapsuedMessage() { free(ptr_data_seg_); }

void EncapsuedMessage::register_Message(const void* ptr_data, MSG_SIZE size) {
  set_DataPointer(ptr_data, size);
  register_Protocol(size);
  consumed_size_ = 0UL;
}

void EncapsuedMessage::set_DataPointer(const void* ptr_data, MSG_SIZE size) {
  correlate_inner(ptr_data, size);
}

const void* EncapsuedMessage::data() const { return base_msg_->data(); }

MSG_SIZE EncapsuedMessage::size() const { return base_msg_->size(); }

void EncapsuedMessage::correlate_inner(const void* msg_data,
                                       MSG_SIZE msg_size) {
  if (msg_data == nullptr) {
    //  exception here
  }
  if (ptr_data_seg_ != nullptr) {
    free(ptr_data_seg_);
  }
  if (base_msg_ == nullptr) {
    base_msg_.reset(new TransmitMessage());
  }
  base_msg_->set_DataPointer(msg_data, msg_size);
  consumed_size_ = 0U;
}

void EncapsuedMessage::add_Data(const void* ptr_data, MSG_SIZE size) {
  if (ptr_data == nullptr || size < get_ProtocolLength()) {
    //  exception here
  }
  MSG_SIZE msg_length = size - get_ProtocolLength();
  if (is_ValidMsg(ptr_data, size)) {
    if (is_NewMsg()) {
      if (ptr_data_seg_ != nullptr) {
        free(ptr_data_seg_);
      }
      try {
        ptr_data_seg_ = malloc(get_MessageSize());
      } catch (...) {
        // exception here
      }
      memcpy(ptr_data_seg_, (char*)ptr_data + get_ProtocolLength(), msg_length);
      consumed_size_ = msg_length;
    } else {
      memcpy((char*)ptr_data_seg_ + consumed_size_,
             (char*)ptr_data + get_ProtocolLength(), msg_length);
      consumed_size_ += msg_length;
    }
    if (is_Done()) {
      base_msg_.reset(new TransmitMessage(ptr_data_seg_, consumed_size_));
    }
  }
}

bool EncapsuedMessage::is_Done() const {
  if (consumed_size_ > get_MessageSize()) {
    // exception here
  }
  if (consumed_size_ == get_MessageSize()) {
    return true;
  }
  return false;
}

MSG_SIZE EncapsuedMessage::size_segment() const {
  return BUFFER_SIZE > base_msg_->size() + get_ProtocolLength()
             ? (base_msg_->size() - consumed_size_) + get_ProtocolLength()
             : BUFFER_SIZE;
}

std::pair<const void*, MSG_SIZE> EncapsuedMessage::generate_PackagedData() {
  if (base_msg_ == nullptr || protocol_ == nullptr) {
    // exception here
  }
  if (ptr_data_seg_ != nullptr) {
    free(ptr_data_seg_);
  }
  uint64_t tmp_msg_size = size_segment();
  try {
    ptr_data_seg_ = malloc(tmp_msg_size);
  } catch (...) {
    //  exception here
  }

  memcpy(ptr_data_seg_, get_MsgProtocol(), get_ProtocolLength());
  memcpy((char*)ptr_data_seg_ + get_ProtocolLength(),
         (char*)base_msg_->data() + consumed_size_,
         tmp_msg_size - get_ProtocolLength());
  return {ptr_data_seg_, tmp_msg_size};
}

void EncapsuedMessage::consumed(uint32_t cosum_no) {
  if (cosum_no < get_ProtocolLength()) {
    return;
  }
  if (cosum_no - get_ProtocolLength() > base_msg_->size() - consumed_size_) {
    // exception here
  }
  consumed_size_ += cosum_no - get_ProtocolLength();
}

}  // namespace Transmit
}  // namespace Themis

std::string char_convert(const void* ptr, uint64_t size) {
  std::string res;
  for (auto i = 0; i < size; ++i) {
    res.push_back(((char*)ptr)[i]);
  }
  return res;
}

/* Test Case Here for Message Package */

// int main(int args, char* argv[]) {
//   Themis::Transmit::EncapsuedMessage msg_publisher, msg_receiver;

//   const char* test_ptr = "test_message";
//   int test_ptr_len = strlen(test_ptr);
//   // printf("test case info: \ntest case content: %s\ntest case size:
//   %d\n\n",
//   //        test_ptr, test_ptr_len);

//   Themis::Transmit::TransmitMessage test_message;
//   test_message.set_DataPointer(test_ptr, test_ptr_len);

//   msg_publisher.register_Message(test_message.data(), test_message.size());

//   printf(
//       "test message info \ntest message size %u\ntest message contains: "
//       "%s\n\n",
//       test_message.size(), (char*)test_message.data());

//   uint64_t publish_size = 36UL;

//   while (1) {
//     if (msg_publisher.is_Done()) {
//       break;
//     }
//     auto published_msg = msg_publisher.generate_PackagedData();
//     msg_publisher.consumed(std::min(publish_size, published_msg.second));
//     std::cout
//         << "published msg info: "
//         << (char*)published_msg.first +
//                sizeof(Themis::Transmit::ProtocolCertificator::package_protocol)
//         << "\nmessage size = " << published_msg.second
//         << "\nprotocol id = " << msg_publisher.protocol_->msg_id_
//         << "\nprotocol package no = "
//         << msg_publisher.protocol_->msg_package_no_
//         << "\nprotocol size = " << msg_publisher.protocol_->size_ <<
//         std::endl;
//     printf("\n");

//     msg_receiver.add_Data(published_msg.first,
//                           std::min(publish_size, published_msg.second));
//     if (msg_receiver.is_Done()) {
//       std::cout << "receving done, msg: "
//                 << char_convert(msg_receiver.data(), msg_receiver.size())
//                 << std::endl;
//     } else {
//       // std::cout << "received msg info: "
//       //           << char_convert(msg_receiver.data(), msg_receiver.size())
//       //           << "\nmessage size = " << published_msg.second
//       //           << "\nprotocol id = " << msg_publisher.protocol_->msg_id_
//       //           << "\nprotocol package no = "
//       //           << msg_publisher.protocol_->msg_package_no_
//       //           << "\nprotocol size = " << msg_publisher.protocol_->size_
//       //           << std::endl;
//     }

//     printf("\n");
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   }

//   printf("received message: %s\n", (char*)msg_receiver.data());
//   return EXIT_SUCCESS;
// }