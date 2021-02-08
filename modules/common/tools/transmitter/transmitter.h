#pragma once

#include "net_work.h"

namespace Themis {
namespace Transmit {
/**
 * @brief basic configuration for transmitter
 * @note it may be inherited by other subclass for unique configuration
 */
struct Transmitter_Configurator {
  int waiting_time_ = 1;
  int buffer_size_ = 30;
  int back_log_ = 30;
};

/**
 * @brief
 * base class of all transmitters, including udp, tcp and shared memory
 * transmitter defines a common interface of message transmission, common
 * interface for send message send_msg and receiving message recv_msg
 */
class Transmitter {
 protected:
  Transmitter() = default;
  virtual ~Transmitter() = default;

 public:
  Transmitter_Configurator* get_MutableConfigurator() { return &configurator_; }
  virtual int send_msg(const TransmitMessage& msg,
                       const std::string descripter) = 0;
  virtual int recv_msg(TransmitMessage& msg, const std::string descripter) = 0;

 protected:
  Transmitter_Configurator configurator_;
};

}  // namespace Transmit
}  // namespace Themis