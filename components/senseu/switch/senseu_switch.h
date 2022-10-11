#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/senseu/senseu.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace senseu {

class SenseUSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;

  void set_senseu_parent(SenseU *parent) { this->parent_ = parent; }

 protected:
  void write_state(bool state) override;

  SenseU *parent_;

};

}  // namespace senseu
}  // namespace esphome

#endif

