#pragma once

#include <unordered_map>

#include "modules/common/utility/thread_safe.h"
#include "transmitter.h"

namespace Themis {
namespace Transmit {

class TCP_Transmitter : public Transmitter {
 public:
  virtual int send_msg(const TransmitMessage& msg,
                       const std::string descripter) override;
  virtual int recv_msg(TransmitMessage& msg,
                       const std::string descripter) override;

 protected:
  int send_msg_inner(const TransmitMessage& msg, const std::string descripter);
  int recv_msg_inner(TransmitMessage& msg, const std::string descripter);

 protected:
 private:
  void receiver_Spinner();

 protected:
  TCP_Transmitter() = default;
  virtual ~TCP_Transmitter() = default;
};

}  // namespace Transmit
}  // namespace Themis