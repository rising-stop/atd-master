#include "net_work.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace Themis {
namespace Transmit {

void ListeningService::set_SockType(int sock_type) { sock_type_ = sock_type; }
void ListeningService::set_Protocol(int protocol) { protocol_ = protocol; }
void ListeningService::set_Domain(int domain) { domain_ = domain; }
void ListeningService::set_Port(const std::string& port) { port_ = port; }

void ListeningService::set_NonBlocking(int sockfd) const {
  int opts;
  opts = fcntl(sockfd, F_GETFL);
  if (opts < 0) {
    //  exception here
  }
  opts = (opts | O_NONBLOCK);
  if (fcntl(sockfd, F_SETFL, opts) < 0) {
    //  exception here
  }
}

void ListeningService::config_Service() {
  addrinfo hints;
  addrinfo* result = nullptr;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = domain_;
  hints.ai_socktype = sock_type_;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  auto err_code = getaddrinfo(nullptr, port_.c_str(), &hints, &result);
  if (err_code != 0) {
    errno_Handler(err_code);
  }

  addrinfo* rp = nullptr;
  int sfd = 0;
  for (rp = result; rp != nullptr; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) continue;
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) break;
    close(sfd);
  }

  if (rp == nullptr) {
    errno_Handler(errno, "No address succeeded");
  }

  listen_fd_ = sfd;
  listen(listen_fd_, back_log_);
  flag_is_configured_ = true;
  return;
}

void ListeningService::errno_Handler(int err_code,
                                     const std::string& str) const {
  fprintf(stderr, "custom information: %s, getaddrinfo: %s\n", str.c_str(),
          (err_code));
}

ListeningService::ListeningService(const std::string& port, int domain,
                                   int sock_type, int protocol,
                                   int waiting_time, int back_log)
    : port_(port),
      domain_(domain),
      sock_type_(sock_type),
      protocol_(protocol),
      back_log_(back_log) {}

void TCP_Server::launch_Server() {
  if (ptr_thread_msg_server_->joinable()) {
    close_Server();
  }
  flag_is_listening_ = true;
  ptr_thread_msg_server_.reset(new std::thread(&spin, this));
}

void TCP_Server::close_Server() {
  flag_is_listening_ = false;
  if (ptr_thread_msg_server_->joinable()) {
    ptr_thread_msg_server_->join();
  }
}

void TCP_Server::spin() {
  while (flag_is_listening_) {
  }
}

TCP_Server::~TCP_Server() {
  close_Server();
  client_list_.for_each_value(
      [](const std::string& name, const AddressInfo& info) {
        close(info.assigned_socket_fd_);
      });
}

}  // namespace Transmit
}  // namespace Themis