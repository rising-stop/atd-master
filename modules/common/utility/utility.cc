#include "utility.h"

namespace atd {
namespace utility {

std::mutex Singleton::register_lock_;
std::unordered_map<Singleton::SGLTN_ID, Singleton *> Singleton::registry_;

FinateStateMachine::State::State(const std::string &id) { set_ID(id); }

void FinateStateMachine::State::set_ID(const std::string &id) { id_ = id; }

const std::string &FinateStateMachine::State::get_ID() const { return id_; }

void FinateStateMachine::State::register_Init(std::function<void()> func) {
  if (!is_registered_init_) {
    is_registered_init_ = true;
    init_ = func;
  }
}
void FinateStateMachine::State::register_Step(
    std::function<std::string()> func) {
  if (!is_registered_step_) {
    is_registered_step_ = true;
    step_ = func;
  }
}
void FinateStateMachine::State::register_Exit(std::function<void()> func) {
  if (!is_registered_exit_) {
    is_registered_exit_ = true;
    exit_ = func;
  }
}

void FinateStateMachine::State::init() {
  if (is_registered_init_) {
    init_();
  }
}

std::string FinateStateMachine::State::step() {
  if (is_registered_step_) {
    return step_();
  }
  return {};
}
void FinateStateMachine::State::exit() {
  if (is_registered_exit_) {
    exit_();
  }
}

FinateStateMachine::State *FinateStateMachine::add_State(
    const std::string &id) {
  State *ptr_state = new State(id);
  std::string new_id = id;
  state_map_.insert({new_id, ptr_state});
  return ptr_state;
}

void FinateStateMachine::set_InitState(const std::string &id) {
  init_state_ = id;
}

const std::string &FinateStateMachine::get_CurrentState() const {
  return current_state_->get_ID();
}

void FinateStateMachine::run() {
  static bool once_flag = true;
  if (once_flag) {
    once_flag = false;
    current_state_ = state_map_.at(init_state_);
    current_state_->init();
  }
  static std::string current_id = init_state_;
  auto next_state = current_state_->step();
  if (next_state != current_id) {
    current_state_->exit();
    current_state_ = state_map_.at(next_state);
    current_state_->init();
    current_id = current_state_->get_ID();
  }
}

}  // namespace utility
}  // namespace atd