#include "senseu.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

#ifdef USE_ESP32

namespace esphome {
namespace senseu {

static const char *const TAG = "senseu";

void SenseU::dump_config() {
  ESP_LOGCONFIG(TAG, "SenseU");
  LOG_SENSOR(" ", "Breath Rate", this->breath_rate_);
  LOG_SENSOR(" ", "Temperature", this->temperature_);
  LOG_SENSOR(" ", "Humidity", this->humidity_);
}

void SenseU::setup() {
  std::string addr = this->parent_->address_str();
  addr.erase(std::remove(addr.begin(), addr.end(), ':'), addr.end());
  this->base_service_uuid_ = espbt::ESPBTUUID::from_raw(BASE_SERVICE_UUID_PREFIX + addr);
  this->data_char_1_uuid_ = espbt::ESPBTUUID::from_raw(DATA_CHAR_1_PREFIX + addr);
  this->data_char_2_uuid_ = espbt::ESPBTUUID::from_raw(DATA_CHAR_2_PREFIX + addr);
  this->data_char_3_uuid_ = espbt::ESPBTUUID::from_raw(DATA_CHAR_3_PREFIX + addr);
  this->data_char_4_uuid_ = espbt::ESPBTUUID::from_raw(DATA_CHAR_4_PREFIX + addr);
  ESP_LOGD(TAG, "Base UUID: %s", this->base_service_uuid_.to_string().c_str());
  ESP_LOGD(TAG, "Data Char 1: %s", this->data_char_1_uuid_.to_string().c_str());
  ESP_LOGD(TAG, "Data Char 2: %s", this->data_char_2_uuid_.to_string().c_str());
  ESP_LOGD(TAG, "Data Char 3: %s", this->data_char_3_uuid_.to_string().c_str());
  ESP_LOGD(TAG, "Data Char 4: %s", this->data_char_4_uuid_.to_string().c_str());
}

bool SenseU::discover_characteristics() {
    bool result = true;
    esphome::ble_client::BLECharacteristic *chr;
    esphome::ble_client::BLEDescriptor *desc;
    
    if(!this->char_handle_1_) {
        chr = this->parent_->get_characteristic(this->base_service_uuid_, this->data_char_1_uuid_);
        if(chr == nullptr) {
            ESP_LOGE(TAG, "Required data characteristic %s not found.", this->data_char_1_uuid_.to_string().c_str());
            result = false;
        } else {
            this->char_handle_1_ = chr->handle;
        }
    }

    if(!this->char_handle_2_) {
        chr = this->parent_->get_characteristic(this->base_service_uuid_, this->data_char_2_uuid_);
        if(chr == nullptr) {
            ESP_LOGE(TAG, "Required data characteristic %s not found.", this->data_char_2_uuid_.to_string().c_str());
            result = false;
        } else {
            this->char_handle_2_ = chr->handle;
        }
    }

    if(!this->char_handle_3_) {
        chr = this->parent_->get_characteristic(this->base_service_uuid_, this->data_char_3_uuid_);
        if(chr == nullptr) {
            ESP_LOGE(TAG, "Required data characteristic %s not found.", this->data_char_3_uuid_.to_string().c_str());
            result = false;
        } else {
            this->char_handle_3_ = chr->handle;
        }
    }

    if(!this->char_handle_4_) {
        chr = this->parent_->get_characteristic(this->base_service_uuid_, this->data_char_4_uuid_);
        if(chr == nullptr) {
            ESP_LOGE(TAG, "Required data characteristic %s not found.", this->data_char_4_uuid_.to_string().c_str());
            result = false;
        } else {
            this->char_handle_4_ = chr->handle;
        }
    }

    if(!this->cccd_1_) {
        desc = this->parent_->get_config_descriptor(this->char_handle_1_);
        if(desc == nullptr) {
            ESP_LOGW(TAG, "No config descriptor for status handle 0x%x. Will not be able to receive notifications",
                this->char_handle_1_);
            result = false;
        } else if(desc-> uuid.get_uuid().len != ESP_UUID_LEN_16 ||
                  desc->uuid.get_uuid().uuid.uuid16 != ESP_GATT_UUID_CHAR_CLIENT_CONFIG) {
            ESP_LOGW(TAG, "Config descriptor 0x%x (uuid %s) is not a client config char uuid", this->char_handle_1_,
                  desc->uuid.to_string().c_str());
            result = false;
        } else {
            this->cccd_1_ = desc->handle;
        }
    }

    if(!this->cccd_2_) {
        desc = this->parent_->get_config_descriptor(this->char_handle_2_);
        if(desc == nullptr) {
            ESP_LOGW(TAG, "No config descriptor for status handle 0x%x. Will not be able to receive notifications",
                this->char_handle_2_);
            result = false;
        } else if(desc-> uuid.get_uuid().len != ESP_UUID_LEN_16 ||
                  desc->uuid.get_uuid().uuid.uuid16 != ESP_GATT_UUID_CHAR_CLIENT_CONFIG) {
            ESP_LOGW(TAG, "Config descriptor 0x%x (uuid %s) is not a client config char uuid", this->char_handle_2_,
                  desc->uuid.to_string().c_str());
            result = false;
        } else {
            this->cccd_2_ = desc->handle;
        }
    }

    if(!this->cccd_3_) {
        desc = this->parent_->get_config_descriptor(this->char_handle_3_);
        if(desc == nullptr) {
            ESP_LOGW(TAG, "No config descriptor for status handle 0x%x. Will not be able to receive notifications",
                this->char_handle_3_);
            result = false;
        } else if(desc-> uuid.get_uuid().len != ESP_UUID_LEN_16 ||
                  desc->uuid.get_uuid().uuid.uuid16 != ESP_GATT_UUID_CHAR_CLIENT_CONFIG) {
            ESP_LOGW(TAG, "Config descriptor 0x%x (uuid %s) is not a client config char uuid", this->char_handle_3_,
                  desc->uuid.to_string().c_str());
            result = false;
        } else {
            this->cccd_3_ = desc->handle;
        }
    }

    if(!this->cccd_4_) {
        desc = this->parent_->get_config_descriptor(this->char_handle_4_);
        if(desc == nullptr) {
            ESP_LOGW(TAG, "No config descriptor for status handle 0x%x. Will not be able to receive notifications",
                this->char_handle_4_);
            result = false;
        } else if(desc-> uuid.get_uuid().len != ESP_UUID_LEN_16 ||
                  desc->uuid.get_uuid().uuid.uuid16 != ESP_GATT_UUID_CHAR_CLIENT_CONFIG) {
            ESP_LOGW(TAG, "Config descriptor 0x%x (uuid %s) is not a client config char uuid", this->char_handle_4_,
                  desc->uuid.to_string().c_str());
            result = false;
        } else {
            this->cccd_4_ = desc->handle;
        }
    }

    return result;
}

void SenseU::enable_notifications() {
      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(),
                                                      this->parent_->get_remote_bda(), this->char_handle_1_);
      if(status) {
        ESP_LOGW(TAG, "Registering for notification failed on %s", this->data_char_1_uuid_.to_string().c_str());
      }

