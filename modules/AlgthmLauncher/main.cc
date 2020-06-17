#include <iostream>
#include <thread>

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"
#include "modules/common/utility/file_parser/csv_parser.h"
#include "modules/common/utility/ipc/semaphore_mutex.h"
#include "modules/common/utility/ipc/shared_memory.h"
#include "modules/common/utility/ipc/shm_protocol.h"

using namespace atd::utility;

int main(int argc, char* argv[]) {
  Singleton::try_register<CSV_Observer>(ReadWriteableFile::FILE_MODE::WRITE,
                                        "test.csv", "./");

  Singleton::instance<CSV_Observer>()->push_Item("var1", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var2", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var3", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var2", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var1", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var1", "var");
  Singleton::instance<CSV_Observer>()->push_Item("var1", "var");

  while (1) {
    /* Write it into shared memory */
    TIMER->set_EndStick("counter");
    TIMER->set_BeginStick("counter");

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "last step elapse "
              << static_cast<double>(TIMER->get_TimeGap("counter")) / 1000
              << "ms" << std::endl;
  }

  exit(EXIT_SUCCESS);
}