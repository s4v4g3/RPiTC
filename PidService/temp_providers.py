
__all__ = ["get_temp_provider_factory", "MockTempProvider"]

class TempProviderBase(object):

    def __init__(self, label):
        self.label = label

    def read_temp(self):
        return 0

class MockTempProvider(TempProviderBase):

    def __init__(self, label="Oven"):
        self.temp = 0.0
        super().__init__(label)

    def read_temp(self):
        print('{} temp = {}'.format(self.label, self.temp))
        return self.temp

class RPiMAX6675TempProvider(TempProviderBase):

    def __init__(self, label="Oven"):
        self.temp = 0.0
        super().__init__(label)

    def read_temp(self):
        #print('temp = {}'.format(self.temp))
        return self.temp

class TempProviderFactory(object):

    def CreateTempProvider(self, temp_provider_class, label):
        return None

class RealTempProviderFactory(TempProviderFactory):

    def CreateTempProvider(self, temp_provider_class, label):
        klass = eval(temp_provider_class)
        return klass(label)

def get_temp_provider_factory():
    return RealTempProviderFactory()






