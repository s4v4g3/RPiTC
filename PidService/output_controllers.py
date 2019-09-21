
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

    def set_output(self, output):
        pass


class OutputControllerFactory(object):

    def CreateOutputController(self, output_controller_class):
        return None

class RealOutputControllerFactory(OutputControllerFactory):

    def CreateOutputController(self, output_controller_class):
        klass = eval(output_controller_class)
        return klass()

def get_output_controller_factory():
    return RealOutputControllerFactory()