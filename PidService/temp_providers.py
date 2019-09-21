import math
from data_logger import LoggerMgr

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
        return self.temp

#constants
SPI_BYTES_TO_READ = 2

class RPiMAX6675TempProvider(TempProviderBase):


    def __init__(self, label="Oven"):
        self.temp = 0.0
        super().__init__(label)
        import pigpio
        self.pi = pigpio.pi()

        if not self.pi.connected:
            raise OSError("unable to connect to pigpio")
        # pi.spi_open(0, 1000000, 0)   # CE0, 1Mbps, main SPI
        # pi.spi_open(1, 1000000, 0)   # CE1, 1Mbps, main SPI
        # pi.spi_open(0, 1000000, 256) # CE0, 1Mbps, auxiliary SPI
        # pi.spi_open(1, 1000000, 256) # CE1, 1Mbps, auxiliary SPI
        # pi.spi_open(2, 1000000, 256) # CE2, 1Mbps, auxiliary SPI
        self.sensor = self.pi.spi_open(0, 1000000, 0)

    def read_temp(self):
        for i in range(0, 5):

            count, d = self.pi.spi_read(self.sensor, SPI_BYTES_TO_READ)
            if count == SPI_BYTES_TO_READ:
                word = (d[0] << 8) | d[1]
                if (word & 0x8006) == 0:  # Bits 15, 2, and 1 should be zero.
                    t = (word >> 3) / 4.0
                    self.temp = t * 1.8 + 32
                    return self.temp
                else:
                    continue
        # unable to read after 5 tries
        self.temp = float('nan')
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






