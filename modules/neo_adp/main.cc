#include "modules/common/common_header.h"
#include "modules/neo_adp/data_service/data_repository.h"
#include "modules/neo_adp/data_service/data_seg4data_monitor.h"
#include "modules/neo_adp/data_service/data_seg4lcm_protocol.h"
#include "modules/neo_adp/imgui_implement/file_interface.h"
#include "modules/neo_adp/opengl_implement/gl_implement.hpp"

using namespace atd::utility;
using namespace atd::protocol;

void init() {
  atd::utility::Singleton::try_register<Runtime_Calculator<>>();
  atd::utility::Singleton::try_register<Runtime_Counter>();
  atd::utility::Singleton::try_register<OpenGL_Frame>();
  atd::utility::Singleton::try_register<ResourceInterface_Manager>();
  atd::utility::Singleton::try_register<DataRepository>();

  atd::utility::Singleton::instance<DataRepository>()
      ->try_register<RealTimeDataDispatcher>(Data_Seg_Name_LCMProtocol);
  atd::utility::Singleton::instance<DataRepository>()
      ->try_register<DataSeg4DataMonitor>(Data_Seg_Name_DataMonitor);

  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &GL_Implement::drawGrid);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &GL_Implement::drawText);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &GL_Implement::draw_PlanningElements);
}

/* main function */
int main(int args, char** argv) {
  init();
  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();
  exit(EXIT_SUCCESS);
}
