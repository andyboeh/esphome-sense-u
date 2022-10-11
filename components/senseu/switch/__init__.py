import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import CONF_ID
from .. import senseu_ns, senseu, CONF_SENSEU_ID

DEPENDENCIES = ["senseu"]
CODEOWNERS = ["@andyboeh"]

SenseUSwitch = senseu_ns.class_("SenseUSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SenseUSwitch),
        cv.GenerateID(CONF_SENSEU_ID): cv.use_id(senseu),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    parent = await cg.get_variable(config[CONF_SENSEU_ID])
    cg.add(var.set_senseu_parent(parent))