      if(this->paired_) {
          status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(),
                                                          this->parent_->get_remote_bda(), this->char_handle_2_);
          if(status) {
            ESP_LOGW(TAG, "Registering for notification failed on %s", this->data_char_2_uuid_.to_string().c_str());
          }

          status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(),
                                                          this->parent_->get_remote_bda(), this->char_handle_3_);
          if(status) {
            ESP_LOGW(TAG, "Registering for notification failed on %s", this->data_char_3_uuid_.to_string().c_str());
          }

          status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(),
                                                          this->parent_->get_remote_bda(), this->char_handle_4_);
          if(status) {
            ESP_LOGW(TAG, "Registering for notification failed on %s", this->data_char_4_uuid_.to_string().c_str());
          }
      }
}

void SenseU::write_notify_config_descriptor(bool enable) {
    if(!this->cccd_1_ || !this->cccd_2_ || !this->cccd_3_ || !this->cccd_4_) {
        ESP_LOGW(TAG, "No descriptor found for notify of handle 0x%x", this->char_handle_1_);
        return;
    }

    uint16_t notify_en = enable ? 1 : 0;
    auto status = esp_ble_gattc_write_char_descr(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), 
                                                 this->cccd_1_, sizeof(notify_en), (uint8_t *) &notify_en, ESP_GATT_WRITE_TYPE_RSP,
                                                 ESP_GATT_AUTH_REQ_NONE);

    if(status) {
        ESP_LOGW(TAG, "esp_ble_gattc_write_char_descr error, status=%d", status);
    }

    if(this->paired_) { 
        status = esp_ble_gattc_write_char_descr(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), 
                                                     this->cccd_2_, sizeof(notify_en), (uint8_t *) &notify_en, ESP_GATT_WRITE_TYPE_RSP,
                                                     ESP_GATT_AUTH_REQ_NONE);

        if(status) {
            ESP_LOGW(TAG, "esp_ble_gattc_write_char_descr error, status=%d", status);
        }

        status = esp_ble_gattc_write_char_descr(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), 
                                                     this->cccd_3_, sizeof(notify_en), (uint8_t *) &notify_en, ESP_GATT_WRITE_TYPE_RSP,
                                                     ESP_GATT_AUTH_REQ_NONE);

        if(status) {
            ESP_LOGW(TAG, "esp_ble_gattc_write_char_descr error, status=%d", status);
        }

        status = esp_ble_gattc_write_char_descr(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), 
                                                     this->cccd_4_, sizeof(notify_en), (uint8_t *) &notify_en, ESP_GATT_WRITE_TYPE_RSP,
                                                     ESP_GATT_AUTH_REQ_NONE);

        if(status) {
            ESP_LOGW(TAG, "esp_ble_gattc_write_char_descr error, status=%d", status);
        }
    }
    
}

