#pragma once

#include <thread>

#include "google/protobuf/message_lite.h"
#include "lcm/lcm-cpp.hpp"
#include "modules/common/utility/ipc/thread_safe.h"
#include "modules/common/utility/utility.h"

namespace atd {
namespace utility {

class LCM_Messages_Adapter {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() = 0;
  virtual void msg_Decode(lcm::ReceiveBuffer*) = 0;

 public:
  LCM_Messages_Adapter() = default;
  virtual ~LCM_Messages_Adapter() = default;
};

template <typename LCM_TYPE>
class LCM_Messages : public LCM_Messages_Adapter, public LCM_TYPE {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() override;
  virtual void msg_Decode(lcm::ReceiveBuffer*) override;

 public:
  explicit LCM_Messages() = default;
  virtual ~LCM_Messages() = default;
};

class ProtoLite_Messages : public LCM_Messages_Adapter {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() override;
  virtual void msg_Decode(lcm::ReceiveBuffer*) override;

 private:
  google::protobuf::MessageLite& ref_msg_;

 public:
  explicit ProtoLite_Messages(google::protobuf::MessageLite&);
  virtual ~ProtoLite_Messages() = default;
};

class LCM_Proxy {
 public:
  enum LCM_MODE : int { SENDER = 0, READER = 1, PARSER = 2 };

  LCM_MODE lcm_Mode() const;
  bool publish(LCM_Messages_Adapter&);
  bool subscribe(LCM_Messages_Adapter&);

 protected:
  bool is_Good() const;
  void lcm_Handler(const lcm::ReceiveBuffer*, const std::string&);
  void spin();

 private:
  lcm::LCM* ptr_lcm_;
  const std::string channel_;
  const LCM_MODE mode_;

  volatile bool flag_shutdown_spin_ = false;
  std::thread* spin_handler_;
  ThreadSafe_Deque<lcm::ReceiveBuffer> buffers_;
  const int buffer_size_;

 public:
  LCM_Proxy() = delete;
  LCM_Proxy(LCM_MODE, const std::string&, int ttl = 1, int buffer_size = 3);
  LCM_Proxy(const LCM_Proxy&) = delete;
  LCM_Proxy(LCM_Proxy&&) = delete;
  ~LCM_Proxy();
};

}  // namespace utility
}  // namespace atd

#include "lcm_dispatcher.tcc"