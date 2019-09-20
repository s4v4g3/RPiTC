
import copy
import os

import six
import json

class DotDict(dict):
    def __getattribute__(self, name):
        try:
            return self[name]
        except KeyError:
            return dict.__getattribute__(self, name)

    def __setattr__(self, key, value):
        self[key] = value
        return value

    @property
    def __dict__(self):
        return {key: self[key] for key in self}

    def save_to_file(self, file_name):
        with open(file_name, 'w') as f:
            json.dump(self.__dict__, f, indent=3)

    def copy(self):
        return DotDict(self.as_dict())

    def apply(self, new_dict):
        """
        Apply values from new_dict into self

        Args:
            new_dict:

        Returns:
            True if anything was changed
        """
        anything_changed = False;
        for key in new_dict:
            value = new_dict[key]
            if key in self.__dict__:
                curr_value = self[key]
                if type(value) != type(curr_value):
                    raise ValueError()
                if value != curr_value:
                    anything_changed = True
                    self[key] = value
        return anything_changed

    def as_dict(self):
        return self.__dict__

    @staticmethod
    def convert_dict_to_dot_dict(dict_):
        """Given a dictionary, converts it to a DotDict instance.

        Converts nested dictionaries (dictionaries in dictionaries and dictionaries in other iterable objects)

        Args:
            dict_ (dict): The JSON dictionary. We don't care what the dictionary is used for, though

        Returns:
            DotDict: The same dictionary, but with "." access
        """
        dot_dict = DotDict(dict_)
        for key in dict_:
            value = dict_[key]
            if isinstance(value, dict):
                setattr(dot_dict, key, DotDict.convert_dict_to_dot_dict(value))
            if is_list(value):
                encountered_lists = [value]
                for list_value in encountered_lists:
                    for idx, elem in enumerate(list_value):
                        if isinstance(elem, dict):
                            list_value[idx] = DotDict.convert_dict_to_dot_dict(elem)
                        if is_list(elem):
                            encountered_lists.append(elem)
        return dot_dict

def is_list(suspected_list):
    if isinstance(suspected_list, six.string_types):
        return False
    if isinstance(suspected_list, dict):
        return False
    try:
        for _ in suspected_list:
            return True
    except (AttributeError, TypeError):
        return False