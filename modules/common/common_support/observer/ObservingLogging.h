#ifndef OBSERVING_LOGGING_H_
#define OBSERVING_LOGGING_H_

#include <signal.h>

#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "modules/common/3rd_party/easylogging/easylogging++.h"

namespace atd {
namespace utility {

#ifndef NUMINFO
#define NUMINFO(num) #num << " = " << num << " "
#endif

static const char *LOGGER_MOTION_ID = "motion";

#if defined(ELPP_FEATURE_CRASH_LOG)
#define SIGNALREGISTRATION(logger)                                      \
  static bool flag_signal_once = true;                                  \
  if (flag_signal_once) {                                               \
    atd::planning::common::LoggingCondition::set_LoggerID(logger);      \
    signal(SIGSEGV,                                                     \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    signal(SIGFPE,                                                      \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    signal(SIGILL,                                                      \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    signal(SIGINT,                                                      \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    signal(SIGTERM,                                                     \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    signal(SIGPIPE,                                                     \
           atd::planning::common::LoggingCondition::deal_CrashHandler); \
    flag_signal_once = false;                                           \
  }
#endif

#ifndef REGISTER_LOGGER
#define REGISTER_LOGGER(name, path)                                \
  static bool flag_logger_once = true;                             \
  if (flag_logger_once) {                                          \
    el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason); \
    el::Loggers::getLogger(name);                                  \
    el::Configurations conf(path);                                 \
    el::Loggers::reconfigureLogger(name, conf);                    \
    flag_logger_once = false;                                      \
  }
#endif

class LoggingCondition {
 public:
  LoggingCondition() = default;
  ~LoggingCondition() = default;

  static bool is_FuncObserving(const std::function<bool()> &func) {
    return func();
  }

#if defined(ELPP_FEATURE_CRASH_LOG)
 public:
  static void deal_CrashHandler(int signum) {
    el::Helpers::logCrashReason(signum, true, el::Level::Fatal,
                                crash_handler_logger_ID);
  }

  static void set_LoggerID(const char *id) {
    crash_handler_logger_ID = new char[strlen(id) + 1];
    strcpy(crash_handler_logger_ID, id);
  }

  static char *crash_handler_logger_ID;
#endif
};

}  // namespace utility
}  // namespace atd

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &o_strm,
                         const std::pair<T1, T2> &target) {
  o_strm << "std::pair: "
         << "[" << target.first << ", " << target.second << "]";
  return o_strm;
}

template <typename T1>
std::ostream &operator<<(std::ostream &o_strm, const std::vector<T1> &target) {
  o_strm << "std::vector: ";
  if (target.empty()) {
    o_strm << "empty";
    return o_strm;
  }
  o_strm << "[";
  for (auto item4loop = target.cbegin(); item4loop != (target.cend() - 1);
       item4loop++) {
    o_strm << *item4loop << " ";
  }
  o_strm << *(target.cend() - 1) << "]";
  return o_strm;
}

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &o_strm,
                         const std::map<T1, T2> &map_data) {
  o_strm << "std::map size: " << map_data.size();
  if (map_data.empty()) {
    return o_strm;
  }
  int map_counter = 1;
  for (const auto &itr4loop : map_data) {
    o_strm << map_counter << " > " << *itr4loop << "\n";
    map_counter++;
  }
  return o_strm;
}

#define PLANNING_RESET "\033[0m"
#define PLANNING_BLACK "\033[30m"              /* Black */
#define PLANNING_RED "\033[31m"                /* Red */
#define PLANNING_GREEN "\033[32m"              /* Green */
#define PLANNING_YELLOW "\033[33m"             /* Yellow */
#define PLANNING_BLUE "\033[34m"               /* Blue */
#define PLANNING_MAGENTA "\033[35m"            /* Magenta */
#define PLANNING_CYAN "\033[36m"               /* Cyan */
#define PLANNING_WHITE "\033[37m"              /* White */
#define PLANNING_BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define PLANNING_BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define PLANNING_BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define PLANNING_BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define PLANNING_BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define PLANNING_BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define PLANNING_BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define PLANNING_BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define RED_CERR(str) \
  std::cerr << PLANNING_RED << str << PLANNING_RESET << std::endl;

#endif