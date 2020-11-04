#pragma once

#include <stdlib.h>
#include <time.h>

#include <chrono>

#include "thread_safe.h"
#include "utility.h"

namespace atd {
namespace utility {

using namespace std::chrono;

/* class : runtime calculator */

template <class Duration = milliseconds>
class Runtime_Calculator : public Singleton {
  friend class Singleton;
  typedef time_point<system_clock, Duration> time_stick;

 public:
  void set_BeginStick(const std::string &name) {
    time_stick now_stick = Now();
    time_probe_.insert_change(name, now_stick);
  }

  void wait_and_set_EndStick(const std::string &name) {
    std::pair<time_stick, bool> end_stick{{}, false};
    while (!end_stick.second) {
      end_stick = time_probe_.read_value(name);
    }

    time_stick now_stick = Now();
    Duration time_gap = now_stick - end_stick.first;
    storage_duration_.insert_change(name, time_gap);
  }

  std::pair<uint64_t, bool> get_TimeGap(const std::string &name) const {
    auto time_gap = storage_duration_.read_value(name);
    if (time_gap.second) {
      return {time_gap.first.count(), true};
    }
    return {0, false};
  }

  time_stick Now() { return time_point_cast<Duration>(system_clock::now()); }

  float get_Rand() {
    srand(static_cast<unsigned int>(time(NULL)));
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  }

  bool set_CountDown(const std::string &name, uint64_t mill_sec) {
    time_stick now_stick = Now();
    return countdown_probe_.try_insert(
        name, std::make_pair(now_stick,
                             duration_cast<Duration>(milliseconds(mill_sec))));
  }

  bool is_CountDownOver(const std::string &name) {
    auto read_ins = countdown_probe_.read_value(name);
    if (read_ins.second) {
      time_stick now_stick = Now();
      Duration time_gap = now_stick - read_ins.first.first;
      if (time_gap.count() > read_ins.first.second.count()) {
        countdown_probe_.remove_key(name);
      } else {
        return false;
      }
    }
    return true;
  }

 private:
  ThreadSafe_HashMap<std::string, time_stick> time_probe_;
  ThreadSafe_HashMap<std::string, Duration> storage_duration_;
  ThreadSafe_HashMap<std::string, std::pair<time_stick, milliseconds>>
      countdown_probe_;

 private:
  Runtime_Calculator() = default;
  virtual ~Runtime_Calculator() = default;
};

class Runtime_Counter : public Singleton {
  friend class Singleton;

 public:
  uint64_t get_Counter(const std::string &name) {
    auto find_res = counters_.insert({name, 0});
    if (find_res.second) {
      return 0;
    }
    return ++(find_res.first->second);
  }

 private:
  std::unordered_map<std::string, uint64_t> counters_;

 private:
  Runtime_Counter() = default;
  ~Runtime_Counter() = default;
};

}  // namespace utility
}  // namespace atd

#define TIMER                        \
  atd::utility::Singleton::instance< \
      atd::utility::Runtime_Calculator<std::chrono::milliseconds>>()

#define COUNTER(name)                                                \
  atd::utility::Singleton::instance<atd::utility::Runtime_Counter>() \
      ->get_Counter(name)
