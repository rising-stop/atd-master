#pragma once

#include <google/protobuf/message_lite.h>

#include <lcm/lcm-cpp.hpp>
#include <thread>

#include "modules/common/utility/thread_safe.h"
#include "modules/common/utility/utility.h"

namespace atd {
namespace utility {

class LCM_Messages_Adapter {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() const noexcept = 0;
  virtual bool msg_Decode(const lcm::ReceiveBuffer*) noexcept = 0;

 public:
  LCM_Messages_Adapter() = default;
  virtual ~LCM_Messages_Adapter() = default;
};

template <typename LCM_TYPE>
class LCM_Messages : public LCM_Messages_Adapter, public LCM_TYPE {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() const noexcept override;
  virtual bool msg_Decode(const lcm::ReceiveBuffer*) noexcept override;

 public:
  explicit LCM_Messages() = default;
  virtual ~LCM_Messages() = default;
};

template <typename PROTO_TYPE>
class Proto_Messages : public LCM_Messages_Adapter, public PROTO_TYPE {
 public:
  virtual std::shared_ptr<lcm::ReceiveBuffer> msg_Encode() const noexcept override;
  virtual bool msg_Decode(const lcm::ReceiveBuffer*) noexcept override;

 public:
  explicit Proto_Messages() = default;
  virtual ~Proto_Messages() = default;
};

enum LCM_MODE : int { SENDER = 0, READER = 1};
template <typename MSG_TYPE>
class LCM_Proxy {
 public:
  typedef typename std::enable_if<
      std::is_base_of<LCM_Messages_Adapter, MSG_TYPE>::value, MSG_TYPE>::type
      BASE_MSG_TYPE;

  LCM_MODE lcm_Mode() const;
  void publish(const BASE_MSG_TYPE&);
  bool subscribe(BASE_MSG_TYPE&);

 protected:
  bool is_Good() const;
  void catch_LCM_buffers(const lcm::ReceiveBuffer*, const std::string&);
  void receiver_spin();
  void publisher_spin();

 private:
  lcm::LCM* ptr_lcm_;
  const std::string channel_;
  const LCM_MODE mode_;

  volatile bool flag_shutdown_spin_ = false;
  std::thread* spin_handler_ = nullptr;
  ThreadSafe_Deque<BASE_MSG_TYPE> buffers_;
  const int buffer_size_;

 public:
  LCM_Proxy() = delete;
  LCM_Proxy(LCM_MODE, const std::string&, int ttl = 1, int buffer_size = 5);
  LCM_Proxy(const LCM_Proxy&) = delete;
  LCM_Proxy(LCM_Proxy&&) = delete;
  ~LCM_Proxy();
};

}  // namespace utility
}  // namespace atd

#include "lcm_dispatcher.tcc"