#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/preferences.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace senseu {

namespace espbt = esphome::esp32_ble_tracker;

static const std::string BASE_SERVICE_UUID_PREFIX = "0102ca20-9e06-a079-2e3f-";
static const std::string DATA_CHAR_1_PREFIX = "01021921-9e06-a079-2e3f-";
static const std::string DATA_CHAR_2_PREFIX = "01021922-9e06-a079-2e3f-";
static const std::string DATA_CHAR_3_PREFIX = "01021923-9e06-a079-2e3f-";
static const std::string DATA_CHAR_4_PREFIX = "01021925-9e06-a079-2e3f-";
static uint8_t SET_UID_DATA[] = {0x69, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x00 };

enum WRITE_REQ {
  UID_DATA,
  REGISTER_TYPE,
  RECONNECTION_TYPE,
  GET_BATCH,
  LEANING_TYPE,
  TEMP_ALARM,
  KICKING_ALARM,
  BREATH_ALARM,
  POWER_ON,
  POWER_OFF,
};

struct SenseUStorage {
  bool paired;
  bool configured;
  uint8_t baby_code[6];
} PACKED;

class SenseU : public esphome::ble_client::BLEClientNode, public Component {
 public:
  void setup() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void set_breath_rate(sensor::Sensor *breath_rate) { breath_rate_ = breath_rate; }
  void set_temperature(sensor::Sensor *temperature) { temperature_ = temperature; }
  void set_humidity(sensor::Sensor *humidity) { humidity_ = humidity; }
  void set_posture_sensor(text_sensor::TextSensor *posture) { posture_ = posture; }
  void set_status_sensor(text_sensor::TextSensor *status) { status_ = status; }
  void set_breath_alarm(binary_sensor::BinarySensor *breath_alarm ) { breath_alarm_ = breath_alarm; }
  void set_posture_alarm(binary_sensor::BinarySensor *posture_alarm ) { posture_alarm_ = posture_alarm; }
  void set_temperature_alarm(binary_sensor::BinarySensor *temperature_alarm) { temperature_alarm_ = temperature_alarm; }
  void set_power_switch(bool state);
  void set_baby_code(std::string baby_code) { baby_code_ = baby_code; }

 private:
  void write_char(WRITE_REQ cmd);
  bool discover_characteristics();
  void write_notify_config_descriptor(bool enable);
  void enable_notifications();
  void store_preferences();

  ESPPreferenceObject pref_;
  SenseUStorage pref_storage_;
  espbt::ESPBTUUID base_service_uuid_;
  espbt::ESPBTUUID data_char_1_uuid_;
  espbt::ESPBTUUID data_char_2_uuid_;
  espbt::ESPBTUUID data_char_3_uuid_;
  espbt::ESPBTUUID data_char_4_uuid_;
  std::string baby_code_;
  uint16_t char_handle_1_;
  uint16_t cccd_1_;
  uint16_t char_handle_2_;
  uint16_t cccd_2_;
  uint16_t char_handle_3_;
  uint16_t cccd_3_;
  uint16_t char_handle_4_;
  uint16_t cccd_4_;
  bool power_state_{true};
  sensor::Sensor *breath_rate_{nullptr};
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *humidity_{nullptr};
  text_sensor::TextSensor *posture_{nullptr};
  text_sensor::TextSensor *status_{nullptr};
  binary_sensor::BinarySensor *breath_alarm_{nullptr};
  binary_sensor::BinarySensor *posture_alarm_{nullptr};
  binary_sensor::BinarySensor *temperature_alarm_{nullptr};
};

}  // namespace senseu
}  // namespace esphome

#endif

