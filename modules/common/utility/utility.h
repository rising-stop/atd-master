/* created by liangle */

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <unordered_map>

#include "exceptions.h"

namespace atd {
namespace common {
namespace utility {

/**
 * @brief Macro SINGLETON
 * declare class as the singleton, using default constructor
 */
#define SINGLETON(TYPE)                              \
 public:                                             \
  static TYPE *instance() {                          \
    std::lock_guard<std::mutex> lk(instance_mutex_); \
    std::call_once(flag_init_, &init);               \
    return instance_;                                \
  }                                                  \
                                                     \
 private:                                            \
  static void init() { instance_ = new TYPE(); }     \
  static TYPE *instance_;                            \
  static std::once_flag flag_init_;                  \
  static std::mutex instance_mutex_;                 \
  TYPE() = default;                                  \
  ~TYPE() = default;                                 \
  TYPE(const TYPE &) = delete;                       \
  TYPE(TYPE &&) = delete;

#define SINGLETON_MEMBER_REGISTER(TYPE) \
  TYPE *TYPE::instance_ = nullptr;      \
  std::once_flag TYPE::flag_init_;      \
  std::mutex TYPE::instance_mutex_;

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
    auto result = CreateObjectOrNull(id, args...);
    if (!result) {
      std::stringstream sstm;
      sstm << "not found " << id << " in factory";
      throw CommonException(sstm.str());
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
// class Singleton {
//  public:
//   typedef std::string SGLTN_ID;
//   template <typename SINGLETON_DERIVED>
//   static typename std::enable_if<
//       std::is_base_of<Singleton, SINGLETON_DERIVED>::value,
//       SINGLETON_DERIVED>::type *
//   instance(const SGLTN_ID &id) {
//     std::call_once(flag_init_, &init);
//     return dynamic_cast<SINGLETON_DERIVED *>(registry_[id]);
//   }

//  protected:
//   template <typename SINGLETON_DERIVED>
//   bool try_register(const SGLTN_ID &id, SINGLETON_DERIVED *ptr_instnc) {
//     return registry_.insert(std::make_pair(id, ptr_instnc)).second;
//   }

//  private:
//   static std::once_flag flag_init_;
//   static std::unordered_map<SGLTN_ID, Singleton *> registry_;

//  protected:
//   Singleton() = default;
//   ~Singleton() = default;
//   Singleton(const Singleton &) = delete;
//   Singleton(Singleton &&) = delete;
// };

}  // namespace utility
}  // namespace common
}  // namespace atd