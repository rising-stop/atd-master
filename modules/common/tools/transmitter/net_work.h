#pragma once
#include <netdb.h>
#include <sys/socket.h>

#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "message_package.h"
#include "modules/common/utility/thread_safe.h"

namespace Themis {
namespace Transmit {

struct AddressInfo {
  std::string name_;
  std::shared_ptr<sockaddr_in> addr_;
  int assigned_socket_fd_ = 0;
};

class ListeningService {
 public:
  virtual void launch_ListeningService() = 0;
  virtual void close_ListeningService() = 0;

  void set_BufferSize(int);
  void set_SockType(int);
  void set_Protocol(int);
  void set_Domain(int);
  void set_Port(const std::string&);

 protected:
  void config_Service();
  void set_NonBlocking(int) const;
  void errno_Handler(int, const std::string& message = "") const;

 protected:
  bool flag_is_configured_ = false;
  int listen_fd_ = 0;

  std::string port_;
  int domain_ = AF_UNSPEC;
  int sock_type_ = 0;
  int protocol_ = 0;
  int back_log_ = 30;

 public:
  ListeningService() = default;
  ListeningService(const std::string& port, int domain, int sock_type,
                   int protocol, int waiting_time, int back_log);
  virtual ~ListeningService() = default;
};

class UDP_Server : public ListeningService {
  virtual void launch_ListeningService() override;
  virtual void close_ListeningService() override;
};

class TCP_Server : public ListeningService {
 public:
  virtual void launch_ListeningService() override;
  virtual void close_ListeningService() override;

 private:
  void launch_Server();
  void spin();
  void close_Server();

 private:
  std::function<bool(const AddressInfo&)> ip_filter_ = [](const AddressInfo&) {
    return true;
  };
  Themis::utility::ThreadSafe_HashMap<std::string, AddressInfo> client_list_;
  Themis::utility::ThreadSafe_Deque<TransmitMessage> message_buffer_;
  int buffer_size_ = 0;
  volatile bool flag_is_listening_ = false;

  std::shared_ptr<std::thread> ptr_thread_msg_server_;

 public:
  TCP_Server() = default;
  TCP_Server(const std::string& port, int domain, int protocol, int buffer_size,
             int waiting_time, int back_log);
  virtual ~TCP_Server();
};

}  // namespace Transmit
}  // namespace Themis