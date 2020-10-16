#include "data_repository.h"

void DataRepository::update() {
  for (auto& item : ptr_map_) {
    if (!item.second->update()) {
      item.second->error_handler();
    }
  }
}