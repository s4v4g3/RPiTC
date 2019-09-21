import os
import logging
import logging.config
import inspect


__all__ = ["LoggerMgr", "ConsoleColor"]


class ConsoleColor:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    YELLOW = '\033[93m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    CYAN = '\033[96m'
    RED = '\033[91m'


class LoggerMgr(object):

    @classmethod
    def configure(cls, config):
        logging.config.dictConfig(config)

    @staticmethod
    def get_module_logger():
        # Get calling module name
        try:
            frm = inspect.stack()[2]
            mod = inspect.getmodule(frm[0])
            name_array = mod.__name__
            name = name_array.rpartition('.')[2]
        except (ValueError, ImportError, IndexError, AttributeError, IOError):
            name = ""
        return logging.getLogger(name)

    @staticmethod
    def Info(message, color=ConsoleColor.OKGREEN):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info(message)
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def info(*args, **kwargs):
        return LoggerMgr.Info(*args, **kwargs)

    @staticmethod
    def Warning(message, color=ConsoleColor.WARNING):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.warning(message)
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def warning(*args, **kwargs):
        return LoggerMgr.Warning(*args, **kwargs)

    @staticmethod
    def Error(message, color=ConsoleColor.FAIL):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.error(message)
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def error(*args, **kwargs):
        return LoggerMgr.Error(*args, **kwargs)

    @staticmethod
    def Debug(message, color=ConsoleColor.OKBLUE):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.debug(message)
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def debug(*args, **kwargs):
        return LoggerMgr.Debug(*args, **kwargs)

    @staticmethod
    def PrintComplete(color=ConsoleColor.OKGREEN):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info("Test Complete!")
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def Header(Header, color=ConsoleColor.CYAN):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info(Header)
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def TestStart(TestId, color=ConsoleColor.CYAN):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info("TEST BEGIN - {}".format(TestId))
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def TestStop(TestId, color=ConsoleColor.CYAN):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info("TEST END - {}".format(TestId))
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def Line(color=ConsoleColor.RED):
        _log = LoggerMgr.get_module_logger()
        print(color, end="")
        _log.info("*************************************************************************************")
        print(ConsoleColor.ENDC, end="")

    @staticmethod
    def setConsoleLevel(level):
        for handler in logging.getLogger().handlers:
            if type(handler).__name__ == 'StreamHandler':
                handler.setLevel(level)

    @staticmethod
    def setLogFilePath(path):
        logger = logging.getLogger()
        handler_array = logger.handlers
        for ihandler in range(0, len(handler_array)):
            handler = handler_array[ihandler]
            if type(handler).__name__ == 'RotatingFileHandler':
                from logging.handlers import RotatingFileHandler
                new_handler = RotatingFileHandler(os.path.join(path, os.path.basename(handler.baseFilename)),
                                                  handler.maxBytes, handler.backupCount)
                new_handler.setLevel(handler.level)
                new_handler.setFormatter(handler.formatter)
                logger.removeHandler(handler)
                logger.addHandler(new_handler)
                break

    @staticmethod
    def addTestLogger(path):
        logger = logging.getLogger()
        handler_array = logger.handlers
        for ihandler in range(0, len(handler_array)):
            handler = handler_array[ihandler]
            if type(handler).__name__ in ['RotatingFileHandler', 'ConcurrentRotatingFileHandler']:
                from logging import FileHandler
                test_handler = FileHandler(path)
                test_handler.setLevel(handler.level)
                test_handler.setFormatter(handler.formatter)
                logger.addHandler(test_handler)
                break

        return test_handler

    @staticmethod
    def removeTestLogger(test_handler):
        if test_handler is not None:
            logger = logging.getLogger()
            logger.removeHandler(test_handler)