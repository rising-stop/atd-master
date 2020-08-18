#pragma once

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

  time_stick Now() {
    return time_point_cast<Duration>(system_clock::now());
  }

 private:
  ThreadSafe_HashMap<std::string, time_stick> time_probe_;
  ThreadSafe_HashMap<std::string, Duration> storage_duration_;

 private:
  Runtime_Calculator() = default;
  virtual ~Runtime_Calculator() = default;
};

}  // namespace utility
}  // namespace atd