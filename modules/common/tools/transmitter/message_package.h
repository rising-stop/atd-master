#pragma once
#include <stdint.h>
#include <stdio.h>

#include <memory>

#include "net_work_config.h"

#define MSG_SIZE uint64_t
#define BUFFER_SIZE BUFSIZ

namespace Themis {
namespace Transmit {

class Common_Message {
 public:
  virtual const void* data() const = 0;
  virtual MSG_SIZE size() const = 0;
  virtual void set_DataPointer(const void* ptr_data, MSG_SIZE size) = 0;

 public:
  virtual ~Common_Message() = default;
};

/**
 * @brief basic message structure for a message which contains a void pointer
 * and its size;
 */
class TransmitMessage : public Common_Message {
 public:
  virtual const void* data() const override;
  virtual MSG_SIZE size() const override;
  virtual void set_DataPointer(const void* ptr_data, MSG_SIZE size) override;

 private:
  void* ptr_data_ = nullptr;
  MSG_SIZE size_ = 0;

 public:
  TransmitMessage() = default;
  TransmitMessage(const TransmitMessage&);
  TransmitMessage(TransmitMessage&&);
  TransmitMessage(void* ptr_data, MSG_SIZE size);
  virtual ~TransmitMessage();
};

class ProtocolCertificator {
 public:
  struct package_protocol {
    char head_[8];
    char serial_no_[8];
    uint32_t msg_id_ = 0;
    uint32_t msg_package_no_ = 0;
    uint64_t size_ = 0;
  };

  MSG_SIZE get_ProtocolLength() const;
  MSG_SIZE get_MessageSize() const;

  void register_Protocol(MSG_SIZE);
  const void* get_MsgProtocol();

  bool is_ValidMsg(const void*, MSG_SIZE);
  bool is_NewMsg() const;

  std::shared_ptr<package_protocol> protocol_;

 private:
  void read_ProtocolSerialNo();

  uint64_t msg_counter_ = 0;

 public:
  ProtocolCertificator();
  ~ProtocolCertificator() = default;
};

/**
 * @brief class EncapsuedMessage, a TransmitMessage class proxy, capable of
 * encapsuling message into multi-packages protocol;
 */
class EncapsuedMessage : public Common_Message, public ProtocolCertificator {
 public:
  virtual const void* data() const override;
  virtual MSG_SIZE size() const override;

 public:
  /* publish function */
  void register_Message(const void* ptr_data, MSG_SIZE size);
  std::pair<const void*, MSG_SIZE> generate_PackagedData();
  void consumed(uint32_t);

  /* receive function */
  void add_Data(const void* ptr_data, MSG_SIZE size);
  bool is_Done() const;

 private:
  virtual void set_DataPointer(const void* ptr_data, MSG_SIZE size) override;
  void correlate_inner(const void*, MSG_SIZE);

  const void* data_segment() const;
  MSG_SIZE size_segment() const;

 private:
  std::shared_ptr<TransmitMessage> base_msg_;

  void* ptr_data_seg_ = nullptr;
  MSG_SIZE consumed_size_ = 0;

 public:
  EncapsuedMessage() = default;
  EncapsuedMessage(const EncapsuedMessage&);
  EncapsuedMessage(EncapsuedMessage&&);
  virtual ~EncapsuedMessage();
};

}  // namespace Transmit
}  // namespace Themis