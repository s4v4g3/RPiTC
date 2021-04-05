import copy
import os

import six
import json
from dot_dict import DotDict

__all__ = ["PIDConfigModel", "PID_CONFIG_DEFAULTS", "create_gauges", "update_gauges"]

PID_CONFIG_DEFAULTS = {
    "set_point": 225,
    "kb": 0.0,
    "kp": 5.0,
    "ki": 0.005,
    "kd": 1.0,
    "smooth_coeff": (1.0 / 20.0),
    "max_output": 100.0,
}


def create_gauges(gauge_class):
    gauges = {}
    for key in PID_CONFIG_DEFAULTS:
        gauges[key] = gauge_class(f"bbq_config_{key}", f"BBQ Config value of {key}")
    return gauges


def update_gauges(gauges, config_model):
    for key in gauges:
        gauges[key].set(config_model[key])


class PIDConfigModel(DotDict):
    """
    A wrapper for the JSON PID configuration.

    This class wraps a single DUT value in the dut list in the config. This class makes the DUT information into a
    "dot" accessible dict, but also tries to provide documentation on some of the common keys provided in the JSON file.
    """

    set_point = None  # type: int
    """int: PID set point"""

    kb = None  # type: float
    """float: bias PID constant"""
    kp = None  # type: float
    """float: proportional PID constant"""
    ki = None  # type: float
    """float: integral PID constant"""
    kd = None  # type: float
    """float: derivative PID constant"""
    smooth_coeff = None  # type: float
    """float: running average smoothing coefficient"""
    max_output = None  # type: float
    """float: maximum output"""

    @classmethod
    def convert_json_data_to_model(cls, json_data, defaults=None):
        if defaults is None:
            defaults = PID_CONFIG_DEFAULTS
        for default in defaults:
            default_value = defaults[default]
            if default not in json_data:
                json_data[default] = default_value
        return DotDict.convert_dict_to_dot_dict(json_data)

    @classmethod
    def convert_json_file_to_model(cls, json_fn, defaults=None):
        """
        Given a JSON PID configuration model, returns an instance of this class representing that file.

        Args:
            json_fn (str): The path to the file
            defaults (dict[str, object]): A mapping of defaults to use. If None, uses the PID_CONFIG_DEFAULTS map.
                Set to an empty dict (`dict()`) to not use any defaults, or pass your own map.

        Returns:
            PIDConfigModel: The PID configuration read from the JSON file.
        """
        with open(json_fn, "r") as json_file:
            json_data = json.load(json_file)

        return cls.convert_json_data_to_model(json_data, defaults)