void SenseU::write_char(WRITE_REQ cmd) {
    uint16_t len = 20;
    uint8_t *data_ptr;
    uint8_t data[20];
    data_ptr = data;
    uint16_t chr = this->char_handle_4_;
    uint32_t now = millis();
    memset(data, 0x00, 20);

    switch(cmd) {
    case UID_DATA:
        len = 18;
        data_ptr = SET_UID_DATA;
        chr = this->char_handle_1_;
    break;
    case REGISTER_TYPE:
        len = 10;

        data[0] = 0x68;
        data[1] = (now >> 24) & 0xff;
        data[2] = (now >> 16) & 0xff;
        data[3] = (now >> 8) & 0xff;
        data[4] = (now & 0xff);
        data[5] = 0x01;
        data[6] = 0x00;
        data[7] = 0x32;
        data[8] = 0x03;
        data[9] = 0x00;
        chr = this->char_handle_1_;
    break;
    case RECONNECTION_TYPE:
        len = 15;

        data[0] = 0x70;
        for(int i=0; i<6; i++)
            data[i+1] = this->baby_code_[i];
        data[7] = (now >> 24) & 0xff;
        data[8] = (now >> 16) & 0xff;
        data[9] = (now >> 8) & 0xff;
        data[10] = (now & 0xff);
        data[11] = 0x00;
        data[12] = 0x00;
        data[13] = 0x00;
        data[14] = 0x00;
        chr = this->char_handle_1_;
    break;
    case GET_BATCH:
        len = 20;

        data[0] = 0xc0;
        data[1] = 0x01;  
    break;
    case LEANING_TYPE:
        len = 5;

        data[0] = 0xf5;
        data[1] = 0xf2;
        data[2] = 0x32;
        data[3] = 0x03;
        data[4] = 0x00;
    break;
    case TEMP_ALARM:
        len = 6;

        data[0] = 0xb2;
        data[1] = 0x00;
        data[2] = 0x68;
        data[3] = 0x01;
        data[4] = 0xc8;
        data[5] = 0x00;
    break;
    case KICKING_ALARM:
        len = 5;

        data[0] = 0xb3;
        data[1] = 0x00;
        data[2] = 0x0f;
        data[3] = 0x03;
        data[4] = 0x00;
    break;
    case BREATH_ALARM:
        len = 3;

        data[0] = 0xb0;
        data[1] = 0x01;
        data[2] = 0x19;
    break;
    default:
        return;
    }
    auto status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), 
          chr, len, data_ptr, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
    if(status) {
          ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(),
               status);
    }
}

