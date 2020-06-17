#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <exception>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace atd {
namespace utility {

using namespace std::chrono;

static const char *NODE_LAUNCH_STICK_ID = "launch_time";
static const char *NODE_STEP_STICK_ID = "step_time";

/* class : runtime calculator */

template <class Duration = milliseconds>
class Runtime_Calculator {
  typedef time_point<system_clock, Duration> time_stick;

 public:
  Runtime_Calculator() = default;
  virtual ~Runtime_Calculator() = default;

  void set_BeginStick(const std::string &name) {
    time_stick now_stick = Now();
    auto itr4map = time_probe_.find(name);
    if (itr4map == time_probe_.cend()) {
      time_probe_.emplace(name, now_stick);
    } else {
      itr4map->second = now_stick;
    }
  }

  void set_EndStick(const std::string &name) {
    const auto itr4stick = time_probe_.find(name);
    if (itr4stick == time_probe_.cend()) {
      throw std::logic_error("can not find begin stick");
      return;
    }
    time_stick now_stick = Now();
    Duration time_gap = now_stick - itr4stick->second;
    auto itr4storage = storage_duration_.find(name);
    if (itr4storage == storage_duration_.cend()) {
      storage_duration_.emplace(name, time_gap);
    } else {
      itr4storage->second = time_gap;
    }
  }

  int get_TimeGap(const std::string &name) const {
    const auto itr4map = storage_duration_.find(name);
    if (itr4map == storage_duration_.cend()) {
      return Duration(0).count();
    }
    return itr4map->second.count();
  }

#ifdef FLAG_USE_OUTSIDE_TIME
 public:
  system_clock::time_point TimeSync(const system_clock::time_point &);
  system_clock::time_point TimeSync(const uint64_t &);

 private:
  system_clock::time_point Now();
#else

 private:
  time_stick Now() { return time_point_cast<Duration>(system_clock::now()); }
#endif

 private:
  std::unordered_map<std::string, time_stick> time_probe_;
  std::unordered_map<std::string, Duration> storage_duration_;
};

/* class : runtime counter */
class Runtime_Counter {
 public:
  Runtime_Counter() = default;
  virtual ~Runtime_Counter() = default;

  void set_Counter(const std::string &name) {
    const auto itr4counter = counter_probe_.find(name);
    if (itr4counter == counter_probe_.cend()) {
      counter_probe_.emplace(name, uint64_t(1));
    } else {
      counter_probe_[name] += 1;
    }
  }

  const uint64_t &get_Counter(const std::string &name) const {
    return counter_probe_.at(name);
  }

 private:
  std::unordered_map<std::string, uint64_t> counter_probe_;
};

/* class : runtime observer */

class Runtime_Observer : public Runtime_Calculator<microseconds>,
                         public Runtime_Counter {
 private:
  Runtime_Observer() { set_BeginStick(NODE_LAUNCH_STICK_ID); }
  ~Runtime_Observer() = default;
  Runtime_Observer(const Runtime_Observer &) = delete;
  Runtime_Observer(Runtime_Observer &&) = delete;

  int NODE_LOG_PERIOD_ = 5;
  static Runtime_Observer *instance_;
  static std::mutex instance_mutex_;

 public:
  static Runtime_Observer *get_Instance();

  void set_LogPeriod(const int &);

  bool is_LogPeriod() const;
};

#ifndef TIMER
#define TIMER atd::utility::Runtime_Observer::get_Instance()
#endif

}  // namespace utility
}  // namespace atd

#endif