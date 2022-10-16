#include "senseu_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

#ifdef USE_ESP32

namespace esphome {
namespace senseu {

static const char *const TAG = "senseu.switch";

void SenseUSwitch::setup() {
    bool initial_state = this->get_initial_state().value_or(true);
    if (initial_state) {
        this->turn_on();
    } else {
        this->turn_off();
    }
}

void SenseUSwitch::write_state(bool state) {
    this->parent_->set_power_switch(state);
    this->publish_state(state);
}

}  // namespace senseu
}  // namespace esphome

#endif

