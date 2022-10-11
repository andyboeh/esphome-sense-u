#include "senseu_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

#ifdef USE_ESP32

namespace esphome {
namespace senseu {

static const char *const TAG = "senseu.switch";

void SenseUSwitch::setup() {

}

void SenseUSwitch::write_state(bool state) {
    this->parent_->set_power_switch(state);
    this->publish_state(state);
}

}  // namespace senseu
}  // namespace esphome

#endif

