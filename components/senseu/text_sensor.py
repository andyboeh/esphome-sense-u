import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    CONF_STATUS,
)

CODEOWNERS = ["@andyboeh"]
DEPENDENCIES = ["senseu"]

from . import senseu

CONF_POSTURE = "posture"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(senseu),
            cv.Optional(CONF_POSTURE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(),
        }
    )
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ID])

    if CONF_STATUS in config:
        sens = await text_sensor.new_text_sensor(config[CONF_STATUS])
        cg.add(parent.set_status_sensor(sens))

    if CONF_POSTURE in config:
        sens = await text_sensor.new_text_sensor(config[CONF_POSTURE])
        cg.add(parent.set_posture_sensor(sens))

