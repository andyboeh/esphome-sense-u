# ESPHome Sense-U Baby Monitor

This project aims to provide an ESPHome integration for the Sense-U baby monitor by directly communicating via BLE with the Sense-U button.

## Status

The basics are working for this component. However, some functionality might be missing or might be unimplemented. 

## Getting started

These steps assume that your Sense-U is **not** paired to an app/the base station. If it is paired, then the pairing is lost. Technically, it is possible to connect the sensor to app/base station and ESPHome, but not at the same time. See section "Sharing baby code".

  1. Add the required configuration to the YAML file - see [example_sense-u.yaml](example_sense-u.yaml) for a template. Do not provide a baby code.
  2. Compile and upload, start log output. You will se a message similar to "Could not register UID, enable pairing mode and try again."
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
