# ESPHome Sense-U Baby Monitor

This project aims to provide an ESPHome integration for the Sense-U baby monitor by directly communicating via BLE with the Sense-U button.

There is now also support for replacing the firmware of the Base Station 2 with ESPHome, since the Base Station 2 is powered by an ESP32 module. See below for details.

## Status

The basics are working for this component. However, some functionality might be missing or might be unimplemented. 
The Base Station 2 is, apart from the temperature sensor, fully supported.

## Getting started

These steps assume that your Sense-U is **not** paired to an app/the base station. If it is paired, then the pairing is lost. Technically, it is possible to connect the sensor to app/base station and ESPHome, but not at the same time. See section "Sharing baby code".

  1. Add the required configuration to the YAML file - see [example_sense-u.yaml](example_sense-u.yaml) for a template. Do not provide a baby code.
  2. Compile and upload, start log output. You will see a message similar to "Could not register UID, enable pairing mode and try again."
  3. Enable pairing mode on the Sense-U button by double-shaking the sensor. It starts blinking.
  4. Observe the log, the next connection attempt should be successful.

## Sharing baby code

The pairing procedure is not a pairing at the BLE level, but rather at the application level. The Sense-U generates a Baby Code that needs to be provided during initialization. 
If you capture the relevant BLE packet on Android during pairing, you can reuse the baby code for ESPHome. You can then connect using your phone and using ESPHome, but not simultaneously.

If you have multiple ESPHome devices, you can also share the baby code. 

  1. Pair as usual on ESPHome device #1.
  2. Check the log for the baby code - a message similar to "Paired using baby code: 1234567890" is displayed (early during setup, make sure to capture it soon enough).
  3. Add the provided baby_code to the ESPHome config of device #2.
  4. To make sure that the code is remembered during updates/resets, it is recommended to add the same baby_code to device #1 once it is known.

## Protocol

See a description on the BLE protocol in [protocol.md](protocol.md)

## Base Station 2

The Base Station 2 can be converted to ESPHome. The device needs to be disassembled, inside you will find three pins labelled "IO0", "ESP32_RX" and "ESP32_TX". On another unpopulated header you will find 5V and GND pins. Use these pins for flashing, **do not connect USB**.

If you want to make a backup first, you can do so by running `esptool.py`:
```
esptool.py -b 115200 --port /dev/ttyUSB0 read_flash 0x00000 0x400000 flash_4M.bin
```

An exemplary configuration file will follow soon, for the time being, this information should be sufficient:

```
output:
  - platform: ledc
    pin: GPIO4
    id: buzzer

  - platform: ledc
    pin: GPIO17
    id: status_output_green

  - platform: ledc
    pin: GPIO18
    id: status_output_blue

  - platform: ledc
    pin: GPIO5
    id: status_output_red

  - platform: ledc
    pin: GPIO27
    id: main_output_blue

  - platform: ledc
    pin: GPIO32
    id: main_output_green

  - platform: ledc
    pin: GPIO33
    id: main_output_red

switch:
  - platform: template
    turn_on_action:
      - output.turn_on: buzzer
      - output.ledc.set_frequency:
          id: buzzer
          frequency: "1000Hz"
      - output.set_level:
          id: buzzer
          level: "50%"
    turn_off_action:
      - output.turn_off: buzzer
    name: Piezo On

binary_sensor:
  - platform: gpio
    pin:
      number: GPIO25
      inverted: yes
    name: "Reset Button"
    id: reset_button

  - platform: gpio
    pin: GPIO14
    id: touch_sensor
    name: "Touch Sensor"

light:
  - platform: rgb
    name: "Status Light"
    red: status_output_red
    green: status_output_green
    blue: status_output_blue

  - platform: rgb
    name: "Main Light"
    red: main_output_red
    green: main_output_green
    blue: main_output_blue
```

## TODO

  * Provide more examples how to handle alerts
  * Improve battery sensor
  * Investigate base station