#include "ObservingLogging.h"

namespace atd {
namespace planning {
namespace common {
#if defined(ELPP_FEATURE_CRASH_LOG)
char* LoggingCondition::crash_handler_logger_ID;
#endif
}  // namespace common
}  // namespace planning

}  // namespace atd
