import os
from pid_config_model import PIDConfigModel
from pid_state_model import PIDStateModel
from temp_providers import TempProviderBase
from output_controllers import OutputControllerBase

__all__ = ["PidController"]


class PidController(object):

    def __init__(self):
        pass

    @classmethod
    def __calc_avg_temp(cls, pid_config, pid_state):
        if pid_state.avg_oven_temp == 0:
            pid_state.avg_oven_temp = pid_state.last_oven_temp
        else:
            temp_val = pid_state.last_oven_temp - pid_state.avg_oven_temp
            pid_state.avg_oven_temp += pid_config.smooth_coeff * temp_val

    def pid_iteration(self, interval_s, pid_config, pid_state, oven_temp_provider, opt_temp_providers, output_controller):
        """

        Args:
            self (PidController):
            interval_s (float):
            pid_config (PIDConfigModel):
            pid_state (PIDStateModel):
            oven_temp_provider (TempProviderBase):
            opt_temp_providers (list[TempProviderBase]):
            output_controller (OutputControllerBase):

        Returns:

        """

        pid_state.last_oven_temp = oven_temp_provider.read_temp()
        opt_temps = {}
        for opt_temp_provider in opt_temp_providers:
            opt_temps[opt_temp_provider.label] = opt_temp_provider.read_temp()
        pid_state.opt_probe_temps = opt_temps

        self.__calc_avg_temp(pid_config, pid_state)

        last_output = pid_state.output
        error = pid_config.set_point - pid_state.last_oven_temp

        if True:
            pid_state.i_term = 0
            if (error > 0 and (last_output < pid_config.max_output or pid_state.error_sum < 0)) or (error < 0 and (last_output > 0 or pid_state.error_sum > 0)):
                new_error_sum = pid_state.error_sum + pid_config.ki * error
                if (new_error_sum > 0 and pid_state.error_sum < 0) or (new_error_sum < 0 and pid_state.error_sum > 0):
                    #reset error sum
                    pid_state.error_sum = 0
                else:
                    pid_state.error_sum = new_error_sum
                    pid_state.i_term = pid_config.ki * error

        pid_state.p_term = pid_config.kp * error
        pid_state.d_term = pid_config.kd * (pid_state.avg_oven_temp - pid_state.last_oven_temp)

        output = pid_config.kb
        output += pid_state.p_term
        output += pid_state.d_term
        output += pid_state.error_sum
        if output > pid_config.max_output:
            pid_state.output = pid_config.max_output
        elif output > 0:
            pid_state.output = output
        else:
            pid_state.output = 0

        output_controller.set_output(pid_state.output)






