#include "ObservingLogging.h"

namespace atd {
namespace utility {

#if defined(ELPP_FEATURE_CRASH_LOG)
char* LoggingCondition::crash_handler_logger_ID;
#endif

}  // namespace utility
}  // namespace atd