void SenseU::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_OPEN_EVT");
      //this->logged_in_ = false;
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_DISCONNECT_EVT");
      //this->logged_in_ = false;
      this->node_state = espbt::ClientState::IDLE;
      if (this->breath_rate_ != nullptr)
        this->breath_rate_->publish_state(NAN);
      if (this->temperature_ != nullptr)
        this->temperature_->publish_state(NAN);
      if (this->humidity_ != nullptr)
        this->humidity_->publish_state(NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_SEARCH_COMPL_EVT");
      if(!this->discover_characteristics()) {
        ESP_LOGE(TAG, "Some required characteristics were not found.");
      } else {
        this->enable_notifications();
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      // This event means that ESP received the request to enable notifications on the client side. But we also have to
      // tell the server that we want it to send notifications. Normally BLEClient parent would handle this
      // automatically, but as soon as we set our status to Established, the parent is going to purge all the
      // service/char/descriptor handles, and then get_config_descriptor() won't work anymore. There's no way to disable
      // the BLEClient parent behavior, so our only option is to write the handle anyway, and hope a double-write
      // doesn't break anything.

      ESP_LOGD(TAG, "ESP_GATTC_REG_FOR_NOTIFY_EVT");
      if(this->node_state != espbt::ClientState::ESTABLISHED) {
          this->node_state = espbt::ClientState::ESTABLISHED;
          this->write_notify_config_descriptor(true);
      }
      ESP_LOGD(TAG, "status: %d, handle=0x%x", param->reg_for_notify.status, param->reg_for_notify.handle);
      if(!this->paired_) {
          this->write_char(UID_DATA);
      } else if(!this->configured_ && param->reg_for_notify.handle == this->char_handle_4_) {
          this->write_char(RECONNECTION_TYPE);
      }
      //this->update();
      break;
    }
    case ESP_GATTC_UNREG_FOR_NOTIFY_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_UNREG_FOR_NOTIFY_EVT");
      break;
    }
    case ESP_GATTC_WRITE_DESCR_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_WRITE_DESCR_EVT");
      break;
    }
    case ESP_GATTC_WRITE_CHAR_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_WRITE_CHAR_EVT");
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGD(TAG, "ESP_GATTC_NOTIFY_EVT");
      if(param->notify.conn_id != this->parent_->get_conn_id())
          break;
      if(param->notify.handle != this->char_handle_1_ && param->notify.handle != this->char_handle_2_ &&
         param->notify.handle != this->char_handle_3_ && param->notify.handle != this->char_handle_4_)
          break;
      for(int i=0; i<param->notify.value_len; i++) {
        ESP_LOGD(TAG, "[%s] ESP_GATTC_NOTIFY_EVT: handle=0x%x, value[%d]=0x%x", this->parent_->address_str().c_str(),
               param->notify.handle, i, param->notify.value[i]);
      }
      switch(param->notify.value[0]) {
        case 0x69: {
          if(param->notify.value[1] != 0x00) {
            ESP_LOGE(TAG, "Could not register UID, enable pairing mode and try again.");
            break;
          }
          this->write_char(REGISTER_TYPE);
          break;
        }
        case 0x68: {
          if(param->notify.value[1] == 0x00) {
            this->paired_ = true;
            esp_ble_gap_disconnect(this->parent_->get_remote_bda());
            for(int i=0; i<6; i++)
                this->baby_code_[i] = param->notify.value[i+2];
            //this->write_char(RECONNECTION_TYPE);
          }
          break;
        }
        case 0x70:
          if(param->notify.value[1] == 0x00) {
            ESP_LOGD(TAG, "Connected successfully!");
            this->write_char(GET_BATCH);
          } else if(param->notify.value[1] == 0x01) {
            ESP_LOGE(TAG, "Error connecting.");
          }
          break;
        case 0xc0:
          this->write_char(LEANING_TYPE);
          break;
        case 0xf5:
          if(!this->configured_) {
            this->write_char(TEMP_ALARM);
          }
          break;
        case 0xb2:
          write_char(KICKING_ALARM);
          break;
        case 0xb3:
          write_char(BREATH_ALARM);
          break;
        case 0xb0:
          this->configured_ = true;
          break;
        default: {
          uint8_t record_type = (param->notify.value[0] >> 3) & 0x1f;
          if(record_type == 0x06) {
            // battery
          } else if(record_type == 0x08) {
            uint8_t status_type = ((param->notify.value[0] << 8 | param->notify.value[1]) >> 6) & 0x1f;
            switch(status_type) {
            case 0x01:
                ESP_LOGD(TAG, "Temperature Package");
                break;
            case 0x02:
                ESP_LOGD(TAG, "Alert");
                break;
            case 0x03:
                break;
            case 0x04:
                ESP_LOGD(TAG, "Gesture");
                break;
            case 0x05:
                ESP_LOGD(TAG, "Breath");
                break;
            case 0x07:
                ESP_LOGD(TAG, "Activity");
                break;
            case 0x08:
                ESP_LOGD(TAG, "BlSignal");
                break;
            }
          }
        } 
      }
      /*
      if (param->notify.handle != this->char_handle_)
        break;

      if (this->illuminance_ != nullptr && this->decoder_->has_light_level()) {
        this->illuminance_->publish_state(this->decoder_->light_level_);
      }
      */
      break;
    }
    default:
      break;
  }
}

}  // namespace senseu
}  // namespace esphome

#endif
