import time

__all__ = ["Stopwatch"]

class Stopwatch(object):
    def __init__(self):
        self.start_time = time.time()
        self.stop_time = None

    def reset(self):
        self.start_time = time.time()
        self.stop_time = None

    def stop(self):
        self.stop_time = time.time()

    def elapsed(self):
        if self.stop_time:
            return self.stop_time - self.start_time
        else:
            return time.time() - self.start_time

