/* created by liangle */

#pragma once

#include <stdlib.h>

#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exceptions.h"

namespace atd {
namespace utility {

/**
 * @class Factory
 * @brief Implements a Factory design pattern with Register and Create methods
 *
 * The objects created by this factory all implement the same interface
 * (namely, AbstractProduct). This design pattern is useful in settings where
 * multiple implementations of an interface are available, and one wishes to
 * defer the choice of the implementation in use.
 *
 * @param IdentifierType Type used for identifying the registered classes,
 * typically std::string.
 * @param AbstractProduct The interface implemented by the registered classes
 * @param ProductCreator Function returning a pointer to an instance of
 * the registered class
 * @param MapContainer Internal implementation of the function mapping
 * IdentifierType to ProductCreator, by default std::unordered_map
 */
template <typename IdentifierType, typename AbstractProduct,
          typename ProductCreator = AbstractProduct *(*)(),
          typename MapContainer =
              std::unordered_map<IdentifierType, ProductCreator>>
class Factory {
 public:
  /**
   * @brief Registers the class given by the creator function, linking it to id.
   * Registration must happen prior to calling CreateObject.
   * @param id Identifier of the class being registered
   * @param creator Function returning a pointer to an instance of
   * the registered class
   * @return True iff the key id is still available
   */
  bool product_register(const IdentifierType &id, ProductCreator creator) {
    return producers_.insert(std::make_pair(id, creator)).second;
  }

  /**
   * @brief Unregisters the class with the given identifier
   * @param id The identifier of the class to be unregistered
   */
  bool product_unregister(const IdentifierType &id) {
    return producers_.erase(id) == 1;
  }

  bool empty() const { return producers_.empty(); }

  /**
   * @brief Creates and transfers membership of an object of type matching id.
   * Need to register id before CreateObject is called. May return NULL
   * silently.
   * @param id The identifier of the class we which to instantiate
   * @param args the object construction arguments
   */
  template <typename... Args>
  std::unique_ptr<AbstractProduct> create_ObjectOrNull(
      const IdentifierType &id, Args &&... args) noexcept {
    auto id_iter = producers_.find(id);
    if (id_iter != producers_.end()) {
      return std::unique_ptr<AbstractProduct>(
          (id_iter->second)(std::forward<Args>(args)...));
    }
    return nullptr;
  }

  /**
   * @brief Creates and transfers membership of an object of type matching id.
   * Need to register id before CreateObject is called.
   * @param id The identifier of the class we which to instantiate
   * @param args the object construction arguments
   */
  template <typename... Args>
  std::unique_ptr<AbstractProduct> create_Object(const IdentifierType &id,
                                                 Args &&... args) {
    auto result = create_ObjectOrNull(id, args...);
    if (!result) {
      CUSTOM_EXCEPTION("not found ", id, " in factory");
    }
    return result;
  }

 private:
  MapContainer producers_;
};

/**
 * @class Singleton
 * @brief declare base class singleton
 */
class Singleton {
 public:
  typedef std::string SGLTN_ID;

  template <typename SINGLETON_DERIVED, typename... ARGS>
  static void try_register(ARGS &&... args) {
    SGLTN_ID id = typeid(SINGLETON_DERIVED).name();
    std::lock_guard<std::mutex> reg_lock(register_lock_);
    auto itr_registry = registry_.find(id);
    if (itr_registry == registry_.end()) {
      registry_.insert(std::make_pair(
          id, new SINGLETON_DERIVED(std::forward<ARGS>(args)...)));
    }
  }

  template <typename SINGLETON_DERIVED>
  static typename std::enable_if<
      std::is_base_of<Singleton, SINGLETON_DERIVED>::value,
      SINGLETON_DERIVED>::type *
  instance() {
    SGLTN_ID id = typeid(SINGLETON_DERIVED).name();
    if (registry_.find(id) == registry_.end()) {
      CUSTOM_EXCEPTION("Singleton ", id.c_str(), "not been registered yet.");
    }
    auto ptr = registry_[id];
    return dynamic_cast<SINGLETON_DERIVED *>(ptr);
  }

 private:
  static std::mutex register_lock_;
  static std::unordered_map<SGLTN_ID, Singleton *> registry_;

 protected:
  Singleton() = default;
  virtual ~Singleton() = default;
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
};

#define CLASS_SINGLETON atd::utility::Singleton

class CString {
 public:
  template <typename... ARG>
  static std::string cstring_cat(const char *, ARG &&...);

  template <typename... ARG>
  static int cstring_length_estimate(const char *, ARG &&...);

  template <typename T, typename... ARG>
  static int cstring_length_estimate(const T &, ARG &&...);

  template <typename T>
  static int cstring_length_estimate(const T &);
  
  static int cstring_length_estimate(const char*);

  template <typename METHOD>
  static void cstring_split(const std::string &split_line, const char comma,
                            METHOD substring_handler, int op_times = -1);

 private:
  CString() = default;
};

class FinateStateMachine {
 public:
  class State {
   public:
    void set_ID(const std::string &);
    const std::string &get_ID() const;

    void register_Init(std::function<void()>);
    void register_Step(std::function<std::string()>);
    void register_Exit(std::function<void()>);

    void init();
    std::string step();
    void exit();

   private:
    std::string id_;
    bool is_registered_init_ = false;
    std::function<void()> init_;
    bool is_registered_step_ = false;
    std::function<std::string()> step_;
    bool is_registered_exit_ = false;
    std::function<void()> exit_;

   public:
    State() = default;
    State(const std::string &);
    State(const State &) = delete;
    State(State &&) = default;
    ~State() = default;
  };

  State *add_State(const std::string &);
  void set_InitState(const std::string &);
  const std::string &get_CurrentState() const;
  void run();

 private:
  State *current_state_;
  std::string init_state_;
  std::unordered_map<std::string, State *> state_map_;

 public:
  FinateStateMachine() = default;
  virtual ~FinateStateMachine() = default;
};

}  // namespace utility
}  // namespace atd

#include "utility.tcc"
