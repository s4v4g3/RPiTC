
import json
from dot_dict import DotDict
from pid_state_model import PIDStateModel, PID_STATE_DEFAULTS
from pid_config_model import PIDConfigModel, PID_CONFIG_DEFAULTS
import os

__all__ = ["ConfigModel", "CONFIG_DEFAULTS"]

CONFIG_DEFAULTS = {
  "pid_config": {},
  "pid_state": {},
  "oven_temp_provider": "RPiMAX6675TempProvider",
  "output_controller": "RPiPWMOutputController",
  "opt_temp_providers": [{
       "label": "Food Probe 1",
       "type": "MockTempProvider"
     }],
  "logging": {
        'version': 1,
        'formatters': {
            'default': {'format': '%(asctime)s - %(levelname)s - %(message)s', 'datefmt': '%Y-%m-%d %H:%M:%S'}
        },
        'handlers': {
            'console': {
                'level': 'DEBUG',
                'class': 'logging.StreamHandler',
                'formatter': 'default',
                'stream': 'ext://sys.stdout'
            },
            'file': {
                'level': 'DEBUG',
                'class': 'logging.handlers.RotatingFileHandler',
                'formatter': 'default',
                'filename': 'log.txt',
                'maxBytes': 1024,
                'backupCount': 3
            }
        },
        'loggers': {
            '': {
                'level': 'DEBUG',
                'handlers': ['console', 'file']
            }
        },
        'disable_existing_loggers': True
    }
}



class ConfigModel(DotDict):
    pid_config = None # type: PIDConfigModel
    """PIDConfigModel: PID Config"""

    pid_state = None  # type: PIDStateModel
    """PIDStateModel: PID Config"""

    oven_temp_provider = None  #type: str
    """str: oven temperature provider class"""

    output_controller = None   #type: str
    """str: output controller class"""

    opt_temp_providers = None  #type: list[(str,str)]
    """list[(str,str)]: optional temp providers (list of tuple with provider class and label)"""

    @classmethod
    def load_from_file(cls, json_fn):
        """

        Args:
            json_fn:

        Returns:
            ConfigModel: The configuration read from the JSON file.
        """
        if os.path.isfile(json_fn):
            config_model = cls.convert_json_file_to_model(json_fn)
        else:
            config_model = cls.convert_json_data_to_model({})
            config_model.save_to_file(json_fn)
        pid_config = config_model.pid_config
        config_model.pid_config = PIDConfigModel.convert_json_data_to_model(pid_config)
        pid_state = config_model.pid_state
        config_model.pid_state = PIDStateModel.convert_json_data_to_model(pid_state)
        return config_model


    @classmethod
    def convert_json_data_to_model(cls, json_data, defaults=None):
        """

        Args:
            json_data:
            defaults:

        Returns:
            ConfigModel: The configuration converted from JSON
        """
        if defaults is None:
            defaults = CONFIG_DEFAULTS
        for default in defaults:
            default_value = defaults[default]
            if default not in json_data:
                json_data[default] = default_value
        return DotDict.convert_dict_to_dot_dict(json_data)

    @classmethod
    def convert_json_file_to_model(cls, json_fn, defaults=None):
        """
        Given a JSON configuration model, returns an instance of this class representing that file.

        Args:
            json_fn (str): The path to the file
            defaults (dict[str, object]): A mapping of defaults to use. If None, uses the CONFIG_DEFAULTS map.
                Set to an empty dict (`dict()`) to not use any defaults, or pass your own map.

        Returns:
            ConfigModel: The configuration read from the JSON file.
        """
        with open(json_fn, 'r') as json_file:
            json_data = json.load(json_file)

        return cls.convert_json_data_to_model(json_data, defaults)
