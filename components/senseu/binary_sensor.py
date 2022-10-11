import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
)

CODEOWNERS = ["@andyboeh"]
DEPENDENCIES = ["senseu"]

from . import senseu

CONF_POSTURE = "posture"
CONF_BREATH = "breath"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(senseu),
            cv.Optional(CONF_POSTURE): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_TEMPERATURE): binary_sensor.binary_sensor_schema(),
            cv.Optional(CONF_BREATH): binary_sensor.binary_sensor_schema(),
        }
    )
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if CONF_BREATH in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BREATH])
        cg.add(parent.set_breath_alarm(sens))

    if CONF_TEMPERATURE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_TEMPERATURE])
        cg.add(parent.set_temperature_alarm(sens))

    if CONF_POSTURE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_POSTURE])
        cg.add(parent.set_posture_alarm(sens))

