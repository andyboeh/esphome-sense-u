import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
    CONF_HUMIDITY,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_FREQUENCY,
)

CODEOWNERS = ["@andyboeh"]
DEPENDENCIES = ["senseu"]

from . import senseu

UNIT_PER_MINUTE = "/min"
CONF_BREATH_RATE = "breath_rate"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(senseu),
            cv.Optional(CONF_BREATH_RATE): sensor.sensor_schema(
                unit_of_measurement=UNIT_PER_MINUTE,
                device_class=DEVICE_CLASS_FREQUENCY,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                device_class=DEVICE_CLASS_TEMPERATURE,
                accuracy_decimals=1,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                device_class=DEVICE_CLASS_HUMIDITY,
                accuracy_decimals=1,
            ),
        }
    )
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if CONF_BREATH_RATE in config:
        sens = await sensor.new_sensor(config[CONF_BREATH_RATE])
        cg.add(parent.set_breath_rate(sens))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(parent.set_temperature(sens))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(parent.set_humidity(sens))
