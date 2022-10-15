
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client
from esphome.const import (
    CONF_ID,
)

CODEOWNERS = ["@andyboeh"]
DEPENDENCIES = ["ble_client", "binary_sensor", "text_sensor", "sensor"]

MULTI_CONF = True

CONF_SENSEU_ID = "senseu_id"
CONF_BABY_CODE = "baby_code"

senseu_ns = cg.esphome_ns.namespace("senseu")
senseu = senseu_ns.class_("SenseU", ble_client.BLEClientNode, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(senseu),
            cv.Optional(CONF_BABY_CODE): cv.string_strict,
            
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)

    if CONF_BABY_CODE in config:
        cg.add(var.set_baby_code(config[CONF_BABY_CODE]))

