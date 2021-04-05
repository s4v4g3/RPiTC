import copy
import os

import six
import json
from dot_dict import DotDict

__all__ = ["PIDStateModel", "PID_STATE_DEFAULTS", "create_gauges", "update_gauges"]

PID_STATE_DEFAULTS = {
    "last_oven_temp": 0.0,
    "avg_oven_temp": 0.0,
    "output": 0,
    "error_sum": 0.0,
    "opt_probe_temps": {},
}


def create_gauges(gauge_class):
    gauges = {}
    for key in PID_STATE_DEFAULTS:
        if not isinstance(PID_STATE_DEFAULTS[key], dict):
            gauges[key] = gauge_class(f"bbq_state_{key}", f"BBQ State value of {key}")
    return gauges


def update_gauges(gauges, state_model):
    for key in gauges:
        gauges[key].set(state_model[key])


class PIDStateModel(DotDict):
    """
    A wrapper for the JSON PID state

    This class wraps a single DUT value in the dut list in the config. This class makes the DUT information into a
    "dot" accessible dict, but also tries to provide documentation on some of the common keys provided in the JSON file.
    """

    last_oven_temp = None  # type: float
    """float: last oven temperature"""

    avg_oven_temp = None  # type: float
    """float: avg oven temperature"""

    output = None  # type: float
    """float: PID output"""

    p_term = None  # type: float
    """float: proportional term"""

    d_term = None  # type: float
    """float: derivative term"""

    i_term = None  # type: float
    """float: integral term"""

    error_sum = None  # type: float
    """float: sum of PID errors"""

    opt_probe_temps = {}  # type: dict
    """dict: optional probe temperatures - key=label, value=temperature"""

    @classmethod
    def convert_json_data_to_model(cls, json_data, defaults=None):
        if defaults is None:
            defaults = PID_STATE_DEFAULTS
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
            PIDStateModel: The DUT configurations read from the JSON file.
        """

        with open(json_fn, "r") as json_file:
            json_data = json.load(json_file)

        return cls.convert_json_data_to_model(json_data, defaults)
