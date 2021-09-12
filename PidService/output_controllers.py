from data_logger import LoggerMgr, ConsoleColor

__all__ = ["get_output_controller_factory", "MockOutputController"]


class OutputControllerBase(object):
    def set_output(self, output):
        pass


class MockOutputController(OutputControllerBase):
    def __init__(self):
        output = 0

    def set_output(self, output):
        self.output = output


class RPiPWMOutputController(OutputControllerBase):
    def __init__(self):
        import pigpio

        self.pi = pigpio.pi()
        self.pin = 13
        self.pi.set_PWM_range(self.pin, 100)
        self.pi.set_PWM_frequency(self.pin, 0)
        LoggerMgr.debug(
            "RPiPWMOutputController: PWM frequency = {}".format(
                self.pi.get_PWM_frequency(self.pin)
            )
        )
        self.output = 0
        LoggerMgr.debug(
            "RPiPWMOutputController: setting output = {}".format(self.output)
        )
        self.pi.set_PWM_dutycycle(self.pin, self.output)

    def set_output(self, output):
        if output != self.output:
            self.output = output
            LoggerMgr.info(
                "RPiPWMOutputController: setting output = {}".format(self.output)
            )
            self.pi.set_PWM_dutycycle(self.pin, self.output)


class OutputControllerFactory(object):
    def CreateOutputController(self, output_controller_class):
        return None


class RealOutputControllerFactory(OutputControllerFactory):
    def CreateOutputController(self, output_controller_class):
        klass = eval(output_controller_class)
        return klass()


def get_output_controller_factory():
    return RealOutputControllerFactory()
